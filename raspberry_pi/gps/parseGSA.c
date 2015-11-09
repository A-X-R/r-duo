/**
 * @file
 * @author Piotr Machocki
 *
 * @brief This code parses a GSA message from the receiver.
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parseGSA.h"

void parseGSA(char *buffer, int strlen, GSA *info)
{
	if (buffer[2] == '1') {
		info->fix = false;
	} else if (buffer[2] == '3') {
		info->fix = true;
	}

	//TODO: Add checksum verification.
}

