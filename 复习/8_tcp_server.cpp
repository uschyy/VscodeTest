#include <stdio.h>
#include <iostream>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>

#include <netinet/in.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

#include <pthread.h>
using namespace std;
class TCP_server
{
private:
    int _sockfd; // 套接字
    struct sockaddr_in _myaddr;

public:
    int GetSocked();
    void *connect_handler_read(void *arg);
    TCP_server(char *ip, short port);
    ~TCP_server();
};
// 获取套接字
int TCP_server::GetSocked()
{
    return _sockfd;
}
void *TCP_server::connect_handler_read(void *arg)
{
    pthread_t th12;
    // 获取客户端的文件描述符
    int sockfd = *((int *)arg);
    // 自己设置分离属性---客户端该做什么
    pthread_detach(pthread_self());
    pthread_create(&th12, NULL, NULL, arg);
    //----------------副线程2监听按钮事件
    while (1)
    {
        char buf[128] = {0};
        int r = read(sockfd, buf, 128); // 把sockfd这个文件描述符中的东西写到buf--会阻塞
        printf("recv:%s\n", buf);       // 客户端读取数据
    }
}
// TCP_server初始化
TCP_server::TCP_server(char *ip, short port)
{
    // 创建服务端sockfd的套接字
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // bind绑定ip和端口号，并用sockaddr_in结构体
    memset(&_myaddr, 0, sizeof(_myaddr));
    _myaddr.sin_family = AF_INET;
    _myaddr.sin_port = htons(port);
    _myaddr.sin_addr.s_addr = inet_addr(ip);

    int r = bind(_sockfd, (struct sockaddr *)&_myaddr, sizeof(_myaddr));
    if (r == -1)
    {
        perror("bind falied\n");
    }
    /*step3：listen监听外边客户端*/
    int re = listen(_sockfd, 64); // 最大监听数为64
    if (re == -1)
    {
        perror("listen failed\n");
    }
}

TCP_server::~TCP_server()
{
    cout << "close TCP_server!!" << endl;
    close(_sockfd);
}

int main(int argc, char *argv[])
{
    printf("%d %s", __LINE__, __FUNCTION__);
    cout << endl;
    TCP_server tcp_server(argv[1], atoi(argv[2]));
    cout << argv[1] << endl;
    cout << argv[2] << endl;
    while (1)
        ;
    return 0;
}
