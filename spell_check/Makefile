CC = gcc
CFLAGS = -g -p -Wall
#VPATH = sp_ext
TARGET = spell_check
OBJECTS = linkedlist.o splaytree.o dictionary.o

$(TARGET): $(OBJECTS) $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@.bin

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CGLAGS) $< -c -o $@

splaytree.o: splaytree.c splaytree.h
	$(CC) $(CGLAGS) $< -c -o $@

dictionary.o: dictionary.c dictionary.h
	$(CC) $(CGLAGS) $< -c -o $@

clean:
	$(RM) $(TARGET).bin $(OBJECTS)
