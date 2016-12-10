#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "calcclient.h"
class Server : public QTcpServer
{
public:
    explicit Server(QObject *parent = 0);
    void startServer();

protected:

    virtual void incomingConnection(qintptr socketDescriptor);

};

#endif // SERVER_H
