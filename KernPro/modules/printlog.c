#include <sys/klog.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	int len, level;
	char *buf;
	int last = '\n';
	int in = 0;

	len = klogctl(10, NULL, 0);	/* read ring buffer size */
	printf("the log starts with \n\n %d \n\n",len);
	if (len < 16 * 1024)
		len = 16 * 1024;
	if (len > 16 * 1024 * 1024)
		len = 16 * 1024 * 1024;

	buf = malloc(len);
	if (!buf) {
		printf("\nCannot allocate memory\n");
		return 0;
	}

	len = klogctl(3, buf, len);	/* read ring buffer */
	if (len < 0) {
		printf("\nCannot Read the buffer\n");
		free(buf);
		return 0;
	}
	if (len == 0)
		return 0;



	/* Skip <#> at the start of lines */
	while (1) {
		if (last == '\n' && buf[in] == '<') {
			in += 3;
			if (in >= len)
				break;
		}
		last = buf[in];
		putchar(last);
		in++;
		if (in >= len)
			break;
	}
	/* Make sure we end with a newline */
	if (last != '\n')
		putchar('\n');

	free(buf);

	return 0;
}
