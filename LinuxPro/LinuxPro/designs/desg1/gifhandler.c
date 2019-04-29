
/* This module takes care of gif files */

#include <stdio.h>
#include <string.h>
#include "handlers.h"

int canHandle(char *filename)
{
	int len;
	char *end;
        printf(" Checking whether we can handle %s\n", filename);
	len = strlen(filename);
	end = filename + (len - 3);
	printf(" end = %s\n", end);
	if (strcmp(end, "gif") == 0)
		return TRUE;
	else
		return FALSE;
}

int drawImage(char *filename)
{
	if (canHandle(filename)) {
		printf(" How is this image %s \n", filename);
		return TRUE;
	} else
		return FALSE;
}

int savefile(char *newfilename)
{
	printf(" file saved by gif handler \n");
	return TRUE;
}
