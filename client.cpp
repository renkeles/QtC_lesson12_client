#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    /*if(ui->comboBox->currentText() == "TCP")
    {*/
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
        connect(tcpSocket, &QTcpSocket::disconnected, tcpSocket, &QTcpSocket::deleteLater);
        nextBlockSize = 0;
  /*  }else if(ui->comboBox->currentText() == "UDP")
    {
        udpSocket = new QUdpSocket(this);
        connect(udpSocket, &QUdpSocket::readyRead, this, &Client::slotReadyRead);

        if(!udpSocket->bind(ui->lineEdit_port->text().toInt()))
        {
            qDebug() << "Error, udp socket create error!";
        }
    }
*/
}

Client::~Client()
{
    delete ui;
}


void Client::on_pushButton_clicked()
{
    tcpSocket->connectToHost("127.0.0.1", ui->lineEdit_port->text().toInt());
    /*
    if(ui->comboBox->currentText() == "TCP")
    {
        qDebug() << "Enabled TCP protocol";
        tcpSocket->connectToHost("127.0.0.1", ui->lineEdit_port->text().toInt());
    }else if(ui->comboBox->currentText() == "UDP")
    {
        qDebug() << "Enabled UDP protocol";

    }*/

}

void Client::SendToServer(QString str)
{
   /* if(ui->comboBox->currentText() == "TCP")
    {*/
        Data.clear();
        QDataStream out(&Data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        out << quint16(0) << QTime::currentTime() << str;
        out.device()->seek(0);
        out << quint16(Data.size() - sizeof(quint16));
        tcpSocket->write(Data);
        ui->lineEdit->clear();

   /* }else if(ui->comboBox->currentText() == "UDP")
    {
        qDebug() << str;
        Data.clear();
        QDataStream out(&Data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        out << str.toUtf8();
        udpSocket->writeDatagram(Data, QHostAddress::LocalHost, ui->lineEdit_port->text().toInt());
    }*/

}

void Client::slotReadyRead()
{
   /* if(ui->comboBox->currentText() == "TCP")
    {*/
        QDataStream in(tcpSocket);
        in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        if(in.status()==QDataStream::Ok)
        {
            /*QString str;
            in >> str;
            ui->textBrowser->append(str);*/
            for (; ; )
            {
                if(nextBlockSize == 0)
                {
                    if(tcpSocket->bytesAvailable() < 2)
                    {
                        break;
                    }
                    in >> nextBlockSize;
                }
                if(tcpSocket->bytesAvailable() < nextBlockSize)
                {
                    break;
                }

                    QString str;
                    QTime time;
                    in >> time >> str;
                    nextBlockSize = 0;
                    ui->textBrowser->append(time.toString() + ": " + str);
            }

        }else
        {
            ui->textBrowser->append("Read error");
        }
 /*   }else if(ui->comboBox->currentText() == "UDP")
    {
        qDebug() << "slotReadyRead";
    }
*/
}


void Client::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());
}


void Client::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}

