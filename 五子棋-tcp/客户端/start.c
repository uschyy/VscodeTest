#include <stdio.h>
#include <string.h>
#include "input.h"
#include "lcd.h"
#include "app.h"
#include "client_main.h"
#include "macro.h"
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
// lcd_draw_bmp("/root/five_qi/use_photo/you_can_move.bmp",500,0);
// clear_screen(0, 0, 800, 480, 0x000000);//清屏
// // 打印当前棋盘和按钮
// printf_photo_to_screen();

// 悔棋的棋盘--用于保存棋盘
int take_back_array[255][15][15]={0};
int step=0;

// 标志是否点击了退出
int exit_flag=-1;

// 标志客户端能否移动
int client_move_flag=-11;

// 标志是否已经有投降事件发生
extern int give_up_flag;

// 标志棋盘是否已经出现胜负
int victory_flag=0;

// 标志棋盘是否已经结束
int finish_flag=-1;

// 是否已经选择
int select_flag=no_choose;


// #define first_move "/root/five_qi/botton_photo/first_move.bmp"

void move_first_or_senond(int select_flag)
{
    if (select_flag==no_choose)
    {
        lcd_draw_bmp("/root/five_qi/botton_photo/first_move.bmp",480,160);
        lcd_draw_bmp("/root/five_qi/botton_photo/second_move.bmp",480,290);
        int st=process_input_move_first_or_scond();
        if (st==first_move)
        {
            client_move_flag=can_move;
        }
        else if (st == second_move)
        {
            client_move_flag=can_not_move;
        }
        
    }
    
}
// 执行五子棋游戏的功能
void game_init_with_start(char **argv)
{
    pthread_t tid ,tid2;
    //棋盘初始化
    Qipan_init();
    // char * ip=argv[1];
    short port=atoi(argv[2]); 
    int sockfd=tcp_client_init(argv,port);
    printf("client init success\n");
    pthread_create(&tid,NULL,recv_data,&sockfd);//副线程1----接收对面的消息
    printf_photo_to_screen();
    while (1)//主线程发送---棋盘事件
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
        if (client_move_flag==can_move)        
        {
            printf_photo_to_screen();
            lcd_draw_bmp("/root/five_qi/use_photo/you_can_move.bmp",500,0);
            int status=process_input();
            if (client_move_flag==can_not_move)
            {
                continue;
            }
            
            if (status==click_down&&give_up_flag==0)//给对面发送坐标消息，且没有点击投降
            {
                client_move_flag=can_not_move;//client不允许动
                printf("client can not move\n");
                int re=process_click_down(&sockfd);
                if (re==down_again)//防止重叠
                {
                    client_move_flag=can_move;
                    continue;
                }
                int st=checkWin(BLACK);
                if (st==victory&&finish_flag==-1)//获得胜利，且没有结束
                {
                    finish_flag=0;
                    victory_flag++;
                    if (victory_flag==1)
                    {
                        char buf[64]="client WIN";
                        send(sockfd,buf,strlen(buf)+1,0);
                        lcd_draw_bmp("/root/five_qi/use_photo/you_win.bmp",-1,-1);
                        sleep(3);
                        clear_screen(0, 0, 800, 480, 0x000000);//清屏
                        // 打印当前棋盘和按钮
                        printf_photo_to_screen();
                        client_move_flag=can_not_move;//结束了，双方都不能动
                        continue;
                    }
                }
            }
        }
    }
}

