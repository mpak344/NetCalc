#include "server.h"
#include <QDebug>
#include <calcclient.h>
Server::Server(QObject *parent)
    : QTcpServer(parent)
{

}

void Server::startServer()
{
    if(this->listen(QHostAddress::Any,1234)) {
        qDebug() << "Server started";
    }
    else {
        qDebug() << "Server didn't started";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    // На каждое новое подключение создаём новый поток для обработки входных данных.
    qDebug() << "Have incomingConnection" << socketDescriptor;
    QThread *thread= new QThread;
    CalcClient *client = new CalcClient(socketDescriptor);
    client->moveToThread(thread);
    connect(thread, SIGNAL(started()), client, SLOT(doWork()), Qt::QueuedConnection); // когда поток запустится запускаем работу

    // после того, как tcp сокет отключится завершаем поток, очищаем динамически выделенную память.
    connect(client, SIGNAL(disconnected()), thread, SLOT(quit()), Qt::QueuedConnection);
    connect(client, SIGNAL(disconnected()), thread, SLOT(deleteLater()), Qt::QueuedConnection);
    connect(client, SIGNAL(disconnected()), client, SLOT(deleteLater()), Qt::QueuedConnection);
    thread->start();
}
