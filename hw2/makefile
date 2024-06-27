CC = gcc
CFLAGS = -Wall -g

# Source files
FS_SRCS = filesystem.c
OPER_SRCS = file_operations.c
MAKE_FS_SRCS = main.c

FS_DAT_FILE = fs.dat
FS_DAT_BLOCK_SIZE = 0.5

# Object files
FS_OBJS = $(FS_SRCS:.c=.o)
OPER_OBJS = $(OPER_SRCS:.c=.o)
MAKE_FS_OBJS = $(MAKE_FS_SRCS:.c=.o)

# Executables
EXEC_MAKE_FS = makeFileSystem
EXEC_OPER = fileSystemOper

all: $(EXEC_MAKE_FS) $(EXEC_OPER)

$(EXEC_MAKE_FS): $(MAKE_FS_OBJS) $(FS_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(EXEC_OPER): $(OPER_OBJS) $(FS_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC_MAKE_FS) $(EXEC_OPER) *.o

.PHONY: all clean
