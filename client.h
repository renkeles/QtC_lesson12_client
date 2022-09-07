#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_returnPressed();

private:
    Ui::Client *ui;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;
    QByteArray Data;
    void SendToServer(QString str);
    quint16 nextBlockSize;

public slots:
    void slotReadyRead();
};
#endif // CLIENT_H
