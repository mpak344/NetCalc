#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QDebug>
#include <tcpclient.h>
#include <QDateTime>
namespace Ui {
class MainWindow;
class Dialog;
}

/********************************************************/
class Dialog : public QDialog {
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = 0);
    QString getAddress()const;
    QString getPort()const;
    ~Dialog();
private:
    Ui::Dialog *dialog;
signals:
    void connectClicked();
    void disconnectClicked();
public slots:
    void clientConnected();
    void clientDisconnected();
};

/********************************************************/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addToField();
    void addExpression();
    void equaly();
    void menuTriggered(QAction* action);
    void onPrintMessage(QString message);
    void onResult(double result);

    void connectToServer();
    void disconnectFromServer();

private:
    Ui::MainWindow *mw;
    Dialog *dialog;
    TcpClient *tcpClient;
    QString expression;

    bool clearResult;

};

#endif // MAINWINDOW_H
