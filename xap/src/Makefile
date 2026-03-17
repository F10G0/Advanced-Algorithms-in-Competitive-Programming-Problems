CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11
TARGET = xap
SRC = xap.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o tmp.out
	