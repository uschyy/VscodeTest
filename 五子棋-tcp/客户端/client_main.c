#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "input.h"
#include "lcd.h"
#include "app.h"
#include "client_main.h"
#include "macro.h"

// // 保存动一步的坐标
// int location_i[255] ;
// int location_j[255] ;

// // 悔棋的
// int take_back_step_server ;
// int take_back_step_client ;

extern int client_move_flag;
extern int step;
extern int take_back_array[255][15][15];
extern int finish_flag;
extern int victory_flag;
// 标识是否投降
int give_up_flag = 0;
// 标识是否发送了
int recv_i_flag = 0;
int recv_j_flag = 0;

int server_i, server_j;

int start_again_flag = -1;
int take_back_flag = -1;

int receive_server_move_flag = -1;

// 客户端的初始化
int tcp_client_init(char **argv,short port) 
{

    /*step1: 创建客户端的的套接字--这个套接字用来与服务器进行通信*/
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /*step2：去连接服务器--用sockaddr_in结构体，定义一个以太网的地址结构，
            最后connect即可*/
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(argv[1]);
    dest_addr.sin_family = AF_INET;
    // 为什么要强转dest_addr而不是其他----解答：函数要求是一个指针，当然要强转，dest_addr是目标服务器地址
    // connect为什么连接sockfd----解答：理解错误，应该是用这个sockfd，去连接dest_addr
    // r是链接套接字
    int r = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (r == -1)
    {
        perror("connect failed!!\n");
    }
    return sockfd;
}

// 客户端的关闭
int close_client_init(int sockfd)
{
    close(sockfd);
}

// 字符串数字·转整型数字
int zhuan_number(char *s)
{
    int sum = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        // printf("s[i]:-->%c\n",s[i]);
        sum = sum * 10 + (s[i] - '0');
    }
    return sum;
}

// 客户端的线程函数--对套接字的操作---接收数据
void *recv_data(void *arg)
{
    pthread_t th2;
    printf("had connected server!!!\n");
    int sockfd = *((int *)arg);
    // 自己设置分离属性---客户端该做什么
    pthread_detach(pthread_self());
    pthread_create(&th2, NULL, client_listen_botton, &sockfd); // 副线程2---监听按钮事件/
    while (1)
    {

        char buf[128] = {0};
        int r = read(sockfd, buf, 128); // 把sockfd这个文件描述符中的东西写到buf--会阻塞
        printf("recv:%s\n", buf);       // 客户端读取数据

        if (strcmp(buf, "give up") == 0) // 接收到对方认输
        {
            // 对方认输--打印图片对手认输
            receive_give_up();
        }
        else if (strcmp(buf, "start again") == 0) // 接收到对方请求再来一次
        {
            // 创建一个新的线程监视屏幕，点击确认，才可以正式重来
            pthread_t th4;
            pthread_create(&th4, NULL, connect_yes_start_again, &sockfd);
            usleep(100);
            while (1)
            {
                if (start_again_flag == YES) // 如果同意重来，给对面发同意，自己也要重开
                {
                    receive_start_again(); // 重启自身
                    step = 0;
                    break;
                }
                else if (start_again_flag == NO)
                {
                    break;
                }

                else//没点到
                    continue;
            }
            start_again_flag = -1; // 让重来按钮可以再一次重来
            finish_flag = -1;      // 表示棋盘没有出现胜负，
        }
        else if (strcmp(buf, "yes start again") == 0) // 接收到对方同意重开
        {
            receive_start_again(); // 重启自身
            finish_flag = -1;
            step = 0;
            // printf("opposite side start again\n");
        }
        // 接收到对方请求悔棋
        // 1.白方请求悔棋--->黑方能动/黑方不能动
        //              --->黑方能动    --->白下，白悔
        //              --->黑方不能动  --->黑下，白悔
        else if (strcmp(buf, "take back") == 0)
        {
            pthread_t th4;
            pthread_create(&th4, NULL, connect_agree_take_back, &sockfd);//创建线程--判断是否同意
            usleep(100);
            while (1)
            {
                if (take_back_flag == YES) // 同意悔棋
                {
                    if (client_move_flag == can_move) // 黑方能动   -->白下，白悔
                    {
                        receive_take_back_aggre(1);
                        client_move_flag = can_not_move;
                    }
                    else if (client_move_flag == can_not_move) // 黑方不能动   -->黑下，白悔
                    {
                        receive_take_back_aggre(2);
                        client_move_flag = can_not_move;
                    }
                    // 重启自身---加数字
                    break;
                }
                else if (take_back_flag == NO)
                {
                    break;
                }

                else
                    continue;
            }
            take_back_flag = -1;
        }

        //  接收--黑方点击了悔棋-->黑方能动/黑方不能动
        //              --->黑方能动        -->白下，黑悔   -2
        //              --->黑方不能动      -->黑下，黑悔。 -1
        else if (strcmp(buf, "take back agree") == 0) // 接收到同意悔棋
        {
            if (client_move_flag == can_move) // 黑方能动        -->白下，黑悔   -2
            {
                receive_take_back_aggre(2);
                client_move_flag = can_move;
            }
            else if (client_move_flag == can_not_move) // 黑方不能动   -->黑下，黑悔   -1
            {
                receive_take_back_aggre(1);
                client_move_flag = can_move;
            }
            take_back_flag = -1;
        }
        else if (strcmp(buf, "take back unagree") == 0) // 接收到不同意悔棋
        {

        }
        else if (strcmp(buf, "exit") == 0) // 接收到对方已经退出
        {
            // break;
            printf("opposite side exit\n");
        }

        else if (strcmp(buf, "server WIN") == 0) // 接收到对方胜利
        {
            lcd_draw_bmp("/root/five_qi/use_photo/you_lose.bmp", -1, -1);
            sleep(3);
            clear_screen(0, 0, 800, 480, 0x000000); // 清屏
            // 打印当前棋盘和按钮
            printf_photo_to_screen();
            client_move_flag = can_not_move;
            printf("opposite side WIN\n");
        }

        // 坐标获取
        else if ((strncmp(buf, "i", 1) == 0) && recv_i_flag == unvisited)
        {
            recv_i_flag = 1;
            server_i = zhuan_number((buf + 2));
        }
        else if ((strncmp(buf, "j", 1) == 0) && recv_j_flag == unvisited)
        {
            recv_j_flag = 1;
            server_j = zhuan_number((buf + 2));
        }
        if (recv_i_flag == visited && recv_j_flag == visited) // 接收到
        {
            recv_i_flag = unvisited;
            recv_j_flag = unvisited;
            printf("i: %d j: %d\n", server_i, server_j);
            while_move(server_i, server_j);
            save_array(take_back_array, Array);
            receive_server_move_flag = can_not_move; // 接受到坐标，对不能动

            printf_photo_to_screen();
            if (give_up_flag == 0)
            {
                client_move_flag = can_move; // 接收到坐标消息就允许动
                lcd_draw_bmp("/root/five_qi/use_photo/you_can_move.bmp",500,0);
                printf("client can move\n");
            }
        }
        int st = checkWin(BLACK);
        if (st == victory)
        {
            char buf[64] = "client WIN";
            send(sockfd, buf, strlen(buf) + 1, 0);
        }

        if (r == 0) // r为0表示断开连接
        {
            printf("connect break!\n");
            close(sockfd);
            return NULL;
        }
    }
}

