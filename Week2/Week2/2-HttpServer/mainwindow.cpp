#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <Qdebug>
#include <QThread>
#include <QString>
#include <Qdir>
#include <QFile>
#include <time.h>
#include <QDateTime>

#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QTcpServer();

    m_sigmap = new QSignalMapper(this);

    int i;
    for(i=0;i<MAX_CONNECTIONS;i++){
        m_used[i]=false;
    }
    m_active_conn=0;
    m_pivot =0;
    qDebug() << "Directory of file: " << QDir::currentPath();
}

void MainWindow::server_New_Connect() {
    QString info;
    //获取客户端连接

    while (server->hasPendingConnections()) {
        QTcpSocket * lsock = server->nextPendingConnection();//根据当前新连接创建一个QTepSocket

        if(m_active_conn == MAX_CONNECTIONS){
            lsock->close();
            info.clear();
            QTextStream(&info) << "too many conn-->" << m_active_conn;
            ui->logshow->append(info);
            return;
        }

        while(m_used[m_pivot]){
            m_pivot = (m_pivot+1)% MAX_CONNECTIONS;
        }
        m_used[m_pivot]=true;
        m_working_socks[m_pivot] =lsock;
        m_active_conn++;

        info.clear();
        QTextStream(&info) << "# of conn-->" << m_pivot;
        ui->logshow->append(info);

        connect(lsock, SIGNAL(readyRead()),  m_sigmap, SLOT(map()));
        m_sigmap->setMapping(lsock, m_pivot);

        connect(m_sigmap, SIGNAL(mappedInt(int)), this, SLOT(socket_Read_Data(int)));

     }
}

void MainWindow::socket_Read_Data(int sock_idx)
{
    time_t now;
    char timebuf[100];
    int  curlen=0;
    char* file;
    char buf[40960];

    char content[1024*1024];
    QTcpSocket * lsock =  m_working_socks[sock_idx];

    lsock->read( buf, 40960 );
    char method[1000], path[1000], protocol[1000];
    memset(path,0,1000);
    sscanf( buf, "%[^ ] %[^ ] %[^ ]", method, path, protocol );
    int idx = 0;
    while(protocol[idx]!='\r'&& protocol[idx]!='\n'&& protocol[idx]!='\0')
        idx++;
    protocol[idx]='\0';
    qDebug() << "path name =" << path << "\n";


    if ( path[0] != '/' ){
        qDebug() << "Bad filename.";
        lsock->close();
        m_used[sock_idx]=false;
        m_active_conn--;
        return ;
    }
    if ( path[1] == '\0' )
        file = "index.html";
    else
        file = &(path[1]);

    QString info;
    QTextStream(&info) << "Request file-> " << file;
    ui->logshow->append(info);

    QFileInfo fi(file);
    if(!fi.exists()){
        info.clear();
        QTextStream(&info) << "no file-> " << file;
        ui->logshow->append(info);
        printf("file name =%s\n", file);
        qDebug() << QDir::currentPath();
        lsock->disconnectFromHost();
        m_used[sock_idx]=false;
        m_active_conn--;
        return ;
    }

    memset(content,0,1024*1024);
    sprintf( &content[curlen],"%s 200 Ok\r\n", protocol );

    curlen =strlen(content);
    sprintf(&content[curlen], "Server: LocalHost\r\n" );

    printf("\r\n%s\r\n", content);
    fflush(stdout);
    now = time( (time_t*) 0 );
    strftime( timebuf, sizeof(timebuf), RFC1123FMT, gmtime( &now ) );
    curlen =strlen(content);
    sprintf(&content[curlen], "Date: %s\r\n", timebuf );
    curlen =strlen(content);
    if(fi.suffix()=="htm" ||fi.suffix()=="html")
            sprintf( &content[curlen],"Content-Type: text/html; charset=utf-8\r\n");
    if(fi.suffix()=="jpg")
            sprintf( &content[curlen],"Content-Type: application/x-jpg\r\n");
    curlen =strlen(content);
    sprintf(&content[curlen], "Content-Length: %lld\r\n",fi.size() );

    curlen =strlen(content);
    sprintf( &content[curlen],"Last-Modified: %s\r\n", timebuf );
    curlen =strlen(content);
    sprintf(&content[curlen], "Connection: close\r\n" );
    curlen =strlen(content);
    sprintf( &content[curlen],"\r\n" );
    curlen =strlen(content);

    info.clear();
    QTextStream(&info) << "Response Head-> " << content<<"Head size"<<curlen;
    ui->logshow->append(info);

    QFile f(file);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open failed." << Qt::endl;
        lsock->close();
        m_used[sock_idx]=false;
        m_active_conn--;
        return ;
    }

    info.clear();
    QTextStream(&info) << "open success-> " << file;
    ui->logshow->append(info);

    f.read(&content[curlen],f.size());
    curlen+=f.size();
    f.close();

    printf("file =%s, size =%d\n",file,curlen);
    int len=100;
    if ((len=lsock->write(content, curlen)) == -1){
        lsock->close();
        m_used[sock_idx]=false;
        m_active_conn--;

        info.clear();
        QTextStream(&info) << "write fail";
        if (!lsock->isWritable()) QTextStream(&info) << "-> file is not writable";
        ui->logshow->append(info);
    }else{
        info.clear();
        QTextStream(&info) << "write success-> " << len << "wanted-> "<<curlen;
        ui->logshow->append(info);
    }

    lsock->flush();

    lsock->close();
    m_used[sock_idx]=false;
    m_active_conn--;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    connect(server, &QTcpServer::newConnection,
        this,       &MainWindow::server_New_Connect);//监听
    if(!server->listen(QHostAddress::Any, 8080)) {
        ui->logshow->append(server->errorString());
    }else{
        QString info;
        QTextStream(&info) << "HTTP Server started on Port-> " << server->serverPort();
        ui->logshow->append(info);
        ui->startButton->setEnabled(false);
    }
}

