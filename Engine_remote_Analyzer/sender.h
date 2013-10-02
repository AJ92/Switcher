#ifndef SENDER_H
#define SENDER_H

#include <QWidget>
#include <QString>

class QTcpSocket;
class QTextEdit;

class Sender : public QWidget
{
    Q_OBJECT
public:
    Sender(QString host,int port);
    ~Sender();

    int send_msg(QString msg);

private slots:
        //socket has been connected
        void connSock();
        //socket has been disconnected (or error)
        void dconSock();
        //socket is readable
        void readSock();

        //enforce disconnect
        void doDisconnect();


private:
        //the socket
        QTcpSocket*sock;
        //stores current text
        QString buf;

        int connection;

        bool processServer();

signals:
        void server_message(QString msg);
        void server_connected(QString adress);

};

#endif // SENDER_H
