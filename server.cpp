#include "server.h"

Server::Server(QTcpServer *parent)
    : QTcpServer(parent)
{
    listen(QHostAddress::Any, 2323);
    qDebug() << "server start works";
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    acceptSocket = new QTcpSocket();
    acceptSocket->setSocketDescriptor(socketDescriptor);
    connect(acceptSocket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(acceptSocket, &QTcpSocket::disconnected, acceptSocket, &QTcpSocket::deleteLater);

    socketsList.push_back(acceptSocket);
    qDebug() << QTime::currentTime() << ": " << "Client connected" << socketDescriptor;

    QString ptrStr = QString( "0x%1" ).arg( reinterpret_cast<qintptr>(socketDescriptor),
                                         QT_POINTER_SIZE * 2, 16, QChar('0') );
    emit showMessage(QTime::currentTime().toString() + ": " + ptrStr + " connected");
    nextBlockSize = 0;
}


void Server::slotReadyRead()
{
    Data.clear();
    acceptSocket = (QTcpSocket*)sender();
    QDataStream in(acceptSocket);
    in.setVersion(QDataStream::Qt_6_7);

    while (!in.atEnd()){
        QString str;
        QTime time;
        QList<QImage> images;
        in >> time >> str;

        while (!in.atEnd()) {
            QByteArray imageData;
            imageData.clear();
            in >> imageData;
            QImage image;
            image.loadFromData(imageData);
            images.append(image);
        }

        if(!str.isEmpty() && !images.isEmpty()){
            emit showMessage(time.toString() + ": " + str + " " + " Sended images");
            SendtoClient(images, str);
        }
        else if(!str.isEmpty() && images.isEmpty()){
            emit showMessage(time.toString() + ": " + str);
            SendtoClient(images, str);
        }
        else if(str.isEmpty() && !images.isEmpty()){
            emit showMessage(time.toString() + ": Sended images");
            SendtoClient(images, str);
        }
    }

    //QImage image;
    //image.loadFromData(imageData);
    //images.append(image);
    //if(!image.isNull())

/*
    if(in.status()==QDataStream::Ok)
    {
        while(true){
            if(nextBlockSize == 0){
                if(acceptSocket->bytesAvailable() < 2){
                    break;
                }
                in >> nextBlockSize;
            }
            if(acceptSocket->bytesAvailable() < nextBlockSize){
                break;
            }

            in >> time >> str >> imageData;
            QImage image;
            image.loadFromData(imageData);
            images.append(image);
            //if(!image.isNull())
            nextBlockSize = 0;
            if(!str.isEmpty() || !images.empty()){
                emit showMessage(time.toString() + ": " + str);
                SendtoClient(images, str);
            }
            if(!str.isEmpty())
            break;
        }

        str = "";

    }
    else
    {
        showMessage("Error");
    }
*/
}


void Server::SendtoClient(QList<QImage> images, QString str)
{
    /*
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);
    if(image.isNull())
        out << qint16(0) << QTime::currentTime() << str;
    else out << qint16(0) << QTime::currentTime() << image << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    */

    Data.clear();

    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);

    //out << qint16(0) << QTime::currentTime() << str;
    out << QTime::currentTime() << str;


    for(auto& image: images){
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        out << imageData;
    }
    images.clear();

   // if(!images.isEmpty()){
        //image.save(&buffer, "PNG");

        //message.imageData = imagesData;

    //}

    //out.device()->seek(0);
    //out << quint16(Data.size() - sizeof(quint16));

    for(QTcpSocket* user: socketsList)
        user->write(Data);
}

