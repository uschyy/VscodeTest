#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <linux/input.h>

#include <sys/socket.h>
#include <linux/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <pthread.h>
#include "input.h"
#include "lcd.h"
#include "app.h"
#include "server_main.h"
#include "start.h"
// #include <linux/time.h>

/*
type:3 code:0 value:304  //触摸横坐标为 304
type:3 code:1 value:258  //触摸纵坐标为 258
type:1 code:330 value:1  //BTN_TOUCH  有触摸压力
type:0 code:0 value:0    //同步数据
type:1 code:330 value:0  //BTN_TOUCH  无触摸压力
type:0 code:0 value:0    //同步数据

*/
// -------------------------------------------------/
// --------------------------

// 结束
#define FINISH 0
extern int server_move_flag;
extern int victory_flag;

int new_x, new_y;


// 获取x，y的值--x0，y0就是第一次按下去的坐标，x1，y1就是抬起时的坐标
void get_input(int fd, int *x0, int *y0, int *x1, int *y1, int *starttime)
{
	printf("listening screen\n");
	struct input_event et;
	struct timeval time1; // 该事件发生的时间
	// clock_t start,end;
	int flag_x = 0, flag_y = 0;
	while (1)
	{

		int r = read(fd, &et, sizeof(et));
		// int t = read(fd,&time1,sizeof(time1));
		if (r == sizeof(et))
		{
			// printf("s:%d, ms:%d",time1.tv_sec,time1.tv_usec);
			if (et.type == EV_ABS && et.code == ABS_X) // 获取x的坐标
			{
				if (flag_x == 0) // 仅仅获取一次
				{
					flag_x = 1;
					*x0 = et.value * 800 / 1024; // 坐标等比例转换
				}
				*x1 = et.value * 800 / 1024; // 持续获取x的坐标
			}
			if (et.type == EV_ABS && et.code == ABS_Y) // 获取y坐标
			{
				if (flag_y == 0)
				{
					flag_y = 1;
					*y0 = et.value * 480 / 600;
				}
				*y1 = et.value * 480 / 600;
			}

			if (et.type == EV_KEY && et.code == BTN_TOUCH && et.value == 0) // 触摸动作结束--手抬起的时候
			{
				// gettimeofday(CLOCK_REALTIME,&time1);
				*starttime = time1.tv_usec;
				break;
			}
		}
	}
}

int process_input(void) // 监听屏幕棋盘事件
{
	int x0, y0, x1, y1, start, end;
	int fd = open("/dev/input/event0", O_RDONLY);
	if (fd == -1)
	{
		perror("open event0 failed");
		return FINISH;
	}
	// 异步获取时间
	int flag_time1 = 0;
	int flag_time2 = 0;
	long time1 = 0;
	long time2 = 0;
	while (1)
	{
		new_x = 0;
		new_y = 0;								   // 全局变量
		get_input(fd, &x0, &y0, &x1, &y1, &start); // 持续监听屏幕事件

		if (abs(x1 - x0) <= 50 && abs(y1 - y0) <= 50) // 第一次点击的坐标和抬起时的坐标相差不大，视为点击
		{
			if (x0 >= 0 && x0 < 480 && y0 > 0 && y0 < 480)
			{
				new_x = x0;
				new_y = y0;
				// location_i[take_back_step_server] = new_x;
				// location_j[take_back_step_server] = new_y;
				// take_back_step_server+=2;
				return click_down;
			}
		}
	}

	close(fd);
	return 0;
}

int process_input1(void) // 监听按钮
{
	int x0, y0, x1, y1, start, end;
	int fd = open("/dev/input/event0", O_RDONLY);
	if (fd == -1)
	{
		perror("open event0 failed");
		return FINISH;
	}
	// 异步获取时间
	int flag_time1 = 0;
	int flag_time2 = 0;
	long time1 = 0;
	long time2 = 0;
	while (1)
	{

		get_input(fd, &x0, &y0, &x1, &y1, &start); // 持续监听屏幕事件

		if (abs(x1 - x0) <= 50 && abs(y1 - y0) <= 50) // 第一次点击的坐标和抬起时的坐标相差不大，视为点击
		{

			printf("===========click(%d,%d)==========\n", x0, y0);
			if (x0 >= 750 && x0 < 800 && y0 >= 0 && y0 <= 60) // 投降
			{
				printf("give up\n");
				return Give_up;
			}
			else if (x0 >= 750 && x0 < 800 && y0 >= 120 && y0 <= 180) // 重来
			{
				printf("start again\n");
				return start_again;
			}
			else if (x0 >= 750 && x0 < 800 && y0 >= 240 && y0 <= 300) // 悔棋
			{
				printf("take back\n");
				return take_back;
			}
			else if (x0 >= 750 && x0 < 800 && y0 >= 360 && y0 <= 420) // 退出
			{
				printf("exit\n");
				return EXIT;
			}
		}
	}

	close(fd);
	return 0;
}

