CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11
TARGET = tes
SRC = tes.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o tmp.out
	