#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mw(new Ui::MainWindow),
    dialog(new Dialog(this)),
    tcpClient(new TcpClient(this))
{
    mw->setupUi(this);
    // обрабатываем нажатие нумерных кнопок
    connect(mw->pushButton_0, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    connect(mw->pushButton_1, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    connect(mw->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    connect(mw->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    connect(mw->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    connect(mw->pushButton_5, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    connect(mw->pushButton_6, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    connect(mw->pushButton_7, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    connect(mw->pushButton_8, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    connect(mw->pushButton_9, SIGNAL(clicked(bool)), this, SLOT(addToField()));
    // обрабатываем нажатия на кнопки операций
    connect(mw->pushButtonPlus, SIGNAL(clicked(bool)), this, SLOT(addExpression()));
    connect(mw->pushButtonMinus, SIGNAL(clicked(bool)), this, SLOT(addExpression()));
    connect(mw->pushButtonPart, SIGNAL(clicked(bool)), this, SLOT(addExpression()));
    connect(mw->pushButtonMultiply, SIGNAL(clicked(bool)), this, SLOT(addExpression()));
    // обрабатываем кнопку "="
    connect(mw->pushButtonEqually, SIGNAL(clicked(bool)), this, SLOT(equaly()));

    // обработка сигналов/слотов из диалога.
    connect(mw->menuMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));
    connect(dialog, SIGNAL(connectClicked()),this, SLOT(connectToServer()));
    connect(dialog, SIGNAL(disconnectClicked()),this, SLOT(disconnectFromServer()));
    connect(tcpClient, SIGNAL(client_connected()), dialog, SLOT(clientConnected()));
    connect(tcpClient, SIGNAL(client_disconnected()), dialog, SLOT(clientDisconnected()));
    connect(tcpClient, SIGNAL(haveMessage(QString)), this, SLOT(onPrintMessage(QString)));
    connect(tcpClient, SIGNAL(haveResult(double)), this, SLOT(onResult(double)));
}

MainWindow::~MainWindow()
{
    delete mw;
}

void MainWindow::connectToServer()
{
    QString adress = dialog->getAddress();
    QString port = dialog->getPort();
    tcpClient->setAddress(adress);
    tcpClient->setPort(port);
    tcpClient->tryConnect();
}

void MainWindow::disconnectFromServer()
{
    tcpClient->tryDisconnect();
}

void MainWindow::addToField()
{
    if(clearResult){
        mw->lineEdit->clear();
        clearResult = false;
    }
    // берём указатель на объект, который посылает сигнал и приводим его к указателю на QPushButton
    QPushButton *s = dynamic_cast<QPushButton*>(sender());
    if(s == nullptr)
        return;
    mw->lineEdit->setText(mw->lineEdit->text().append(s->text()));
}

void MainWindow::addExpression()
{
    if(mw->lineEdit->text().isEmpty())
        return;
    expression.clear();
    QPushButton *s = dynamic_cast<QPushButton*>(sender());
    if(s == nullptr)
        return;
    expression.append(mw->lineEdit->text() + s->text());
    mw->lineEdit->clear();
}

void MainWindow::equaly()
{
    if(expression.isEmpty()|| mw->lineEdit->text().isEmpty())
        return;
    expression.append(mw->lineEdit->text());
    tcpClient->sendData(expression);
    mw->lineEdit->clear();
    expression.clear();
    clearResult = true;
}

void MainWindow::menuTriggered(QAction *action)
{
    if(action->text() == "Settings") {
        if(dialog->isVisible())
            dialog->hide();
        else
            dialog->show();
    }
}

void MainWindow::onPrintMessage(QString message)
{
    mw->textBrowser->append(QDateTime::currentDateTime().toString().append(": ").append(message));
}

void MainWindow::onResult(double result)
{
    mw->lineEdit->setText(QString::number(result));
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    dialog(new Ui::Dialog)
{
    dialog->setupUi(this);
    connect(dialog->pushButton, SIGNAL(clicked(bool)), this, SIGNAL(connectClicked()));
    connect(dialog->pushButton_2, SIGNAL(clicked(bool)), this, SIGNAL(disconnectClicked()));
}

QString Dialog::getAddress() const
{
    return dialog->lineEdit->text();
}

QString Dialog::getPort() const
{
    return dialog->lineEdit_2->text();
}

Dialog::~Dialog()
{
    delete dialog;
}

void Dialog::clientConnected()
{
    dialog->pushButton->setEnabled(false);
    dialog->pushButton_2->setEnabled(true);
}

void Dialog::clientDisconnected()
{
    dialog->pushButton->setEnabled(true);
    dialog->pushButton_2->setEnabled(false);
}
