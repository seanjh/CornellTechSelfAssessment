CC=gcc
CFLAGS=-g -p -Wall -Wextra
TARGET=spell_check
OBJECTS=linkedlist.o splaytree.o hashtable.o dictionary.o

$(TARGET): $(OBJECTS) $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@.a

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CGLAGS) $< -c -o $@

splaytree.o: splaytree.c splaytree.h
	$(CC) $(CGLAGS) $< -c -o $@

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CGLAGS) $< -c -o $@

dictionary.o: dictionary.c dictionary.h
	$(CC) $(CGLAGS) $< -c -o $@

clean:
	$(RM) $(TARGET).a $(OBJECTS)
