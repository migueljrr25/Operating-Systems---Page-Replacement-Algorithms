CC = gcc -std=c99
CFLAGS = -Wall
LDFLAGS =
OBJFILES = main.o options.o page.o
TARGET = memsim

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~