int give_up_botton_click_flag = botton_init;
int start_again_botton_flag = botton_init;
int take_back_botton_click = botton_init;

void *client_listen_botton(void *arg)
{
    // printf("%s %d\n",__FUNCTION__,__LINE__);
    int sockfd = *((int *)arg);
    // printf("%s %d\n",__FUNCTION__,__LINE__);
    while (1)
    {
        int status = process_input1();

        if (status == Give_up )
        {
            // give_up_botton_click_flag = botton_click;
            send_give_up(sockfd); // 发消息
            click_give_up();
            // give_up_botton_click_flag == botton_init;
        }
        else if (status == start_again )
        {
            // start_again_botton_flag = botton_click;
            send_start_again(sockfd);
            // start_again_botton_flag == botton_init;
        }
        else if (status == take_back )
        {
            // take_back_botton_click = botton_click;
            send_take_back(sockfd);
            printf("client send take back\n");

            // take_back_botton_click == botton_init;
        }
        else if (status == EXIT)
        {
            send_exit(sockfd);
        }
    }
}

void *connect_yes_start_again(void *arg)
{
    int r = *((int *)arg); //------------
    pthread_detach(pthread_self());
    lcd_draw_bmp("/root/five_qi/botton_photo/require_start_again.bmp", 480, 160);
    while (1)
    {

        int status = process_input_yse_or_no();

        if (status == YES)
        {
            start_again_flag = YES;
            char buf[16] = "yes start again";
            send(r, buf, strlen(buf) + 1, 0);
            break;
        }
        else if (status == NO)
        {
            break;
        }
        else
        {
            continue;
        }
    }
}
extern int take_back_flag;
// 判断是同意悔棋，给对面是否同意
void *connect_agree_take_back(void *arg)
{
    int r = *((int *)arg); //------------
    pthread_detach(pthread_self());
    lcd_draw_bmp("/root/five_qi/botton_photo/require_take_back.bmp", 480, 160);
    while (1)
    {
        int status = process_input_yse_or_no();

        if (status == YES)
        {
            take_back_flag = YES;
            char buf[32] = "take back agree";
            send(r, buf, strlen(buf) + 1, 0);
            break;
        }
        else if (status == NO)
        {
            take_back_flag = NO;
            char buf[32] = "take back unagree";
            send(r, buf, strlen(buf) + 1, 0);
            break;
        }
        else // 没点到
        {
            continue;
        }
    }
    printf("------------->pthread th4 is finish<-----------------------\n");
}
