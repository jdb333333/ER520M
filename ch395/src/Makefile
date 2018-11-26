###############################################################
#设置编译器
#MTOOL= 
MTOOL=arm-linux-
CC=$(MTOOL)gcc
CFLAGS= -O0 -I$(INC) -L$(LIB) -static -g
STRIP = $(MTOOL)strip

LIB = 
INC := 


###############################################################
#当前工程的路径
ROOT_DIR=$(shell pwd)

#最终bin文件的名字
BIN=net

#当前目录下的所有.c文件
CUR_SOURCE=${wildcard *.c}

#当前目录下的所有.o文件
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}


###############################################################
#顺序执行all后的命令，最后才能是 DEBUG
all:clean $(CUR_OBJS) DEBUG

#将c文件编译为o文件
$(CUR_OBJS):%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@
#编译成可执行文件
DEBUG:
	$(CC) $(CFLAGS) *.o -o $(BIN) -lm  -lpthread
#	$(STRIP) $(BIN) -o $(BIN)-s

	cp $(BIN) /hjNFS

###############################################################
#清除命令
RM := rm -rf
clean:
	$(RM) *.o
	$(RM) *~


