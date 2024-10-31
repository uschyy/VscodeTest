#include <stdio.h>

#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <linux/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "input.h"
#include "lcd.h"
#include "app.h"
#include "server_main.h"
#include "macro.h"
// // 保存动一步的坐标
// int location_i[255];
// int location_j[255];

extern int server_move_flag;
extern int step;
extern int take_back_array[255][15][15];
extern int finish_flag;
extern int victory_flag;
// 悔棋的
int take_back_step_server = 1;
int take_back_step_client = 0;

int flag = 1;         // 允许服务端Dong
int give_up_flag = 0; // 是否已经头像

int receive_cilent_move_flag = -1; // 表示对方能不能动

int tcp_server_init(char *ip, short port)
{
    /*step1：创建服务端的sockfd套接字*/
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /*step2：bind绑定ip和端口号--记得用sockaddr_in结构体*/
    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);
    myaddr.sin_addr.s_addr = inet_addr(ip);
    // -------?为什么不绑定 myaddr.sin_addr.s_addr,而是myaddr
    int r = bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    if (r == -1)
    {
        perror("bind falied\n");
        return -1;
    }
    /*step3：listen监听外边客户端*/
    int re = listen(sockfd, 64); // 最大监听数为64
    if (re == -1)
    {
        perror("listen failed\n");
        return -1;
    }
    return sockfd;
}

int tcp_server_uninit(int sockfd)
{
    close(sockfd);
}

int zhuan_number(char *s)
{
    int sum = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        printf("s[i]:-->%c\n", s[i]);
        sum = sum * 10 + (s[i] - '0');
    }
    return sum;
}

// 标识是否接收了
int recv_i_flag = 0;
int recv_j_flag = 0;
// 对方传回来的坐标i，j
int server_i, server_j;

int start_again_flag = -1;

int take_back_flag = -1;

