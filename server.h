#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTime>
#include <QImage>
#include <QBuffer>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    QTcpSocket* acceptSocket;
    explicit Server(QTcpServer *parent = nullptr);

signals:
    //void newConnection(Connection *connection);
    void showMessage(QString str);

protected:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

private:
    quint16 nextBlockSize;
    QVector<QTcpSocket*> socketsList;
    QByteArray Data;
    void SendtoClient(QList<QImage> image, QString str);
};
#endif // SERVER_H
