/**
 * @file
 * 
 * @brief This code initializes the UART, reads the GPS messages and saves the
 * state in a structure for further processing. 
 *
 */

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "parseGGA.h"
#include "parseGSA.h"

#define INPUT_BUFFER_SZ (128)

struct statusStruct {
	bool hasFix;
	GGA gga;
	GSA gsa;
};

void printLLA(struct statusStruct *info)
{

	if (info->hasFix && info->gga.fix) { 
		printf("\nLat:%f\n", info->gga.lat);
		printf("Lon:%f\n", info->gga.lon);
		printf("Alt:%f\n", info->gga.alt);
	}
}

/**
 * Initialze the Rasbperry PI uart to transmit and receive 9600-8-N-1.
 *
 * \param fd File descriptor of UART port.
 * 
 * \return Returns 0 upon success, -1 if UART could not be initialized.
 */
int initUART(int *fd)
{
	*fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (*fd == -1) {
		return -1;
		err(1, "Couldn't open UART.\n");
	}	
	
	fcntl(*fd, F_SETFL, 0);

	// Get option struct
	struct termios options;
	tcgetattr(*fd, &options);

	// Set port speed
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);

	// Set options
	tcsetattr(*fd, TCSANOW, &options);

	return 0;
}

int readPacket(int fd, char *buf)
{
	size_t pos = 0;
	bool process = true;

	while (process) {
		read(fd, &buf[pos], 1);
		if (buf[pos] != '*') {
			process = true;
			pos++;
		} else {
			read(fd, &buf[pos+1], 2);
			process = false;					
			pos+=2;
		}
	}

	return pos;
}

int processBuffer(char val, int fd, struct statusStruct *status)
{
	char buf[INPUT_BUFFER_SZ];
	int numRead;

	if (val == '$') {
		read(fd, buf, 6);
		if (strstr(buf, "GPGGA,") && status->hasFix) 
		{
			numRead = readPacket(fd, buf);
			parseGGA(buf, numRead, &status->gga);
			printLLA(status);
		} 
		else if (strstr(buf, "GPGSA,")) 
		{
			numRead = readPacket(fd, buf);
			parseGSA(buf, numRead, &status->gsa);
#ifdef DEBUG
			printf("fix:%c\n",buf[2]);
#endif //DEBUG
			status->hasFix = status->gsa.fix;
		}
	}	

	return 0;
}

int main()
{
	int fd;
	initUART(&fd);

	struct statusStruct status;
	memset(&status, 0, sizeof status);

	while(1) {
		int numRead;
		char val;
		numRead = read(fd, &val, 1);

		if (numRead) {
			processBuffer(val, fd, &status);
		}
#ifdef DEBUG
		//printf("%d\n", numRead);
		if (numRead) {
			printf("%c", val);
		} else {
			printf(".");
		}
#endif

	}

	return 0;
}

