#include "databuffer.h"
#include <cstring>
#include <QException>

//构造
DataBuffer::DataBuffer()
{
    m_nBufferSize = BUFFER_SIZE;  //缓冲区大小
    m_nStart = 0;                 //数据开始位置
    m_nEnd = 0;                   //数据结束位置
    m_isFull = false;             //缓冲区是否已满
    m_isEmpty = true;             //缓冲区是否为空
    m_pBuffer = new char[m_nBufferSize];      //分配缓冲区
    memset(m_pBuffer, 0, sizeof(m_pBuffer));  //清空缓冲区
}

//析构
DataBuffer::~DataBuffer()
{
    delete [] m_pBuffer;  //释放缓冲区
    m_pBuffer = NULL;
    m_nBufferSize = 0;
}

//获得缓冲区中数据大小
int DataBuffer::getDataLen()
{
    if(m_isFull)
    {
        return m_nBufferSize;
    }
    else if(m_nEnd < m_nStart)
    {
        return (m_nBufferSize - m_nStart) + m_nEnd;
    }
    else
    {
        return m_nEnd - m_nStart;
    }
}


//重置缓冲区大小
bool DataBuffer::reBufferSize(int nLen)
{
    char *oBuffer = m_pBuffer;  //保存原缓冲区地址
    try
    {
        nLen = nLen < 64 ? 64: nLen;  //保证最小大小
        while(m_nBufferSize < nLen)
        {
            m_nBufferSize *= 2;
        }
        m_pBuffer = new char[m_nBufferSize]; //分配新缓冲区
        memset(m_pBuffer, 0, sizeof(m_pBuffer));

        //将原缓冲区中的内容拷贝到新缓冲区
        if(m_nStart < m_nEnd)
        {
            memcpy(m_pBuffer, oBuffer + m_nStart, m_nEnd - m_nStart);
        }
        else
        {
            int len1 = m_nBufferSize - m_nStart;
            memcpy(m_pBuffer, oBuffer + m_nStart, len1);
            memcpy(m_pBuffer + len1, oBuffer, m_nEnd);
        }

        delete []oBuffer;  //释放原缓冲区
    }
    catch(QException e)
    {
        return false;
    }
    return true;
}

//向缓冲区中添加消息
/*
 * pBuf，要添加的数据
 * nLen，数据长度
 * 成功返回true，失败返回false
 */
bool DataBuffer::addMsg(char *pBuf, int nLen)
{
    try
    {
        if(nLen == 0 || pBuf == NULL)
        {
            return false;
        }
        if(getDataLen() + nLen > m_nBufferSize)       //如果缓冲区过小，重新调整其大小
        {
            reBufferSize(getDataLen() + nLen);
            memcpy(m_pBuffer + m_nEnd, pBuf, nLen);   //将数据添加到缓冲区尾
            m_nEnd += nLen;
            m_isFull = m_nStart == m_nEnd;
        }
        else if(m_nStart <= m_nEnd)
        {
            int rightLen = m_nBufferSize - m_nEnd;    //缓冲区右半部分长度
            if(nLen <= rightLen)
            {
                memcpy(m_pBuffer + m_nEnd, pBuf, nLen);
                m_nEnd += nLen;
                m_isFull = m_nStart == m_nEnd;
            }
            else
            {
                int leftLen = nLen - rightLen;        //剩余数据长度
                memcpy(m_pBuffer + m_nEnd, pBuf, rightLen);
                memcpy(m_pBuffer, pBuf, leftLen);
                m_nEnd = leftLen;
                m_isFull = m_nStart == m_nEnd;
            }
        }
        else
        {
            memcpy(m_pBuffer + m_nEnd, pBuf, nLen);
            m_nEnd += nLen;
            m_isFull = m_nStart == m_nEnd;
        }
        m_isEmpty = false;
    }
    catch(QException e)
    {
        return false;
    }
    return true;
}

//缓冲区复位
void DataBuffer::reset()
{
    if(!m_isEmpty)
    {
        memset(m_pBuffer, 0, sizeof(m_pBuffer));
        m_nStart = 0;
        m_nEnd = 0;
    }
}

//移除缓冲区首部第一个数据包
//nLen：一个数据包的大小
void DataBuffer::poll(int nLen)
{
    if(m_isEmpty || getDataLen() < nLen || nLen == 0 || m_pBuffer == NULL)
    {
        return;
    }

    if(m_nStart < m_nEnd)
    {
        m_nStart += nLen;
        m_isEmpty = m_nStart == m_nEnd;
    }
    else
    {
        int rightLen;
        if(m_nStart == m_nEnd)
        {
            rightLen = m_nBufferSize - m_nEnd;    //缓冲区右半部分长度
        }
        else
        {
            rightLen = m_nBufferSize - m_nStart;  //右半部分数据长度
        }

        if(nLen <= rightLen)   //如果数据包大小 < 缓冲区右面的数据
        {
            m_nStart += nLen;
            m_isEmpty = m_nStart == m_nEnd;
        }
        else
        {
            int leftLen = nLen - rightLen;
            m_nStart = leftLen;
            m_isEmpty = m_nStart == m_nEnd;
        }
    }
}