void *connect_handler_read(void *arg) // 接收消息
{
    pthread_t th12;

    // 获取客户端的文件描述符
    int sockfd = *((int *)arg);
    // 自己设置分离属性---客户端该做什么
    pthread_detach(pthread_self());
    pthread_create(&th12, NULL, connect_listen_botton, arg); //----------------副线程2监听按钮事件

    while (1)
    {
        char buf[128] = {0};
        int r = read(sockfd, buf, 128);  // 把sockfd这个文件描述符中的东西写到buf--会阻塞
        printf("recv:%s\n", buf);        // 客户端读取数据

        if (strcmp(buf, "give up") == 0) // 接收到对方认输
        {
            // 对方认输--打印图片对手认输
            receive_give_up();
        }

        else if (strcmp(buf, "start again") == 0) // 接收到对方请求再来一次
        {
            // 创建一个新的线程监视屏幕，点击确认，才可以正式重来
            pthread_t th3;
            pthread_create(&th3, NULL, connect_yes_start_again, arg);
            usleep(100);
            while (1)
            {
                if (start_again_flag == YES)//如果同意重来，给对面发同意，自己也要重开
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
            start_again_flag = -1;//让重来按钮可以再一次重来
            finish_flag = -1;//表示棋盘没有出现胜负，
        }
        else if (strcmp(buf, "yes start again") == 0) // 接收到对方同意重开
        {
            receive_start_again(); // 重启自身
            finish_flag = -1;
            step = 0;
            // printf("opposite side start again\n");
        }
        // 接收到对方请求悔棋
        // 1.黑方请求悔棋-->白方能动/白方不能动
        //              --->白方能动    -->黑下，黑悔   -1
        //              --->白方不能动  -->白下,黑悔    -2
        else if (strcmp(buf, "take back") == 0) 
        {
            // lcd_draw_bmp("/root/five_qi/use_photo/require_take_back.bmp",)
            pthread_t th4;
            pthread_create(&th4, NULL, connect_agree_take_back, arg);
            usleep(100);
            while (1)
            {
                if (take_back_flag == YES)//同意悔棋
                {
                    if (server_move_flag==can_move)//白方能动-->黑下，黑悔
                    {
                        receive_take_back_aggre(1); // 回到之前的棋盘
                        server_move_flag=can_not_move;
                    }
                    else if (server_move_flag==can_not_move)//白方不能动-->白下,黑悔
                    {
                        receive_take_back_aggre(2);
                        server_move_flag=can_not_move;
                    }
                    
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
        //  接收--白方点击了悔棋-->白方能动/白方不能动
        //              --->白方能动        -->黑下，白悔   -2
        //              --->白方不能动      -->白下，白悔   -1
        else if (strcmp(buf, "take back agree") == 0)
        {
            if (server_move_flag==can_move) // --->白方能动 ->黑下，白悔  -2
            {
                receive_take_back_aggre(2);
                server_move_flag=can_move;
            }
            else if(server_move_flag==can_not_move)//--->白方不能动  -->白下，白悔 -1
            {
                receive_take_back_aggre(1);
                server_move_flag=can_move;
            }
            take_back_flag = -1;
        }

        else if (strcmp(buf, "take back unagree") == 0) // 接收到不同意悔棋
        {

        }
        else if (strcmp(buf, "exit") == 0) // 接收到对方已经退出
        {
            // break;
        }

        else if (strcmp(buf, "client WIN") == 0) // 接收到对方胜利
        {
            lcd_draw_bmp("/root/five_qi/use_photo/you_lose.bmp", -1, -1);
            sleep(3);
            clear_screen(0, 0, 800, 480, 0x000000); // 清屏
            // 打印当前棋盘和按钮
            printf_photo_to_screen();
            server_move_flag = can_not_move;
            printf("opposite side WIN\n");
        }

        //  得到客户端发送来的坐标
        else if (strncmp(buf, "i", 1) == 0 && recv_i_flag == 0)
        {
            recv_i_flag = 1;
            server_i = zhuan_number((buf + 2));
        }
        else if (strncmp(buf, "j", 1) == 0 && recv_j_flag == 0)
        {
            recv_j_flag = 1;
            server_j = zhuan_number((buf + 2));
        }
        if (recv_i_flag == 1 && recv_j_flag == 1)
        {
            recv_i_flag = 0;
            recv_j_flag = 0;
            printf("i: %d j: %d\n", server_i, server_j);
            black_move(server_i, server_j);
            save_array(take_back_array, Array);
            receive_cilent_move_flag = can_not_move; // 接收到坐标，对方不能动

            printf_photo_to_screen();
            if (give_up_flag == 0) // 不在投降状态下
            {
                server_move_flag = can_move;
                lcd_draw_bmp("/root/five_qi/use_photo/you_can_move.bmp",500,0);
            }
        }

        int st = checkWin(WHILE);
        if (st == victory)
        {
            char buf[64] = "server WIN";
            send(sockfd, buf, strlen(buf) + 1, 0);
            printf("server pthread_check_whilel win\n");
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

// 监听按钮事件
void *connect_listen_botton(void *arg)
{
    int r = *((int *)arg); //------------
    pthread_detach(pthread_self());
    while (1)
    {
        printf(" ---> xiancheng2 is listening screen <--  !!!\n");
        int status = process_input1();

        if (status == Give_up )
        {
            send_give_up(r); // 给对面发投降
            click_give_up(); // 执行投降操作
        }
        else if (status == start_again )
        {
            send_start_again(r); // 给对面发重开的消息

        }
        else if (status == take_back )
        {
            send_take_back(r); // 给对面发悔棋消息
            printf("server send takee back\n");
        }
        else if (status == EXIT)
        {
            send_exit(r);
        }
    }
}

// 判断是否真的要重开
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
            start_again_flag = NO;
            break;
        }
        else // 没点到
        {
            continue;
        }
    }
    // clear_screen(480,160,150,160, 0x000000);//清屏
    printf("xiancheng YES OR NO ----------kill self\n");
}
extern int take_back_flag;
// 判断是同意悔棋，给对方发是否同意
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
}
