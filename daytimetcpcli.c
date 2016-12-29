#include	"unp.h"
void
str_cli(FILE *fp, int sockfd)
{
        int                     maxfdp1, stdineof;
        fd_set          rset;
        char            buf[MAXLINE];
        int             n;

        stdineof = 0;
        FD_ZERO(&rset);
        for ( ; ; ) {
                if (stdineof == 0)
                        FD_SET(fileno(fp), &rset);
                FD_SET(sockfd, &rset);
                maxfdp1 = max(fileno(fp), sockfd) + 1;
                Select(maxfdp1, &rset, NULL, NULL, NULL);

                if (FD_ISSET(sockfd, &rset)) {  /* socket is readable */
                        if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
                                if (stdineof == 1)
                                        return;         /* normal termination */
                                else
                                        err_quit("str_cli: server terminated prematurely");
                        }
                        Write(fileno(stdout), buf, n);
                }

                if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
                        if ( (n = Read(fileno(fp), buf, MAXLINE)) == 0) {
                                stdineof = 1;
                                Shutdown(sockfd, SHUT_WR);      /* send FIN */
                                FD_CLR(fileno(fp), &rset);
                                continue;
                        }

                        Writen(sockfd, buf, n);
                }
        }
}
int Sendfile(int sockfd)  //return 1 if success or zero if fail
{
	int n=0;
	int length=0;
	char bufftmp[MAXLINE];
	char filename[MAXLINE+1];
	printf("Please input file name\n");
	scanf("%s",filename);
	Writen(sockfd,filename,strlen(filename)); // send the file name to the server
	FILE *fp = fopen(filename,"rb");
	if(fp==NULL)
	{
		printf("File:%s Not Found\n", filename);
		return 0;
	}
	bzero(bufftmp,MAXLINE);
	while((length=fread(bufftmp,sizeof(char),MAXLINE,fp))>0)
	{
		if(write(sockfd,bufftmp,length)<0)
		{
			printf("Send File %s failed\n",filename);
		}
		bzero(bufftmp,MAXLINE);

	}
	fclose(fp);
	printf("File:%s Transfer Successful!\n", filename);
	
	return 1;


}



int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(13);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");



	if ( (n = read(sockfd, recvline, MAXLINE)) > 0) 
	{
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}


	char sendline[MAXLINE];

	
	while(Fgets(sendline,MAXLINE,stdin)!=NULL)
	{
		if(sendline[0]=='b')
		{
			int result=hostbyteorder(sendline);	

		}
		Writen(sockfd,sendline,strlen(sendline));
		if(sendline[0]=='1')
		{
			str_cli(stdin,sockfd);
		}
		if(sendline[0]=='3')
		{
			Sendfile(sockfd);
		}
		else
		{
			if (Readline(sockfd,recvline,MAXLINE)==0)
				err_quit("err");
			//printf("%s",recvline);
			Fputs(recvline,stdout);

		}
		
	}
	if (n < 0)
		err_sys("read error");

	
	exit(0);
}

int hostbyteorder(char sendline[])
{
	union{
	short s;
	char c[sizeof(short)];
	}un;

	un.s=0x0102;
	if (sizeof(short)==2){
		if(un.c[0]==1&&un.c[1]==2)
			sendline[1]='2';
		else if (un.c[0]==2&&un.c[1]==1)
			sendline[1]='1';
		else 
			sendline[1]='0';

	}

	return 0;
}

