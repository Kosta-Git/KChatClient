#ifndef KCHAT_H
#define KCHAT_H

#include <QWidget>
#include <QtWidgets>
#include <QtNetwork>

namespace Ui {
class KChat;
}

class KChat : public QWidget
{

    Q_OBJECT

private:
    Ui::KChat *ui;
    QTcpSocket *m_socket;
    quint16 m_size;

public slots:
    void on_connect_clicked();
    void on_send_clicked();
    void on_message_returnPressed();
    void dataReceived();
    void co();
    void dc();
    void socketError(QAbstractSocket::SocketError e);

public:
    explicit KChat(QWidget *parent = 0);
    ~KChat();

};

#endif // KCHAT_H
