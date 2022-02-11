CC = gcc
CFLAGS = -Wall -g -rdynamic -Werror 
LIBS = -lm -pthread

TARGET = thule_test
SRCDIR = .

.PHONY:		.FORCE
.FORCE:

all:		clean $(TARGET)

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(wildcard $(SRCDIR)/*.c)) ../thule.c
HEADERS = $(wildcard $(SRCDIR)/*.h) ../thule.h

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

valgrind:
	-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt $(TARGET)

clean:
	-rm -f $(TARGET) $(SRCDIR)*.o valgrind-out.txt
