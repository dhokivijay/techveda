# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <strings.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/sendfile.h>

# define MAX_BUF 4096
# pragma pack(1)  //used for previous version as i was sending stat structure,and facing issues in packaging across various systems

void upload(int connfd)
{
	long filesize=0,count,percent;
        struct stat sb;
        int fd;
	char filename[100];
	char search_result[20];
	char ack[4]="ACK";

	memset( search_result , '\0' , sizeof(search_result));	

	/* recieving filename to upload  */

	memset( filename , '\0' , sizeof(filename));
	read( connfd, filename , sizeof(filename));


	/* verifying if the file exists */

	fd=open( filename , O_RDONLY);
        if(fd == -1)
        {
		/* informing client that requested file is found */

		strcpy( search_result , "not found" );
        	write(connfd , search_result , strlen(search_result));
		printf("client requesting to upload file:%s not found\n",filename);
		return;
        }
        else
        {
		/* informing client that requested file is found */

		strcpy( search_result , "found" );
             //   write(connfd , search_result , strlen(search_result));

		/* recieving ack for filename confirmation */
		//read( connfd , ack , sizeof(ack));	
	
        	/* sending the stat filesize to client for requested file */

                bzero( &sb , sizeof(struct stat) );
                stat( filename , &sb );
		filesize = sb.st_size;
                count    = 0;
                percent  = 0;
                write( connfd, &filesize , sizeof(long));

                printf("file:%s preparing to upload, of size:%ld\n", filename , (off_t)sb.st_size );

		/* recieving ack for filesize confirmation */
                //read( connfd , ack , sizeof(ack));
		
                /* sending complete file to client */

                count = (long)sendfile( connfd , fd , NULL , filesize );
                percent = (count*100)/filesize;
                printf("%s%3ld%s Completed :: %10ld bytes uploaded\n"," [",percent,"%]",count);
	}

}


void download(int connfd)
{
	long filesize=0,count,percent;
        int fd,ret_val;
        char filename[100];
	char str[MAX_BUF];
	char ack[4]="ACK";

	
        write( connfd , ack , strlen(ack));



        memset( filename , '\0' , sizeof(filename));
        read( connfd, filename , sizeof(filename));
	printf("server preparing to recieve file:%s from client\n",filename);
	


        read( connfd, &filesize , sizeof(long));
        printf("File size: %ld bytes\n",filesize);

        count    = 0;
        percent  = 0;


	/* creating the same file as in client */

        fd=open( filename , O_CREAT|O_RDWR,0666);
        if(fd<0)
        {
                perror("inputfile open\n");
                exit(0);
        }

	
        while(1)
        {
                memset( &str , '\0' , MAX_BUF);
                ret_val = read( connfd , str , MAX_BUF );


                if(ret_val < 0)
                break;

                write( fd , str , ret_val );

                count+=ret_val;
          

		if(count == filesize)break;

        }
        close( fd );


}

void filelist( char *buf , int connfd )
{
	char str[20];
	int fd;
	off_t filesize,count,percent;
        struct stat sb;
	char ack[4]="ACK";
	
	

        strcat(buf," >> ");
        sprintf(str,"%d",getpid());
        strcat(buf,str);
        system(buf);


	

	fd=open( str , O_RDONLY);
        bzero( &sb , sizeof(struct stat) );
        stat( str , &sb );
	filesize = sb.st_size;
        count    = 0;
        percent  = 0;
        write( connfd, &filesize , sizeof(long));	



        write( connfd , ack , sizeof(ack));

	


        printf("list of files in current dir,preparing to sent of size:%lld\n", (long long)sb.st_size );

	count = (long)sendfile( connfd , fd , NULL , filesize );
      
        strcpy(buf,"rm ");
        strcat(buf,str);
        system(buf);
}

void server_pwd(char *buf , int connfd)
{
	char str[20];
	int fd;
	long long filesize;
	struct stat sb;
	

        strcat(buf," >> ");
        sprintf(str,"%d",getpid());
        strcat(buf,str);
        system(buf);


	
	fd=open( str , O_RDONLY);
	bzero( &sb , sizeof(struct stat) );
        stat( str , &sb );
	filesize = (long long)sb.st_size;

	sendfile( connfd , fd , NULL , filesize );

        strcpy(buf,"rm ");
        strcat(buf,str);
        system(buf);	
}


int main()
{
        int listfd,retval;
        listfd = socket( AF_INET , SOCK_STREAM , 0);
        socklen_t clilen;
        struct sockaddr_in servaddr,cliaddr;
        struct sigaction act;
        char buf[100];
	char send_buf[MAX_BUF];

        /* configuring for auto destruction of childs by kernel */

        act.sa_handler=SIG_DFL;
        act.sa_flags=SA_NOCLDWAIT;
        sigaction(SIGCHLD,&act,NULL);
        if(retval < 0 )
        {
                perror("sigaction \n");
                exit(1);
        }


     

        bzero(&servaddr ,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(atoi(argv[1]));


	

	int flag = 1;
        setsockopt(  listfd , SOL_SOCKET , SO_REUSEADDR , &flag , sizeof(flag) );


        /* assign address to socket */

        retval = bind( listfd , (struct sockaddr*)&servaddr , sizeof(servaddr));
        if (retval < 0)
        {
                perror("bind:");
                exit(2);
        }

        listen(listfd ,5);//enable 

        while(1)
        {
                clilen =  sizeof(cliaddr);
                int connfd = accept( listfd , (struct sockaddr*)&cliaddr, & clilen);

                if(fork() == 0)
                {
			while(1)
		 	{
				bzero(buf,100);
                		bzero(send_buf,100);

                        	read( connfd, buf, sizeof(buf));
				
				if(strcmp( buf,"download")==0)
				{
                        		printf("client port no:%d , requesting to download file from server\n",ntohs(cliaddr.sin_port));
					upload(connfd);
				}
				else if( strcmp( buf,"upload")==0 )
				{
					printf("client port no:%d , requesting to upload file to server\n",ntohs(cliaddr.sin_port));
                                        download(connfd);
				}
				else if( strcmp( buf,"quit")==0 )
				{
					printf("client port no:%d , terminating \n",ntohs(cliaddr.sin_port));
					close(connfd);
					exit(0);
				}
				else if( strcmp( buf,"dir")==0 )
				{
					printf("client port no:%d , requesting list of files in current directory\n",ntohs(cliaddr.sin_port));
					filelist( buf , connfd );
				}
				else if( strcmp( buf,"pwd")==0 )
                                {
                                        printf("client port no:%d , requesting for pwd of the server\n",ntohs(cliaddr.sin_port));
                                        server_pwd( buf , connfd );
                                }
				else if( strstr( buf,"cd ") == buf )
                                {
                                        printf("client port no:%d , requesting to change the directory server\n",ntohs(cliaddr.sin_port));
                                        memmove( buf , buf + 3 , strlen(buf + 3) + 1 );
                        		chdir(buf);
                                }				
		 	}
                }
        }
//      close(listfd);exit(1);//when server quits
}


