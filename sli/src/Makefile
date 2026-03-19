CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11
TARGET = sli
SRC = sli.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o tmp.out
	