# include <stdio.h>
# include <dlfcn.h>

int main(){
	char *ptr;
	void *handle;
	int i;

	/* step 1: declare pointer that can hold library symbols we intend to access */
	int (*fnptr)(int,int);
	getchar();

	/* step 2: request link-loader to load required library */
	handle = dlopen("libxyz.so",RTLD_NOW);
	if(handle == NULL){
		printf(" Failed to load library \n");
		exit(2);
	}
	/* step 3: request link-loader to return address of the symbol 
                   which is to be accessed */ 
	fnptr = dlsym(handle,"somefunc");
	getchar();
	

	/* step 4: invoke library function using pointer */
	i= (fnptr) (10,20);
	printf(" result of function = %d\n",i);

	/* step 5: request link-loader to unload/detach library when no longer needed */ 
	dlclose(handle);
	getchar();
	return 0;
}	
