/**
* @file
* @author Piotr Machocki
*
* @brief Header file for the GGA message parsing code.
*/
#ifndef PARSEGSA_H_
#define PARSEGSA_H_

#include <stdbool.h>

/*
Format of the GGA message.

GPS DOP and active satellites
eg1. $GPGSA,A,3,,,,,,16,18,,22,24,,,3.6,2.1,2.2*3C
eg2. $GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*35

1	 = Mode:
		M=Manual, forced to operate in 2D or 3D
		A=Automatic, 3D/2D
2	 = Mode:
		1=Fix not available
		2=2D
		3=3D
3-14 = IDs of SVs used in position fix (null for unused fields)
15   = PDOP
16   = HDOP
17   = VDOP
*/

#define GSA_LEN (128)

typedef struct GSA {
	bool fix;
} GSA;

/**
 * Parse the GSA message string and extract required information.
 *
 * \param buffer Pointer to the buffer containing message to be parsed.
 * \param info   Pointer to structure to be filled out.
 */
void parseGSA(char *buffer, int strlen, GSA *info);

#endif //PARSEGSA_H_

