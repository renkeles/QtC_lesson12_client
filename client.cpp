#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
    connect(tcpSocket, &QTcpSocket::disconnected, tcpSocket, &QTcpSocket::deleteLater);
}

Client::~Client()
{
    delete ui;
}


void Client::on_pushButton_clicked()
{
    tcpSocket->connectToHost("127.0.0.1", 2323);
}

void Client::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    out << str;
    tcpSocket->write(Data);
    ui->lineEdit->clear();
}

void Client::slotReadyRead()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    if(in.status()==QDataStream::Ok)
    {
        QString str;
        in >> str;
        ui->textBrowser->append(str);
    }else
    {
        ui->textBrowser->append("Read error");
    }
}


void Client::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());
}


void Client::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}

