#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if (argc != 3 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "%s old-file new-file\n", argv[0]);
		exit(1);
	}

	/* Open input and output files */

	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1) {
		fprintf(stderr, "error opening source file");
		exit(1);
	}

	openFlags = O_CREAT | O_WRONLY | O_SYNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;	/* rw-rw-rw- */

	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1) {
		fprintf(stderr, "error opening source file");
		exit(1);
	}

	/* Transfer data until we encounter end of input or an error */

	while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
		if (write(outputFd, buf, numRead) != numRead) {
			perror("write ");
			exit(1);
		}
	if (numRead == -1) {
		perror("read: ");
		exit(1);
	}

	close(inputFd);
	close(outputFd);

	exit(EXIT_SUCCESS);
}
