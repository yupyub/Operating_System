CC = gcc
TARGET = testlib
OBJECTS = main.o list.o bitmap.o hash.o hex_dump.o

all = $(TARGET)

$(TARGET) : $(OBJECTS) 
	$(CC) -Wall -o $@ $^

clean : 
	rm -f *.o
	rm -f $(TARGET)
