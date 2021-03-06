#include	"unp.h"
#include	<time.h>
int bigorlittle()
{
	union {
	short  s;  
	char   c[sizeof(short)];
	} un; 
	un.s = 0x0102;
	
	if(sizeof(short)==2)
	{
		if(un.c[0]==1&&un.c[1]==2)
			return 2;
		else if(un.c[0]==2&&un.c[1]==1)
			return 1;
		else
			return 0;
	}
	return 0;
}
int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	socklen_t			len;
	struct sockaddr_in	servaddr, cliaddr;
	char				buff[MAXLINE];
	char                revline[MAXLINE+1];
	time_t				ticks;
	char                *exp="Input a b c... for applying different services\n";
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	int                 n=0;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		len = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &len);
		Writen(connfd,exp,strlen(exp));
	 	// printf("KangZewei connection from %s, port %d\n",Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),ntohs(cliaddr.sin_port));
	   // while(strcmp(revline,"exit")!=0)
	   while(1)
		{
		   // printf("n=%d\n",n);
			if((n=read(connfd,revline,MAXLINE))>0)
			{
				revline[n]=0;
				char temp = revline[0];
				switch(temp)
				{
					case 'a':
					    printf("KangZewei connection from %s, port %d\n",Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
						ticks = time(NULL);
						snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
						Writen(connfd, buff, strlen(buff));
						break;
					case 'b':
						temp=revline[1];
						int cli_order = temp-'0';
						int ser_order = bigorlittle();
						char *sendline;
						if (cli_order==ser_order)
						{
							if (cli_order==1)
								sendline="host byte order is same, little\n";
							else
								sendline="host byte order is same, Big\n";

						}
						else
						{
							if(cli_order==1)
								sendline="host byte order is differ, cli little,ser big\n";
							else
								sendline="host byte order is differ, cli big.ser little\n";
						}
						Writen(connfd,sendline,strlen(sendline));
						break;
					case 'c':
						printf("c\n");
						break;
					default:
						printf("reput\n");
						break;
				}
			}
		}

		Close(connfd);
	}
}
