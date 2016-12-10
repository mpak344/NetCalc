#ifndef CALCCLIENT_H
#define CALCCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QDataStream>
class CalcClient : public QObject
{
    Q_OBJECT
public:
    explicit CalcClient(qintptr socketDescriptor);

signals:
    void sendResult(int, double);
    void disconnected();
public slots:
    void onSendResult(int, double);
    void readyRead();
    void onDisconnected();

    void doWork();

private:
    qintptr m_socketDescriptor;
    QTcpSocket *socket;
    double calculate(int,int,int);

    QByteArray data;


};

#endif // CALCCLIENT_H
