/***************************************************************************
 *   v4l2grab Version 0.2                                                  *
 *   Copyright (C) 2009 by Tobias Müller                                   *
 *   Tobias_Mueller@twam.info                                              *
 *                                                                         *
 *   based on V4L2 Specification, Appendix B: Video Capture Example        *
 *   (http://v4l2spec.bytesex.org/spec/capture-example.html)               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


// use ITU-R float conversion for YUV422toRGB888 by default
#if !defined(ITU_R_FLOAT) && !defined(ITU_R_INT) && !defined(NTSC)
#define ITU_R_FLOAT
#endif

#if ((defined(ITU_R_FLOAT)) && (defined(ITU_R_INT)) && (defined(NTSC))) || ((defined(ITU_R_FLOAT)) && (defined(ITU_R_INT))) || ((defined(ITU_R_FLOAT)) && (defined(NTSC))) ||  ((defined(ITU_R_INT)) && (defined(NTSC)))
#error Only one conversion for YUV422toRGB888 is allowed!
#endif

// compile with all three access methods
#if !defined(IO_READ) && !defined(IO_MMAP) && !defined(IO_USERPTR)
#define IO_READ
#define IO_MMAP
#define IO_USERPTR
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <jpeglib.h>
#include <iostream>
#include <iomanip>
#include "SerialComm.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))


// global settings
static unsigned int width = 160;
static unsigned int height = 120;
static unsigned char jpegQuality = 70;
static char* jpegFilename = NULL;
int input_format=0;

// serial stuff
SerialComm *comm;
int err=0;
int num_bytes=0;
unsigned char tempBuffer[20];
unsigned char imageBuffer[640*480*2];
speed_t baud_rate = B115200;
char *port_name = "/dev/ttyACM1\0";

void rotateGreyImage(int width, int height, unsigned char *src, unsigned char *dst) {
	int line, column;
	int index_src=0, index_dst=0;
	for (column = 0; column < width; ++column) {
		for (line = 0; line < height; ++line) {
			index_src = line*width+column;
			dst[index_dst++] = src[index_src];
		}
	}
}

static void RGB565toRGB888(int width, int height, unsigned char *src, unsigned char *dst)
{
	int line, column;
	int index_src=0, index_dst=0;

//	if(cameraRotatedFlag == 0) {
		for (line = 0; line < height; ++line) {
			for (column = 0; column < width; ++column) {
				index_src = line*width*2+column*2;
				dst[index_dst++] = (unsigned char)(src[index_src] & 0xF8);
				dst[index_dst++] = (unsigned char)((src[index_src]&0x07)<<5) | (unsigned char)((src[index_src+1]&0xE0)>>3);
				dst[index_dst++] = (unsigned char)((src[index_src+1]&0x1F)<<3);
			}
		}
//	} else {
//		for (column = 0; column < width; ++column) {
//			for (line = 0; line < height; ++line) {
//				index_src = line*width*2+column*2;
//				dst[index_dst++] = (unsigned char)(src[index_src] & 0xF8);
//				dst[index_dst++] = (unsigned char)((src[index_src]&0x07)<<5) | (unsigned char)((src[index_src+1]&0xE0)>>3);
//				dst[index_dst++] = (unsigned char)((src[index_src+1]&0x1F)<<3);
//			}
//		}
//	}

}


/**
	Convert from YUV422 format to RGB888. Formulae are described on http://en.wikipedia.org/wiki/YUV

	\param width width of image
	\param height height of image
	\param src source
	\param dst destination
*/
static void YUV422toRGB888(int width, int height, unsigned char *src, unsigned char *dst)
{
	int line, column;
	unsigned char *py, *pu, *pv;
	unsigned char *tmp = dst;

	/* In this format each four bytes is two pixels. Each four bytes is two Y's, a Cb and a Cr.
	   Each Y goes to one of the pixels, and the Cb and Cr belong to both pixels. */
	py = src;
	pu = src + 1;
	pv = src + 3;

	#define CLIP(x) ( (x)>=0xFF ? 0xFF : ( (x) <= 0x00 ? 0x00 : (x) ) )

	for (line = 0; line < height; ++line) {
		for (column = 0; column < width; ++column) {
#ifdef ITU_R_FLOAT
			// ITU-R float
			*tmp++ = CLIP((double)*py + 1.402*((double)*pv-128.0));
			*tmp++ = CLIP((double)*py - 0.344*((double)*pu-128.0) - 0.714*((double)*pv-128.0));
			*tmp++ = CLIP((double)*py + 1.772*((double)*pu-128.0));
#endif

#ifdef ITU_R_INT
			// ITU-R integer
			*tmp++ = CLIP( *py + (*pv-128) + ((*pv-128) >> 2) + ((*pv-128) >> 3) + ((*pv-128) >> 5) );
			*tmp++ = CLIP( *py - (((*pu-128) >> 2) + ((*pu-128) >> 4) + ((*pu-128) >> 5)) - (((*pv-128) >> 1) + ((*pv-128) >> 3) + ((*pv-128) >> 4) + ((*pv-128) >> 5)) );  // 52 58
			*tmp++ = CLIP( *py + (*pu-128) + ((*pu-128) >> 1) + ((*pu-128) >> 2) + ((*pu-128) >> 6) );
#endif

#ifdef NTSC
			// NTSC integer
			*tmp++ = CLIP( (298*(*py-16) + 409*(*pv-128) + 128) >> 8 );
			*tmp++ = CLIP( (298*(*py-16) - 100*(*pu-128) - 208*(*pv-128) + 128) >> 8 );
			*tmp++ = CLIP( (298*(*py-16) + 516*(*pu-128) + 128) >> 8 );
#endif
			// increase py every time
			py += 2;

			// increase pu,pv every second time
			if ((column & 1)==1) {
				pu += 4;
				pv += 4;
			}
		}
	}
}

