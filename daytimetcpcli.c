#include	"unp.h"

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
			hostbyteorder(sendline);	

		}

		Writen(sockfd,sendline,strlen(sendline));

		if (Readline(sockfd,recvline,MAXLINE)==0)
			err_quit("err");
		//printf("%s",recvline);
		Fputs(recvline,stdout);
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

