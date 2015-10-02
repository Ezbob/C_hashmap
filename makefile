CC = gcc
CFLAGS = -Wall
DEBUGFLAGS = -g
OBJS = hashmap.o
SRCS = $(OBJS:.o=.c)
HEADERS = $(OBJS:.o=.h)
PROGRAM = 

$(OBJS):

clean:
	$(RM) $(OBJS) $(PROGRAM)
