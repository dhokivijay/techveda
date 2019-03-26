
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <strings.h>
# include <arpa/inet.h>
# include <string.h>
# include <netinet/in.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/sendfile.h>
# include <stdio_ext.h>


# define MAX_BUF 5000 

int sockfd,filefd;
ssize_t ret_val;
struct sockaddr_in servaddr,cliaddr;
char str[MAX_BUF];



void server_pwd()
{
	memset( &str , '\0' , MAX_BUF);
	recv( sockfd , str , MAX_BUF , 0 );
	write( 1 , str , strlen(str) );	
}

void filelist()
{
	long filesize,count,percent;
	char ack[4]="ACK";

	/* recieving server_file_stat filesize messege from server  */

	read( sockfd, &filesize , sizeof(long));
	//      printf("File List size: %lld bytes\n",(long long)sb.st_size);

	count    = 0;
	percent  = 0;


	/* recieving ack for filesize confirmation */
	read( sockfd , ack , sizeof(ack));


	/* recieveing the filelist of server's current directory and display in terminal */

	while(1)
	{
		memset( &str , '\0' , MAX_BUF);
		//              ret_val = read( sockfd , str , MAX_BUF );
		ret_val = recv( sockfd , str , MAX_BUF , 0 );

		if(ret_val < 0)
			break;

		write( 1 , str , strlen(str) );

		count+=strlen(str);
		percent = (count*100)/filesize;

		if(count == filesize)break;
	}
	/*	
		if( count == filesize)
		printf("Sucessfully recieved filelist of server's current directory\n");
		else
		printf("Failed to recieve filelist of server's current directory\n");
	 */
}












void upload(char *option)
{
	struct stat sb;
	long filesize=0,count,percentage;
	char filename[100];
	int ret_val,fd;
	char ack[4]="ACK";

	/* input the filename which you want to upload to server */

	bzero( filename,100);
	printf("Enter the file you want to upload::");
	__fpurge(stdin);
	scanf("%[^\n]s",filename);


	/* verifying if the file exists */

	fd=open( filename , O_RDONLY);
	if(fd == -1)
	{
		printf("The file to be uploaded is not found in current directory\n");
		return;
	}

	/* informing server to prepare itself for recieving the file */

	write( sockfd, option , strlen(option));

	/* recieving ack for  readiness */
	  read( sockfd , ack , sizeof(ack));

	/* sending the file name to server which you want to upload */

	ret_val=write( sockfd, filename , strlen(filename));
	if(ret_val < 0)
	{
		perror("write:");
	}

	/* sending the stat filesize of the file to be uploaded to server */

	bzero( &sb , sizeof(struct stat));
	fstat( fd , &sb );
	filesize = sb.st_size;
	count    = 0;
	percentage = 0;
	write( sockfd, &filesize , sizeof(long));

	printf("file:%s preparing to upload, of size:%ld\n", filename , filesize );


	/* upload complete file to server current directory */

	count = (long)sendfile( sockfd , fd , NULL , filesize );

}



void download()
{

	long filesize=0,count,percentage;
	char filename[100];
	char server_reply[20];
	int ret;
	char ack[4]="ACK";

	/* input the filename which you want to download */

	bzero( filename,100);
	printf("Enter the file you want to download::");
	__fpurge(stdin);
	scanf("%[^\n]s",filename);


	/* sending the file name to server which you want to download */

	ret_val=write( sockfd, filename , strlen(filename));
	if(ret_val < 0)
	{
		perror("write:");
	}

	    memset( server_reply , '\0' , sizeof(server_reply));
	if( strcmp( server_reply , "not found") == 0)
	{
		printf("TYPE>>no such file found in current directory of server\n");
		return;
	}

	/* recieving server_file_stat structure messege from server  */

	read( sockfd, &filesize , sizeof(long));
	printf("File size: %ld bytes\n",filesize);

	count    = 0;
	percentage  = 0;


	/* creating the same file as server */

	filefd=open( filename , O_CREAT|O_RDWR ,0666);
	if(filefd<0)
	{
		perror("inputfile open\n");
		exit(0);
	}


	/* downloading the file contents and copying it to the newly created file */
	count=0;
	while(1)
	{
		memset( &str , '\0' , MAX_BUF);
		ret_val = read( sockfd , str , MAX_BUF );

		if(ret_val <= 0)
			break;

		write( filefd , str , ret_val );

		count+=ret_val;
		if(count == filesize) 
			break;

	}
	close( filefd );
}









int main(int argc, char *argv[])
{
	char option[50];
	char serv_ip[100];

	if(argc < 2)
	{
		printf("Loopback address selected\n");
		strcpy( serv_ip , "127.0.0.1" );
	}
	else
	{
		
		strcpy( serv_ip , "argv[1]" );
		printf("Client to be connected to IP address::%s",serv_ip);
	}



	/* configuration for socket communication */

	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(atoi(argv[1]));
	inet_pton(AF_INET , serv_ip , &servaddr.sin_addr);

	ret_val = connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr));
	if(ret_val < 0)
	{
		perror("connect:");
		exit(1);
	}

	socklen_t len = sizeof(cliaddr);
	if (getsockname( sockfd , (struct sockaddr *)&cliaddr, &len) == -1)//fetching portno. of (client/of itself)
		perror("getsockname");
	printf(" Established connection with server at Client portno:%d\n",ntohs( cliaddr.sin_port));

	while(1)
	{
		/* choose your option and request server for your task */

		printf("\n                  SELECT THE OPTION OF YOUR CHOICE \n");
		printf("\n	          spwd: tells server pwd,pwd is client pwd \n");
		printf("TYPE <spwd>     : <display current directory of server>\n");
		printf("TYPE <pwd>      : <display current directory of client>\n");
		printf("TYPE <sdir>     : <display file names in current directory of server>\n");
		printf("TYPE <dir>      : <display file names in current directory of client>\n");
		printf("TYPE <scd path> : <change current directory of server>\n");
		printf("TYPE <cd path>  : <change current directory of client>\n");
		printf("TYPE <download> : <download any file from current directory of server>\n");
		printf("TYPE <upload>   : <upload any file from current directory of client to server>\n");
		printf("TYPE <quit>     : <terminate the client session>\n");
		memset ( option , '\0' , sizeof(option));
		__fpurge(stdin);
		scanf("%[^\n]s",option);

		if(strcmp(option,"download")==0)
		{
			write( sockfd, option , strlen(option));
			download();
		}
		else if(strcmp(option,"upload")==0)
		{
			upload(option);
		}
		else if(strcmp(option,"quit")==0)
		{
			write( sockfd, option , strlen(option));
			exit(0);
		}
		else if(strcmp(option,"dir")==0)
		{
			system(option);
		}
		else if(strcmp(option,"sdir")==0)
		{
			memmove( option , option+1 , strlen(option+1) + 1 );
			write( sockfd, option , strlen(option));
			filelist();
		}
		else if(strcmp(option,"pwd")==0)
		{
			system(option);
		}
		else if(strcmp(option,"spwd")==0)
		{
			memmove( option , option+1 , strlen(option+1) + 1 );
			write( sockfd, option , strlen(option));
			server_pwd();
		}
		else if( strstr(option , "cd ") == option )
		{
			memmove( option , option + 3 , strlen(option + 3) + 1 );
			chdir(option);
		}
		else if( strstr(option , "scd ") == option )
		{
			memmove( option , option + 1 , strlen(option + 1) + 1 );
			write( sockfd, option , strlen(option));
		}
		else
			printf("TYPE>>Invalied Option\n");

	}       
	close( sockfd );

	return 0;
}

