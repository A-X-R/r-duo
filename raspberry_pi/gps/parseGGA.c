/**
 * @file
 * @author Piotr Machocki
 *
 * @brief This code parses a GGA message from the receiver.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parseGGA.h"

/**
 * Convert cardinal direction to integer.
 *
 * \param direction Cardinal direction to convert to integer.
 *
 * \returns 1 if direction is East or North.
 * \returns -1 if direction is West or South.
 */
static int cardinalToInt(char direction)
{
	switch (direction) {
		case 'E':
			return 1;
		case 'W':
			return -1;
		case 'N':
			return 1;
		case 'S':
			return -1;
		default:
			//TODO: Add error output.
			break;
	}

	return 0;
}

/**
 * Convert latitiude from degrees/minutes to decimal degrees.
 * Input format is as follows:
 * 4807.038,N   Latitude 48 deg 07.038' N
 *
 * \param buffer Pointer to buffer containing coordinate in degree/minutes format.
 * \param direction Indicates whether coordinate is North or South.
 *
 * \return Returns latitude in decimal degrees format.
 */
static float convertLat(char *buffer, char direction)
{
	char sdeg[3];
	char smin[8];

	strncpy(sdeg, &buffer[0], 2);
	strncpy(smin, &buffer[2], 7);

	int deg;
	float min;

	sscanf(sdeg, "%d", &deg);
	sscanf(smin, "%f", &min);

#ifdef DEBUG
	printf("\n%s,%s\n", sdeg, smin);
	printf("\n%d,%f\n", deg, min);
#endif //DEBUG

	return ((deg + (min/60)) * cardinalToInt(direction));
}

/**
 * Convert longitude from degrees/minutes to decimal degrees.
 * Input format is as follows:
 * 01131.000,E  Longitude 11 deg 31.000' E
 *
 * \param buffer Pointer to buffer containing coordinate in degree/minutes format.
 * \param direction Indicates whether coordinate is West or East.
 *
 * \return Returns latitude in decimal degrees format.
 */
static float convertLon(char *buffer, char direction) 
{
	char sdeg[4];
	char smin[8];
	
	strncpy(sdeg, &buffer[0], 3);
	strncpy(smin, &buffer[3], 7);

	int deg;
	float min;
	sscanf(sdeg, "%d", &deg);
	sscanf(smin, "%f", &min);

#ifdef DEBUG
	printf("\n%s,%s\n", sdeg, smin);
	printf("\n%d,%f\n", deg, min);
#endif //DEBUG

	return ((deg + (min/60)) * cardinalToInt(direction));
}

void parseGGA(char *buffer, int strlen, GGA *info)
{
	char lat[10];
	char latNS;
	char lon[11];
	char lonEW;

	sscanf(buffer, "%f,%9s,%c,%10s,%c,%d,%d,%f,%f,M,%f,M,0000,0000*%d", 
			&info->time,
			&lat[0],
			&latNS,
			&lon[0],
			&lonEW,
			&info->fix,
			&info->numSat,
			&info->dil,
			&info->alt,
			&info->geoid,
			&info->checksum);

	//TODO: Add checksum verification.

	lat[9] = '\0';
	lon[10] = '\0';

	info->lat = convertLat(lat, latNS);
	info->lon = convertLon(lon, lonEW);

}

