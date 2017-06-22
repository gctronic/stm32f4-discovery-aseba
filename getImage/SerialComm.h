/*
 *  SerialComm.h
 *  Created by Stefano Morgani on 11/18/08.
 *
 */

#ifndef SERIALCOMM_H_
#define SERIALCOMM_H_

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

class SerialComm {

	public:
		SerialComm();
		~SerialComm();
		int connect(char *path, speed_t baud_rate);
		void disconnect();
		int simple_write(char *buf, int num_bytes);
		int simple_read(char *buf, int num_bytes);
		int writeData(char *buf, int num_bytes, int usleep_time);		//buf contains the command to send, num_bytes specifies the length of the buffer, usleep_time (useoncds) specifies the total waiting time before exit writing
		int readData(char *buf, int num_bytes, uint32_t usleep_time);		//buf will contain the data received, num_bytes specifies the number of bytes to read, usleep_time (useoncds) specifies the total waiting time before exit reading
		void flush();
	private:
		int fd;
};

#endif
