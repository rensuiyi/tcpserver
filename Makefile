TARGET=tcpserver
OBJS=main.o	
CC=gcc
#CFLAGS=
MAKE_DIR=$(PWD)
#LIB_DIR=
OBJS_DIR=obj
#INCLUDE_DIR=
#INCLUDE= -I $(LIB_DIR) -I (OBJ_DIR) -I(INCLUDE_DIR)
SUBDIRS=src obj
TARGET_DIR=$(PWD)

export CC OBJS OBJS_DIR MAKE_DIR TARGET_DIR TARGET

all:CHECK_DIR $(SUBDIRS)
CHECK_DIR:
	@echo $(PWD)
$(SUBDIRS):ECHO
	make -C $@
ECHO:
	@echo begin compile
clean:
	-rm -f $(OBJS_DIR)*.o
