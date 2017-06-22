/*
 *  SerialComm.cpp
 *  Created by Stefano Morgani on 11/18/08.
 *
 */

#include "SerialComm.h"

SerialComm::SerialComm() {

}

SerialComm::~SerialComm() {

}

int SerialComm::connect(char *path, speed_t baud_rate) {
	// open the USB stream
        fd = open(path, O_RDWR  | O_NONBLOCK | O_NDELAY);
	if(fd==-1){
		return -1;
	}

	// get the baud rate and the flags
	struct termios term;
	tcgetattr(fd, &term);

        term.c_iflag = 0;
        term.c_oflag = 0;
        term.c_cflag = CLOCAL | CREAD | CS8;
        term.c_lflag = 0;       // non-canonical reading
        term.c_cc[ VTIME ] = 0; //timeout in tenths of a second
        term.c_cc[ VMIN ] = 0;  //no wait for any char

        cfsetispeed(&term, baud_rate);
        cfsetospeed(&term, baud_rate);

        if( tcsetattr(fd, TCSAFLUSH, &term) < 0)	//termios flags setting
	{
		perror("tcsetattr: ");
	}

	return 0;
}

void SerialComm::disconnect() {
	tcflush(fd,TCIOFLUSH);
	close(fd);
}

void SerialComm::flush() {
	tcflush(fd,TCIOFLUSH);
}

int SerialComm::simple_write(char *buf, int num_bytes) {
	int bytes_written = 0;
	bytes_written = write(fd, buf, num_bytes);
	return bytes_written;
}

int SerialComm::simple_read(char *buf, int num_bytes) {
	return read(fd, buf, num_bytes);
}

int SerialComm::writeData(char *buf, int num_bytes, int usleep_time) {
	errno=0;
	int bytes_written=0;
        int timeout = usleep_time/100;                  //wait 100 useconds between every read; the loop lasts for usleep_time/100 times
	while(bytes_written<num_bytes) {
		bytes_written+=write(fd, &buf[bytes_written], num_bytes-bytes_written);
		if(bytes_written<num_bytes) {
                        usleep(100);                    //1/(115200/8) = 70 useconds per byte
			timeout--;
		}
		if(timeout==0) {
			break;
		}
	}
	return bytes_written;
}

int SerialComm::readData(char *buf, int num_bytes, uint32_t usleep_time) {
	int bytes_red=0;
        int temp = 0;
        int32_t timeout = usleep_time/100;                  //wait 100 useconds between every read; the loop lasts for usleep_time/100 times
	while(bytes_red<num_bytes) {
                temp = read(fd,&buf[bytes_red],num_bytes-bytes_red);
                if(temp >= 0) {
                    bytes_red+=temp;
                }
		if(bytes_red<num_bytes) {
                        usleep(100);                    //1/(115200/8) = 70 useconds per byte
			timeout--;
		}
		if(timeout==0) {
            //printf("timeout in receiving...\n");
			break;
		}
	}
	return bytes_red;
}
