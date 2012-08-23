#$(MAKE_DIR)/$(OBJS_DIR)/main.o ip.o:main.c
OBJS=main.o ip.o server.o 
all:$(OBJS)

$(filter %.o ,$(OBJS)):%.o: %.c
	$(CC) -c $(CFLAGS)  $< -o $@
clean:
	-rm -rf *.o
