#ifndef DATABUFFER_H
#define DATABUFFER_H

#define BUFFER_SIZE  1024  //初始缓冲区大小

class DataBuffer
{
public:
    char *m_pBuffer;    //缓冲区
    int m_nBufferSize;  //缓冲区大小
    int m_nStart;       //数据开始位置
    int m_nEnd;         //数据结束位置
    bool m_isFull;      //缓冲区是否已满
    bool m_isEmpty;     //缓冲区是否为空

    int getDataLen();            //获得缓冲区中数据大小
    bool reBufferSize(int nLen); //调整缓冲区大小
    bool addMsg(char *pBuf, int nLen);  //添加消息到缓冲区
    void poll(int nLen);   //移除缓冲区首部的第一个数据包
    void reset();          //缓冲区复位

 public:
    DataBuffer();
    ~DataBuffer();
};

#endif // DATABUFFER_H
