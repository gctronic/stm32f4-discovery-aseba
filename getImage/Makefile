TARGET = getImage
CC = g++
CFLAGS = -Wall
LFLAGS = -ljpeg
SRC = SerialComm.cpp getImage.cpp
OBJS =

all: $(OBJS)
		$(CC) $(SRC) $(CFLAGS) -o $(TARGET) $(LFLAGS) $(OBJS)

clean:
		rm -f $(TARGET)
		rm *.o

