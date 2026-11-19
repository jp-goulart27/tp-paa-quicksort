CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
TARGET = quicksort_exp
SRCS = src/main.c src/quicksort.c src/gerador.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o