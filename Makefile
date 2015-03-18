F_CPU	= 8000000
DEVICE	= attiny85
TARGET	= tempcontrol.elf

CC	= avr-gcc

SRCS	= tempcontrol.c		\
	  ds18b20.c		\
	  dallas_one_wire.c

OBJS	= tempcontrol.o		\
	  ds18b20.o		\
	  dallas_one_wire.o

CFLAGS	= -Wall			\
	  -Os			\
	  -std=c99		\
	  -DF_CPU=$(F_CPU)	\
	  -I /usr/avr/include	\
	  -mmcu=$(DEVICE)

LDFLAGS	= -L /usr/avr/lib

clean:
	rm -f $(OBJS) $(TARGET)

all : clean $(TARGET)
	echo "make all complete"

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