int process_input_yse_or_no(void) // 监听重来事件中的确认还是取消
{
	int x0, y0, x1, y1, start, end;
	int fd = open("/dev/input/event0", O_RDONLY);
	if (fd == -1)
	{
		perror("open event0 failed");
		return FINISH;
	}
	// 异步获取时间
	int flag_time1 = 0;
	int flag_time2 = 0;
	long time1 = 0;
	long time2 = 0;
	while (1)
	{
		get_input(fd, &x0, &y0, &x1, &y1, &start);	  // 持续监听屏幕事件
		if (abs(x1 - x0) <= 50 && abs(y1 - y0) <= 50) // 第一次点击的坐标和抬起时的坐标相差不大，视为点击
		{

			printf("===========click(%d,%d)==========\n", x0, y0);
			if (x0 >= 480 && x0 < 530 && y0 >= 280 && y0 <= 320) // 投降
			{
				printf("YES\n");
				return YES;
			}
			else if (x0 >= 580 && x0 < 630 && y0 >= 280 && y0 <= 320)
			{
				clear_screen(0, 0, 800, 480, 0x000000); // 清屏
				// 打印当前棋盘和按钮
				printf_photo_to_screen();
				return NO;
			}
			else
			{
				return 999;
			}
		}
	}
	close(fd);
	return 0;
}


int process_input_move_first_or_scond(void)//先动还是后动
{
	int x0,y0,x1,y1,start,end;
	int fd = open("/dev/input/event0", O_RDONLY);
	if(fd == -1)
	{
		perror("open event0 failed");
		return FINISH;
	}
	// 异步获取时间
	int flag_time1=0;
	int flag_time2=0;
	long time1=0;
	long time2=0;
	while(1)
	{
		get_input(fd, &x0, &y0, &x1, &y1,&start);//持续监听屏幕事件
		if(abs(x1-x0)<=50 && abs(y1-y0)<=50)//第一次点击的坐标和抬起时的坐标相差不大，视为点击
		{

			printf("===========click(%d,%d)==========\n", x0, y0);
			if(x0>=480 && x0<630 && y0>=160 && y0<=230)// 
			{
				clear_screen(0, 0, 800, 480, 0x000000);//清屏
				// 打印当前棋盘和按钮
				printf_photo_to_screen();
				return first_move;
			}
			else if (x0>=480 && x0<630 && y0>=290 && y0<=360)
			{
				clear_screen(0, 0, 800, 480, 0x000000);//清屏
				// 打印当前棋盘和按钮
				printf_photo_to_screen();
				return second_move;
			}
			else
			{
				continue;
			}
			
		}
	}

	close(fd);
	return 0;
}
extern int give_up_flag;
// 点击认输，给对面发认输消息
void send_give_up(int sockfd)
{
	char buf[32] = "give up";
	ssize_t r = send(sockfd, buf, strlen(buf) + 1, 0);
	if (r == -1)
	{
		perror("give up send faild\n");
	}
}

// 接受到对手认输
void receive_give_up()
{
	// 打印对方投降图片
	lcd_draw_bmp("/root/five_qi/use_photo/op_give_up.bmp", -1, -1);
	// 对方认输，棋盘结束，自然不能动
	server_move_flag = can_not_move;
	sleep(3);
	clear_screen(0, 0, 800, 480, 0x000000); // 清屏
	// 打印当前棋盘和按钮
	printf_photo_to_screen();
	printf("opposite side give up\n");
	new_x = 0;
	new_y = 0;
	give_up_flag = 1;
}

// 点击再来，给对面发 再来一把消息
void send_start_again(int sockfd)
{
	char buf[32] = "start again";
	ssize_t r = send(sockfd, buf, strlen(buf) + 1, 0);
	if (r == -1)
	{
		perror("start again send faild\n");
	}
}


