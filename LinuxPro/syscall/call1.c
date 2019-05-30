# include <stdio.h>

int main()
{
	int res;
	res = syscall();
	printf("val returned by syscall %d\n",res);
	return 0;
}

int syscall()
{
	int res;
	__asm__("movl $338, %eax");
        __asm__("int $0x80");
        __asm__("movl %eax, -4(%ebp)");
	return res;
}
