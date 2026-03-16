CC = gcc
CFLAGS = -Wall -g
TARGET = cat
SRC = cat.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)