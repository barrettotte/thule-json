CC = gcc
CFLAGS = -Wall -g -rdynamic -Werror 
LIBS = -lm -pthread

GDB = gdb
GDB_BRKPT = -ex 'b main'

SRCDIR = .
TARGET = thule_test.out

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
HEADERS = $(wildcard $(SRCDIR)/*.h)

.PHONY:		.FORCE
.FORCE:

all:		clean $(TARGET)

%.o: 		%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: 	$(TARGET) $(OBJECTS)

$(TARGET): 	$(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

valgrind:	clean $(TARGET)
	-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(TARGET)
	-cat valgrind-out.txt | grep 'ERROR SUMMARY'

clean:
	-rm -f $(TARGET) $(SRCDIR)*.o valgrind-out.txt

debug:		clean $(TARGET)
		$(GDB) -ex 'file $(TARGET)' -ex 'layout src' -ex 'list' $(GDB_BRKPT)
