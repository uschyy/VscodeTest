#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
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
#include "start.h"
#include "macro.h"


int take_back_array[255][15][15]={0};
int step=0;

// 按钮路径
#define botton_path "/root/five_qi/botton_photo"

// 非按钮的路径
#define use_photo_path "/root/five_qi/use_photo"

int exit_flag=-1;

int server_move_flag=-11;

int victory_flag=0;

extern int give_up_flag;

int i_before=0;
int j_before=0;

int finish_flag=-1;

// 是否已经选择
int select_flag=no_choose;

void move_first_or_senond(int select_flag)
{
    if (select_flag==no_choose)
    {
        lcd_draw_bmp("/root/five_qi/botton_photo/first_move.bmp",480,160);
        lcd_draw_bmp("/root/five_qi/botton_photo/second_move.bmp",480,290);
        int st=process_input_move_first_or_scond();
        if (st==first_move)
        {
            server_move_flag=can_move;
        }
        else if (st==second_move)
        {
            server_move_flag=can_not_move;
        }
        
    }
    
}
// 执行五子棋游戏的功能
void game_init_with_start(char **argv)
{
    // 服务端的初始化
    int sockfd=tcp_server_init(argv[1],atoi(argv[2]));
    //棋盘初始化
    Qipan_init();
    
    
    // 定一个以太网的地址结构sockaddr_in结构体
    struct sockaddr_in src_add;
    // 把结构体清空
    memset(&src_add,0,sizeof(src_add));
    socklen_t addrlen=sizeof(src_add);//-------为什么要做这步
    // 接收客户端--accept
    // accept返回的是一个链接套接字
    // ------为什么是&src_add,src_add不是清空了吗？？
    int r=accept(sockfd,(struct sockaddr *)&src_add,&addrlen);//--这个r就是后面交流使用的接口
    // 如果返回值是不等于-1，就说明有客户端链接，就创建一个线程
    if (r!=-1)
    {
        pthread_t th1;
        pthread_create(&th1,NULL,connect_handler_read,&r);//----------------//副线程1接收客户端信息
    }
    printf("server printf screen\n");

    printf_photo_to_screen();
    while (1)//-------------------------------------------------------------//主线程发送--棋盘事件
    {
        if (select_flag==no_choose)
        {
            move_first_or_senond(select_flag);
            select_flag=had_choose;

        }
        if (exit_flag==1)
        {
            break;
        }
        if(server_move_flag==can_move)//为1时才可以动
        {
            printf("server can move--p2\n");
            lcd_draw_bmp("/root/five_qi/use_photo/you_can_move.bmp",500,0);
            int status=process_input();
            if (server_move_flag==can_not_move)
            {
                continue;
            }
            
            if (status==click_down&&give_up_flag==0)//给对面发送坐标消息，且没有点击投降
            {
                server_move_flag=can_not_move;//server不允许动
                int re=process_click_down(r);
                if (re==down_again)//防止重叠
                {
                    server_move_flag=can_move;
                    continue;
                }

                int st=checkWin(WHILE);
                if (st==victory&&finish_flag==-1)//获得胜利，且没有结束
                {
                    finish_flag=0;
                    victory_flag++;
                    if (victory_flag==1)
                    {
                        printf("-------server win-------\n");
                        char buf[64]="server WIN";
                        send(r,buf,strlen(buf)+1,0);
                        lcd_draw_bmp("/root/five_qi/use_photo/you_win.bmp",-1,-1);
                        sleep(3);
                        clear_screen(0, 0, 800, 480, 0x000000);//清屏
                         // 打印当前棋盘和按钮
                        printf_photo_to_screen();
                        server_move_flag=can_not_move;//结束了，双方都不能动
                        continue;
                    }
                }
            }
        }   

        
    }

    printf("p1\n");

    tcp_server_uninit(sockfd);
}