/**
	Print error message and terminate programm with EXIT_FAILURE return code.

	\param s error message to print
*/
static void errno_exit(const char* s)
{
	fprintf(stderr, "%s error %d, %s\n", s, errno, strerror (errno));
	exit(EXIT_FAILURE);
}


/**
	Write image to jpeg file.

	\param img image to write
*/
static void jpegWrite(unsigned char* img)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW row_pointer[1];
	FILE *outfile = fopen( jpegFilename, "wb" );

	// try to open file for saving
	if (!outfile) {
		errno_exit("jpeg");
	}

	// create jpeg data
	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	// set image parameters
	cinfo.image_width = width;
	cinfo.image_height = height;
	switch(input_format) {
		case 0:
			cinfo.input_components = 3;
			cinfo.in_color_space = JCS_RGB;
			break;
		case 1:
			cinfo.input_components = 3;
			cinfo.in_color_space = JCS_RGB;
			break;
		case 2:
			cinfo.input_components = 1;
			cinfo.in_color_space = JCS_GRAYSCALE;
			break;
	}

	// set jpeg compression parameters to default
	jpeg_set_defaults(&cinfo);
	// and then adjust quality setting
	jpeg_set_quality(&cinfo, jpegQuality, TRUE);

	// start compress
	jpeg_start_compress(&cinfo, TRUE);

	// feed data
	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = &img[cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	// finish compression
	jpeg_finish_compress(&cinfo);

	// destroy jpeg data
	jpeg_destroy_compress(&cinfo);

	// close output file
	fclose(outfile);
}

/**
	process image read
*/
static void imageProcess(const void* p)
{
    // Beware that the images of the PO3030 camera contains a little strip on the right side with "strange" colors;
    // this strip is used for internal camera configuration (white balance?).
    // If you want to cut it off this is the right place to do it...

	unsigned char* src = (unsigned char*)p;
	unsigned char* dst;

	// convert from YUV422 to RGB888
	switch(input_format) {
		case 0: // convert from RGB565 to RGB888
			dst = (unsigned char*)malloc(width*height*3*sizeof(char));
			RGB565toRGB888(width,height,src,dst);
			break;
		case 1: // convert from YUV422 to RGB888
			dst = (unsigned char*)malloc(width*height*3*sizeof(char));
			YUV422toRGB888(width,height,src,dst);
			break;
		case 2:
			dst = (unsigned char*)malloc(width*height*sizeof(char));
			//if(cameraRotatedFlag == 1) {
			//	rotateGreyImage(width,height,src,dst);
			//} else {
				dst = src;
			//}
			break;
	}

	int temp=0;
	//if(cameraRotatedFlag == 1) {
	//	temp = width;
	//	width = height;
	//	height = temp;
	//}

	// write jpeg
	switch(input_format) {
		case 0:
			jpegWrite(dst);
			break;
		case 1:
			jpegWrite(dst);
			break;
		case 2:
			jpegWrite(dst);
			break;
	}
}


