#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <linux/fb.h>
#include <sys/ioctl.h>   
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "jpeglib.h"

#include "input.h"


#define MAX(a,b)   ((a)>(b)?(a):(b))
#define MIN(a,b)   ((a)<(b)?(a):(b))

int fb=-1;//屏幕设备文件的文件描述符
struct fb_var_screeninfo fbinfo;//保存屏幕信息的结构体
int *plcd=NULL;//帧缓冲的首地址

/*
	lcd_init: lcd初始化函数
*/
void lcd_init(void)
{
	//1.打开屏幕设备文件  /dev/fb0
	fb = open("/dev/fb0", O_RDWR);
	if(fb == -1)
	{
		perror("open fb0 failed");
		return;
	}
	//2.获取屏幕信息
	ioctl(fb, FBIOGET_VSCREENINFO, &fbinfo);
	printf("LCD: %ld*%ld*%d\n", fbinfo.xres, fbinfo.yres, fbinfo.bits_per_pixel);
	//3.映射  mmap
	plcd = (void *)mmap(NULL, fbinfo.xres*fbinfo.yres*fbinfo.bits_per_pixel/8, PROT_READ|PROT_WRITE,
	MAP_SHARED,	fb, 0);
}

/*
	lcd_draw_point: 在屏幕的坐标(x0,y0)处，画一个颜色值为color的像素点
*/


// 用数组保存数据--全部保存到数组
unsigned int fubuffer[480][800];
static inline
void lcd_draw_point(int x0, int y0, int color)
{
	if(x0>=0 && x0<fbinfo.xres && y0>=0 && y0<fbinfo.yres)
	{
		fubuffer[y0][x0] = color;
	}
}

// 刷新一帧--一次性将数组全部拷贝到plcd这里
void update_frame()
{
	memcpy(plcd,fubuffer,800*480*4);
}

/*
	lcd_draw_rectangle: 在屏幕坐标(x0,y0)处，画一个宽为w，高为h，颜色为color的矩形
*/
void clear_screen(int x0, int y0, int w, int h, int color)
{
	// printf("%s %d\n",__FUNCTION__,__LINE__);
	for(int i=x0; i<x0+w; i++)
	{
		for(int j=y0; j<y0+h; j++)
		{
			lcd_draw_point(i, j, color);
		}
	}
}


/*
	lcd_draw_bmp: 在屏幕的坐标(x0,y0)处显示 bmpname指向的bmp图片(-1,-1)表示要让图片放屏幕居中位置
*/
void lcd_draw_bmp(const char *bmpname, int x0, int y0)
{
	int fd = open(bmpname, O_RDONLY);
	// printf("lcd_draw_bmp: %s\n",bmpname);
	if(fd == -1)
	{
		perror("open bmp failed\n");
		return;
	}
	/*读魔数*/
	unsigned char buf[2];
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, 2);
	if(buf[0]!=0x42 || buf[1]!=0x4D)
	{
		printf("this picture is not bmp!\n");
		close(fd);
		return;
	}
	/*读位图数据的偏移地址*/
	int offset;
	lseek(fd, 0x0A, SEEK_SET);
	read(fd, &offset, 4);
	/*读取图片的宽度和高度*/
	int width;
	lseek(fd, 0x12, SEEK_SET);
	read(fd, &width, 4);
	int height;
	lseek(fd, 0x16, SEEK_SET);
	read(fd, &height, 4);
	/*读取图片的色深*/
	short colordepth;
	lseek(fd, 0x1C, SEEK_SET);
	read(fd, &colordepth, 2);

	// printf("\nbmp: offset:%d width:%d height:%d colordepth:%d\n",offset,width,height,colordepth);

	if(x0==-1 || y0==-1) //居中
	{
		if(width<800 || height<480)
		{
			if(width<800)
				x0 = (800-width)/2;
			if(height<480)
				y0 = (480-height)/2;
		}
		else
		{
			x0 = 0;
			y0 = 0;
		}
	}	


	/*读取像素数组值，并用画点函数画出来*/
	lseek(fd, offset, SEEK_SET);
	for(int i=0;i<abs(height);i++)
	{
		for(int j=0;j<width;j++)
		{
			int color=0;
			if(colordepth == 24)
			{
				read(fd, &color, 3);
			}
			else if(colordepth == 32)
			{
				read(fd, &color, 4);
			}
			lcd_draw_point(x0+j, y0+(height>0?height-1-i:i), color);
			
		}
		lseek(fd, (4-colordepth/8*width%4)%4, SEEK_CUR);//跳过每行的无效的填充数据
	}
	update_frame();
	close(fd);
}

// 按钮图片打印
void Fixed_botton_printing()
{
    // 固定的三个按钮图片。--打印图片的坐标如果为（-1，-1）让图片打印居中
    lcd_draw_bmp("/root/five_qi/botton_photo/give_up.bmp",750,0);          //投降
    lcd_draw_bmp("/root/five_qi/botton_photo/start_again.bmp",750,120);    //重来
    lcd_draw_bmp("/root/five_qi/botton_photo/take_back.bmp",750,240);      //悔棋
    lcd_draw_bmp("/root/five_qi/botton_photo/exit.bmp",750,360);
	printf("printf botton photo done\n");           //退出
    // lcd_draw_bmp("/root/test/botton_photo/delete.bmp",0,420);
}

/*
	lcd_uninit: lcd解初始化函数
*/
void lcd_uninit(void)
{
	//5.解映射 munmap
	munmap(plcd, fbinfo.xres*fbinfo.yres*fbinfo.bits_per_pixel/8);
	//6.关闭屏幕设备文件
	close(fb);
}


