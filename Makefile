CC 			?= gcc
CFLAGS 		= -Wall -Wextra -Wpedantic -O3

BIN_NAME 	= $(shell basename $(shell pwd))
VPATH 		= src:
C_SRC_FILES = $(shell ls -I "*.h" src)
OBJECTS  	= $(C_SRC_FILES:.c=.o)
LD_FLAGS 	= -lncurses

debug: CFLAGS += -g
debug: $(BIN_NAME)

$(BIN_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LD_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(LD_FLAGS)

clean:
	rm -f $(BIN_NAME) $(OBJECTS)
