CC = gcc
CFLAGS = -Wall -g -rdynamic -Werror 
LIBS = -lm -pthread

TARGET = thule
SRCDIR = src

.PHONY: default all clean

default:	$(TARGET)
all:		default

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
HEADERS = $(wildcard $(SRCDIR)/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@
	
clean:
	-rm -f $(SRCDIR)*.o
	-rm -f $(TARGET)
