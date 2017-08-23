#include "kchat.h"
#include "ui_kchat.h"
#include <QtNetwork>

KChat::KChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KChat)
{

    ui->setupUi(this);

    ui->textArea->setReadOnly(true);

    m_socket = new QTcpSocket(this);

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(dataReceived()) );
    connect(m_socket, SIGNAL(connected()), this, SLOT(co()) );
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(dc()) );
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)) );

    ui->send->setEnabled(false);

    m_size = 0;

}

void KChat::on_connect_clicked()
{

    ui->textArea->append("<h2>Attempting to connect...</h2>");

    ui->connect->setEnabled(false);

    m_socket->abort();

    m_socket->connectToHost(ui->ip->text(), ui->port->value());

}

void KChat::on_send_clicked()
{

    QByteArray packet;

    QDataStream data(&packet, QIODevice::WriteOnly );

    QString message = ui->username->text() + ": " + ui->message->text();

    data << (quint16) 0;

    data << message;

    data.device()->seek(0);

    data << (quint16)(packet.size() - sizeof(quint16) );

    m_socket->write(packet);

    ui->message->clear();

    ui->message->setFocus();

}

void KChat::on_message_returnPressed()
{

    on_send_clicked();

}

void KChat::dataReceived()
{

    QDataStream data(m_socket);

    if(!m_size){

        if(m_socket->bytesAvailable() < (int)sizeof(quint16) )
            return;

        data >> m_size;

    }

    if(m_socket->bytesAvailable() < m_size)
        return;

    QString message;

    data >> message;

    ui->textArea->append(message);

    m_size = 0;

}

void KChat::co()
{

    ui->textArea->append("<u>A connection was established</u>");

    ui->connect->setEnabled(true);

    ui->send->setEnabled(true);

}

void KChat::dc()
{

    ui->textArea->append("<u>The connection dropped</u>");

}

void KChat::socketError(QAbstractSocket::SocketError e)
{

    switch(e){

    case QAbstractSocket::HostNotFoundError:
                ui->textArea->append(tr("<em>ERROR: Unable to find host server.</em>"));
                break;

    case QAbstractSocket::ConnectionRefusedError:

                ui->textArea->append(tr("<em>ERROR: Host refused connection.</em>"));

                break;

    case QAbstractSocket::RemoteHostClosedError:

                ui->textArea->append(tr("<em>ERROR: Server dropped connection.</em>"));

                break;

    default:
        ui->textArea->append(tr("<em>ERROR : ") + m_socket->errorString() + tr("</em>"));

    }

    ui->connect->setEnabled(true);

}

KChat::~KChat()
{

    delete ui;

}
