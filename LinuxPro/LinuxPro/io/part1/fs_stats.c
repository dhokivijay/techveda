#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/statvfs.h>

int main(int argc, char *argv[])
{
	struct statvfs sb;

	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "%s path\n", argv[0]);
		exit(1);
	}

	if (statvfs(argv[1], &sb) == -1) {
		perror("statvfs");
		exit(1);
	}

	printf("Block size                       %lu\n", sb.f_bsize);
	printf("Fundamental block size           %lu\n", sb.f_frsize);
	printf("Total blocks (in above units)    %lu\n",
	       (unsigned long)sb.f_blocks);
	printf("Free blocks for priv. proc.      %lu\n",
	       (unsigned long)sb.f_bfree);
	printf("Free blocks for unpriv. proc.    %lu\n",
	       (unsigned long)sb.f_bavail);
	printf("Total number of i-nodes          %lu\n",
	       (unsigned long)sb.f_files);
	printf("Free i-nodes for priv. proc.     %lu\n",
	       (unsigned long)sb.f_ffree);
	printf("Free i-nodes for nonpriv. proc.  %lu\n",
	       (unsigned long)sb.f_favail);
	printf("File system ID                   %#lx\n", sb.f_fsid);
	printf("Flags                            %#lx\n", sb.f_flag);
	printf("Maximum filename length          %lu\n", sb.f_namemax);

	exit(EXIT_SUCCESS);
}
