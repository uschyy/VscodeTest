#makefile通用模板 
TARGET := client_game
CC := arm-linux-gcc
HEADPATH := /home/china/arm_libs/include
CSRCPATH := ./
#CSRCPATH1 := ../src
LIBPATH := /home/china/arm_libs/lib
LIBNAME := jpeg


#step1: 获取所有的.c文件
CSRCS := $(wildcard $(CSRCPATH)*.c)
#CSRCS += $(wildcard $(CSRCPATH1)*.c)

#step3: 把所有的.o文件链接成一个可执行文件
OBJS := $(patsubst %.c, %.o, $(CSRCS))
$(TARGET) : $(OBJS)
#	$(CC) $+ -o $@
	$(CC) $+ -o $@ -L$(LIBPATH) -l$(LIBNAME) -pthread
	cp  $@  /mnt/hgfs/学习过程
	
#step2: 把.c文件编译成.o文件 
%.o : %.c 
	$(CC) -c $< -o $@ -I$(HEADPATH)
	

#step4: 用伪目标文件实现还原编译  
clean :
	rm -rf $(OBJS) $(TARGET)
	


