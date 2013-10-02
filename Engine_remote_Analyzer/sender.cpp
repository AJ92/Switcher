#include "sender.h"

#include <QtNetwork/QTcpSocket>
Sender::Sender(QString host,int port)
{
    connection = 0;
    //connect
    sock=new QTcpSocket(this);
    connect(sock,SIGNAL(connected()),this,SLOT(connSock()));
    connect(sock,SIGNAL(disconnected()),this,SLOT(dconSock()));
    connect(sock,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(dconSock()));
    connect(sock,SIGNAL(readyRead()),this,SLOT(readSock()));
    sock->connectToHost(host,port);
    qDebug("connected???");
}

Sender::~Sender()
{
        //close the socket
        if(sock)
                sock->close();
}

void Sender::connSock()
{
    qDebug("connected!!!");
    connection = 1;
}

void Sender::dconSock()
{
        //make sure there are no more signals from the socket
        sock->disconnect();
        //disconnect & delete
        sock->close();
        sock->deleteLater();
        sock=0;
        //inform user
        //QMessageBox::warning(,"Disconnected","The connection has been closed.");
        qDebug("disconnected!!!");
}

void Sender::readSock()
{
        //receive
        //buffer.append(QString::fromUtf8(sock->readAll()));
        buf+=QString::fromUtf8(sock->readAll());
        //qDebug(buf.toUtf8());
        //process data
        while(processServer());
}


bool Sender::processServer()
{
    //is there a complete line?
    int pos=buf.indexOf(';');
    if(pos<0)return false;

    //cut the line out
    QString ln=buf.left(pos).trimmed();

    buf=buf.mid(pos+1);
    //is this an empty line? yes: continue with next line
    if(ln.length()==0)return true;

    server_message(ln);

    QStringList list = ln.split(QRegExp("\\s+"),QString::SkipEmptyParts); //SkipEmptyParts
    if(list.at(0) == "connected")
    {
        qDebug("connected to a server");
    }
    return true;
}


void Sender::doDisconnect()
{
    sock->disconnectFromHost();
}


int Sender::send_msg(QString msg)
{
        //sanity check
        if(!sock)return 0;
        //send
        sock->write(msg.toUtf8());
        return 1;
}
