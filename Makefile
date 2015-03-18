F_CPU	= 8000000
DEVICE	= attiny85
TARGET	= tempcontrol

SRCS	= tempcontrol.c		\
	  ds18b20.c		\
	  dallas_one_wire.c

OBJS	= tempcontrol.o		\
	  ds18b20.o		\
	  dallas_one_wire.o

FLAGS	= -Wall			\
	  -Os			\
	  -std=c99		\
	  -DF_CPU=$(F_CPU)	\
	  -mmcu=$(DEVICE)

clean:
	rm -f $(OBJS) $(TARGET)

all : clean $(TARGET)
	echo "make all complete"

$(TARGET) : $(OBJS)
	avr-gcc $(FLAGS) -o $@ $(SRCS)

