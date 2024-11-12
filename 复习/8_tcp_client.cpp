#include <iostream>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>

#include <netinet/in.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
using namespace std;
class TCP_client
{
private:
    int sockfd;
    struct sockaddr_in dest_addr;

public:
    TCP_client(char *ip, short port);
    ~TCP_client();
};

TCP_client::TCP_client(char *ip, short port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /*step2：去连接服务器--用sockaddr_in结构体，定义一个以太网的地址结构，
            最后connect即可*/
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(ip);
    dest_addr.sin_family = AF_INET;
    // 为什么要强转dest_addr而不是其他----解答：函数要求是一个指针，当然要强转，dest_addr是目标服务器地址
    // connect为什么连接sockfd----解答：理解错误，应该是用这个sockfd，去连接dest_addr
    // r是链接套接字
    int r = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (r == -1)
    {
        perror("connect failed!!\n");
    }
}

TCP_client::~TCP_client()
{
    close(sockfd);
}

int main(int argc, char *argv[])
{
    TCP_client tcp_client(argv[1], atoi(argv[2]));
    while (1)
        ;
    return 0;
}
