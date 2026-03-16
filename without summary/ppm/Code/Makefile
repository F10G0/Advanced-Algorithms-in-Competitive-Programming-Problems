CC = gcc
CFLAGS = -Wall -g
TARGET = ppm
SRC = ppm.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)