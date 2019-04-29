#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>

int main()
{
	int fd, ret;
	ssize_t numRead;
	size_t size, alignment = 512;
	off_t offset;
	void *buf;

	size = getpagesize();

	fd = open("./testfile", O_RDONLY | O_DIRECT);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	ret = posix_memalign(&buf, alignment, size * 8);
	if (ret < 0) {
		perror("memalign");
		exit(1);
	}

	numRead = read(fd, buf, size);
	if (numRead == -1) {
		perror("read");
		exit(1);
	}
	printf("Read %ld bytes\n", (long)numRead);

	exit(EXIT_SUCCESS);
}
