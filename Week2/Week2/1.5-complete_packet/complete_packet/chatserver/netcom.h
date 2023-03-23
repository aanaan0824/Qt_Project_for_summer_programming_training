#ifndef NETCOM_H
#define NETCOM_H


#include <QTcpSocket>
#include <QDebug>
#include "databuffer.h"

#pragma pack(push, 1) //采用1字节对齐方式

//包头
typedef struct
{
    int nLen;  //包体长度
}PacketHead;

//封包对象：包头 + 包体
typedef struct
{
    PacketHead head;  //包头
    char *body;       //包体
}Packet;

#pragma pack(pop)

class NetCom : public QObject
{
    Q_OBJECT
public:
    QTcpSocket *m_tcpSocket;  //通信套接字
    DataBuffer m_Buffer;      //套接字关联的缓冲区

    void packData(char *data, int nLen);   //封包，发送
    void unpackData(char *data, int nLen); //将接收到的数据放在缓冲区后，解包

    NetCom();
    ~NetCom();
signals:
    void recv(char *data);
};

#endif // NETCOM_H
