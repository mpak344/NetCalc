#include "calcclient.h"

CalcClient::CalcClient(qintptr socketDescriptor)
{
    m_socketDescriptor = socketDescriptor;
}

void CalcClient::onSendResult(int error, double result)
{
    //отправляем ответ
    QByteArray dataOut;
    QDataStream streamOut(&dataOut, QIODevice::WriteOnly);
    streamOut << error;
    streamOut << result;
    socket->write(dataOut);
    socket->flush();
}

void CalcClient::readyRead()
{
    // сериализуем полученные данные и вычисляем значение выражения.
    int error = 400;
    double result = 0.0;
    data.append(socket->readAll());
    if(data.size() >= sizeof(qint16)) {
        QDataStream  stream(data);
        qint16 size;
        stream >> size;
        if(data.size() == size) {
            int m1,m2;
            int op;
            error = 200;
            stream >> m1;
            stream >> m2;
            stream >> op;
            result = calculate(m1, m2, op);
            data.clear();
            emit sendResult(error, result);
        }
    }
}

void CalcClient::onDisconnected()
{
    qDebug() << "Client" << m_socketDescriptor << "disconnected";
    socket->deleteLater();
}

void CalcClient::doWork()
{

    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(m_socketDescriptor)) {
        qDebug() << "Socked descriptor not accepted";
        socket->deleteLater();
        emit disconnected();
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(this, SIGNAL(sendResult(int, double)), this, SLOT(onSendResult(int, double)));

    qDebug() << "Client" <<m_socketDescriptor << "connected";
}

double CalcClient::calculate(int m1 , int m2, int op)
{
    double res;
    switch (op) {
    case 0:
        res = m1 + m2;
        qDebug() << m1<< "+" << m2 << "=" << res;
        break;
    case 1:
        res = m1 - m2;
        qDebug() << m1<< "-" << m2 << "=" << res;
        break;
    case 2:
        res = m1 * m2;
        qDebug() << m1<< "*" << m2 << "=" << res;
        break;
    case 3:
        res = (double)m1 / (double)m2;
        qDebug() << m1<< "/" << m2 << "=" << res;
        break;
    }
    return res;
}