extern int select_flag;

// 接收到对手再来一遍申请
void receive_start_again()
{
	Qipan_init();
	clear_screen(0, 0, 800, 480, 0x000000); // 清屏
	printf_photo_to_screen();
	server_move_flag = -11;
	printf("opposite side start again\n");
	new_x = 0;
	new_y = 0;
	give_up_flag = 0;
	victory_flag=0;
	select_flag=no_choose;
	if (select_flag==no_choose)
    {
        move_first_or_senond(select_flag);
        select_flag=had_choose;
    }
}

// 点击悔棋，给对面发悔棋消息
void send_take_back(int sockfd)
{
	char buf[32] = "take back";
	ssize_t r = send(sockfd, buf, strlen(buf) + 1, 0);
	if (r == -1)
	{
		perror("take back send faild\n");
	}
}

// 点击退出，给对面发退出指令
void send_exit(int sockfd)
{
	char buf[32] = "exit";
	ssize_t r = send(sockfd, buf, strlen(buf) + 1, 0);
	if (r == -1)
	{
		perror("exit send faild\n");
	}
}


// ---------------------------------------------------
extern int step;
extern int take_back_array[15][15][15];
void read_take_array(int s)
{
	step=step-s;
	if (step<=0)
	{
		Qipan_init();
		step=0;
	}
	else
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				Array[i][j]=take_back_array[step][i][j];
			}
		}
	}
}
void save_array()
{
	step++;
	printf("starting save\n");
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			// printf("step:%d,i:%d,j:%d\n",step,i,j);
			take_back_array[step][i][j]=Array[i][j];
		}
	}
	
	printf("save---array--\n");
	printf_save_Array();
}
void printf_save_Array()
{
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			// printf("step:%d,i:%d,j:%d\n",step,i,j);
			printf("%d ",take_back_array[step-1][i][j]);
		}
		printf("\n");
	}
}


// ----------------------------------------------------
extern int receive_cilent_move_flag;
// 表示下棋了--给对面发坐标，把自己点击的坐标也给到自己的棋盘
int process_click_down(int sockfd)
{
	// 点击的格子，在实际棋盘中的位置i，j
	int i = new_x / 32;
	int j = new_y / 32;
	char buf_i[15] = {0};
	char buf_j[15] = {0};

	printf("server click location :(%d , %d)", i, j);
	sprintf(buf_i, "i:%d", i);
	sprintf(buf_j, "j:%d", j);

	if (Array_flag[i][j] == unvisited)
	{
		printf("server down\n");
		ssize_t r = send(sockfd, buf_i, strlen(buf_i) + 1, 0);

		ssize_t t = send(sockfd, buf_j, strlen(buf_j) + 1, 0);

		if (r == -1)
		{
			perror("click_down send i faild\n");
		}
		if (t == -1)
		{
			perror("click_down send j faild\n");
		}
		Array_flag[i][j] = visited;
		while_move(i, j);

		usleep(300);
		printf("save before\n");
		save_array();
		printf("save after\n");
	}
	else
	{
		printf("you had down\n");
		return down_again;
	}
	clear_screen(500, 0, 96, 64, 0x000000); // 清屏
	printf_photo_to_screen();
	return 999;
}

// 点击投降的事件
void click_give_up()
{
	// 打印我输了图片
	lcd_draw_bmp("/root/five_qi/use_photo/you_lose.bmp", -1, -1);
	// 对方认输，棋盘结束，自然不能动
	server_move_flag = can_not_move;
	sleep(3);
	clear_screen(0, 0, 800, 480, 0x000000); // 清屏
	// 打印当前棋盘和按钮
	printf_photo_to_screen();
	printf("server give up\n");
	new_x = 0;
	new_y = 0;
	give_up_flag = 1;
}


extern int server_move_flag;
extern int step;
extern int take_back_array[15][15][15];
// 收到悔棋同意,打印棋盘
void receive_take_back_aggre(int take_back_step)
{
	// printf_Qipan();
	printf("----------\n");
	read_take_array(take_back_step);
	usleep(500);
	printf_Qipan();
	clear_screen(0, 0, 800, 480, 0x000000);//清屏
	printf_photo_to_screen();
	// server_move_flag=can_move;
}