/**
	print usage information
*/
static void usage(FILE* fp, int argc, char** argv)
{
	fprintf (fp,
		"Usage: %s [options]\n\n"
		"Options:\n"
		"-d | --device name   Video device name [/dev/video0]\n"
		"-h | --help          Print this message\n"
		"-o | --output        JPEG output filename\n"
		"-q | --quality       JPEG quality (0-100)\n"
		"-m | --mmap          Use memory mapped buffers\n"
		"-r | --read          Use read() calls\n"
		"-u | --userptr       Use application allocated buffers\n"
		"-W | --width         width\n"
		"-H | --height        height\n"
		"",
		argv[0]);
	}

static const char short_options [] = "d:ho:q:mruW:H:f:";

static const struct option
long_options [] = {
	{ "device",     required_argument,      NULL,           'd' },
	{ "help",       no_argument,            NULL,           'h' },
	{ "output",     required_argument,      NULL,           'o' },
	{ "quality",    required_argument,      NULL,           'q' },
	{ "width",      required_argument,      NULL,           'W' },
	{ "height",     required_argument,      NULL,           'H' },
	{ "input_format",required_argument,      NULL,           'f' },
	{ 0, 0, 0, 0 }
};

int main(int argc, char **argv)
{
    FILE *rawData = fopen("rawData.dat", "wb");

	for (;;) {
		int index, c = 0;

		c = getopt_long(argc, argv, short_options, long_options, &index);

		if (-1 == c)
			break;

		switch (c) {
			case 0: /* getopt_long() flag */
				break;

			case 'd':
				port_name = optarg;
				break;

			case 'h':
				// print help
				usage(stdout, argc, argv);
				exit(EXIT_SUCCESS);

			case 'o':
				// set jpeg filename
				jpegFilename = optarg;
				break;

			case 'q':
				// set jpeg quality
				jpegQuality = atoi(optarg);
				break;

			case 'W':
				// set width
				width = atoi(optarg);
				break;

			case 'H':
				// set height
				height = atoi(optarg);
				break;

		    	case 'f':   //0=RGB565, 1=YUYV, 2=GREY
		        	input_format = atoi(optarg);
		        	break;
			default:
				usage(stderr, argc, argv);
				exit(EXIT_FAILURE);
		}
	}

	// check for need parameters
	if (!jpegFilename) {
		fprintf(stderr, "You have to specify JPEG output filename!\n\n");
		usage(stdout, argc, argv);
		exit(EXIT_FAILURE);
	}

	// serial communication init
    comm = new SerialComm();
	err = comm->connect(port_name, baud_rate);
	if(err == -1) {
        perror("Cannot open serial port: ");
        return 1;
    }

    //while(1) {
        //if(comm->readData((char*)tempBuffer, 3, 90000) > 0) {
        //    printf("rx: %x %x %x\r\n", tempBuffer[0], tempBuffer[1], tempBuffer[2]);
            //std::cout << "rx: " << std::hex << (int)tempBuffer[0] << std::hex << tempBuffer[1] << std::hex << tempBuffer[2] << std::endl;
        //    if(tempBuffer[0]==0xAA && tempBuffer[1]==0xBB && tempBuffer[2]==0xCC) {
         //       std::cout << "synced" << std::endl;
                if(input_format == 2) { // grey image
                    unsigned int bytesCount = comm->readData((char*)imageBuffer, width*height, 10000000);
                    std::cout << "image received" << std::endl;
                    std::cout << "image size = " << bytesCount << std::endl;
                    if(bytesCount == width*height) {
                        fwrite (imageBuffer , sizeof(char), width*height, rawData);
                        imageProcess(imageBuffer);
                        std::cout << "image saved to " << jpegFilename << std::endl;
                    } else {
                        std::cout << "image not saved" << std::endl;
                    }
                } else {
                    unsigned int bytesCount = comm->readData((char*)imageBuffer, width*height*2, 10000000);
                    std::cout << "image received" << std::endl;
                    std::cout << "image size = " << bytesCount << std::endl;
                    if(bytesCount == width*height*2) {
                        fwrite (imageBuffer , sizeof(char), width*height*2, rawData);
                        imageProcess(imageBuffer);
                        std::cout << "image saved to " << jpegFilename << std::endl;
                    } else {
                        std::cout << "image not saved" << std::endl;
                    }
                }

        //    }
        //}

    //}

    comm->disconnect();
    fclose(rawData);
	exit(EXIT_SUCCESS);

	return 0;
}
