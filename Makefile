TARGET=tcpserver
#OBJS=tcpserver
CC=gcc
#CFLAGS=
MAKE_DIR=$(PWD)
#LIB_DIR=
#OBJ_DIR=
#INCLUDE_DIR=
#INCLUDE= -I $(LIB_DIR) -I (OBJ_DIR) -I(INCLUDE_DIR)

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)


	
.PHONY =clean
clean:
	-rm -f $(OBJS_DIR)*.o
