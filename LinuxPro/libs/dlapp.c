/* test application to show how to load shared objects as runtime libs */
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int main()
{
	void *ptr, *ptr1;
	void (*fptr) (void);

	printf("%s:process registered with pid %d\n",__func__, getpid());


	ptr1 = dlopen("./libdep.so", RTLD_NOW | RTLD_GLOBAL);
	if (!ptr1) {
		printf("%s\n", dlerror());
		goto error;
	}

	ptr = dlopen("./libdrv.so", RTLD_LAZY);
	if (!ptr) {
		printf("%s\n", dlerror());
		goto error;
	}

	fptr = dlsym(ptr, "x");
	if (!fptr) {
		printf("%s\n", dlerror());
		goto error;
	}
	(fptr) ();
	getchar();

	
	fptr = dlsym(ptr, "y");
	if (!fptr) {
		printf("%s\n", dlerror());
		goto error;
	}

	(fptr) ();
	getchar();

	/* detach lib from process address space */
	dlclose(ptr);
	getchar();
	printf("%s:lib detached\n",__func__);
	return 0;

 error:
	exit(1);

}
