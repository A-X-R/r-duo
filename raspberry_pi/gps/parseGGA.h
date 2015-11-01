/**
* @file
* @author Piotr Machocki
*
* @brief Header file for the GGA message parsing code.
*/
#ifndef PARSEGGA_H_
#define PARSEGGA_H_

/*
Format of the GGA message.

GGA          Global Positioning System Fix Data
123519       Fix taken at 12:35:19 UTC
4807.038,N   Latitude 48 deg 07.038' N
01131.000,E  Longitude 11 deg 31.000' E
1            Fix quality: 0 = invalid
1 = GPS fix (SPS)
2 = DGPS fix
3 = PPS fix
4 = Real Time Kinematic
5 = Float RTK
6 = estimated (dead reckoning) (2.3 feature)
7 = Manual input mode
8 = Simulation mode
08           Number of satellites being tracked
0.9          Horizontal dilution of position
545.4,M      Altitude, Meters, above mean sea level
46.9,M       Height of geoid (mean sea level) above WGS84
ellipsoid
(empty field) time in seconds since last DGPS update
(empty field) DGPS station ID number
*47          the checksum data, always begins with *
*/

#define GGA_LEN (128)

typedef struct GGA {
	float 	time;
	float 	lat;
	char  	latNS;
	float 	lon;
	char	lonEW;
	int		fix;
	int		numSat;
	float 	dil;
	float	alt;
	float	geoid;
	int		checksum;
} GGA;

/**
 * Parse the GGA message string and extract required information.
 *
 * \param buffer Pointer to the buffer containing message to be parsed.
 * \param info   Pointer to structure to be filled out.
 */
void parseGGA(char *buffer, int strlen, GGA *info);

#endif //PARSEGGA_H_

