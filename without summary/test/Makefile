CC = gcc
CFLAGS = -Wall -g
TARGET = test
SRC = test.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)