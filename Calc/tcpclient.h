#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

struct frame {
    int m1;
    int m2;
    int op;
};

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    ~TcpClient();
    void setAddress(QString &address);
    void setPort(QString &port);
    QString getAddress() const;
    QString getPort() const;
    void sendData(const QString &data);

signals:
    void client_connected();
    void client_disconnected();
    void haveMessage(QString message);
    void haveResult(double result);

private slots:
    void readyRead();

private:
    QTcpSocket *socket;
    QString m_address;
    QString m_port;

    frame convertDataToFrame(const QString&);
    QByteArray data;
public slots:
    void tryConnect();
    void tryDisconnect();
};

#endif // TCPCLIENT_H
