TARGET=tcpserver
CC=gcc
#CFLAGS=
MAKE_DIR=$(PWD)
#LIB_DIR=
OBJS_DIR=obj
#INCLUDE_DIR=
#INCLUDE= -I $(LIB_DIR) -I (OBJ_DIR) -I(INCLUDE_DIR)
SUBDIRS=src 
TARGET_DIR=$(PWD)
OBJS=src/*.o	

export CC OBJS OBJS_DIR MAKE_DIR TARGET_DIR TARGET


all:CHECK_DIR $(TARGET)
$(TARGET):$(SUBDIRS)
	$(CC) -o $(TARGET) $(OBJS)
CHECK_DIR:
	@echo $(PWD)
$(SUBDIRS):ECHO 
	make -C $@
ECHO:
	@echo begin compile
clean:
	make clean -C $(SUBDIRS)
	-rm -rf $(TARGET)
