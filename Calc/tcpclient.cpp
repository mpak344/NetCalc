#include "tcpclient.h"
#include <QDataStream>
#include <QByteArray>
TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(connected()), this, SIGNAL(client_connected()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(client_disconnected()));

    connect(socket, &QTcpSocket::disconnected, [=] {
        haveMessage(QString().append("Disconnected from host ").append(m_address));
    });
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

TcpClient::~TcpClient()
{
    delete socket;
}

void TcpClient::setAddress(QString &address)
{
    m_address = address;
}

void TcpClient::setPort(QString &port)
{
    m_port = port;
}

QString TcpClient::getAddress() const
{
    return m_address;
}

QString TcpClient::getPort() const
{
    return m_port;
}

void TcpClient::sendData(const QString &data)
{
    // формируем пакет и отправляем на сервер
    if (socket->state() != QAbstractSocket::ConnectedState) {
        emit haveMessage("Client isn't connected to server.");
        return;
    }
    frame f = convertDataToFrame(data);
    QByteArray dataOut;
    QDataStream streamOut(&dataOut, QIODevice::WriteOnly);
    streamOut.setVersion(QDataStream::Qt_5_6);
    streamOut << qint16(0);
    streamOut << f.m1;
    streamOut << f.m2;
    streamOut << f.op;

    streamOut.device()->seek(0);
    streamOut << qint16(dataOut.size());
    socket->write(dataOut);
    socket->flush();
}


void TcpClient::readyRead()
{
    // сериализуем ответ от сервера
    data = socket->readAll();
    if(data.size() == sizeof(int) + sizeof(double)) {
        QDataStream  stream(data);
        int error;
        double result;
        stream >> error;
        stream >> result;
        if(error == 200) {
            emit haveMessage(QString().append(" Server ").append(m_address).append("returned result: ").append(QString::number(result)));
            emit haveResult(result);
        } else {
            emit haveMessage(QString().append("Server received error: ").append(QString::number(result)));
        }
        data.clear();
    }
}

frame TcpClient::convertDataToFrame(const QString &data)
{
    frame f;
    QStringList l;
    if(data.split("+").size() == 2) {
        l = data.split("+");
        f.op = 0;
    }
    if(data.split("-").size() == 2) {
        l = data.split("-");
        f.op = 1;
    }
    if(data.split("*").size() == 2) {
        l = data.split("*");
        f.op = 2;
    }
    if(data.split("/").size() == 2) {
        l = data.split("/");
        f.op = 3;
    }
    if(l.size() == 2) {
        f.m1 = l[0].toLongLong();
        f.m2 = l[1].toLongLong();
        return f;
    }
    else {
        return {-1, -1, -1};
    }

}


void TcpClient::tryConnect()
{
    socket->connectToHost(m_address, m_port.toInt());
    emit haveMessage(QString().append("Connected to host ").append(m_address));
}

void TcpClient::tryDisconnect()
{
    socket->disconnectFromHost();
}
