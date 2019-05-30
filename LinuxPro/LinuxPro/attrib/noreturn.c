
void terminate(void) __attribute__((noreturn));
void terminate(void) 
{
	printf("\n Critcal Error \n");
	exit(1);
}

int main()
{
	int a=10,b=20;
	if(a<b)
		terminate();
	else
		/* do something */
}

