#include        "unp.h"
#include        <time.h>
int                 clinum=0;



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
void sig_chld(int signo)
{
        pid_t pid;
        int stat;
        while((pid=waitpid(-1,&stat,WNOHANG))>0)
        {
                printf("child %d terminated\n",pid );
                clinum--;
                printf("clinum: %d\n",clinum);
        }
}
void detailsrv(int id,int sockfd)
{
     int n=0;
     int length=0;
     char filename[MAXLINE+1];
     char bufftmp[MAXLINE];
     switch(id)
     {
        case 1:
                str_echo(sockfd);
                break;
        case 2:

                break;
        case 3:
                if((n=read(sockfd,filename,MAXLINE))>0)
                {
                        filename[n]=0;
                        FiLE *fp = fopen(filename,w);
                        if(fp==NULL)
                        {
                                printf("File:\t%s can not open to write\n",filename);
                                exit(1);
                        }
                }
                bzero(bufftmp,MAXLINE);
                while((length=recv(sockfd,bufftmp,MAXLINE,0))>0)
                {
                        if(fwrite(buffer, sizeof(char), length, fp) < length)
                        {
                                printf("File:\t%s write failed\n",filename);
                                break;
                        }
                        bzero(bufftmp,MAXLINE);
                }
                break;

     }
}
int
main(int argc, char **argv)
{
        pid_t                           childpid;
        int                                     listenfd, connfd;
        socklen_t                       len;
        struct sockaddr_in      servaddr, cliaddr;
        char                            buff[MAXLINE];
        char                revline[MAXLINE+1];
        time_t                          ticks;
        char                *exp="Input a b c... for applying different services\n";
        listenfd = Socket(AF_INET, SOCK_STREAM, 0);
        int                 n=0;
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family      = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port        = htons(13);   /* daytime server */

        Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
        Signal(SIGCHLD,sig_chld);
        Listen(listenfd, LISTENQ);

        for ( ; ; ) {
                len = sizeof(cliaddr);
                if((connfd = Accept(listenfd, (SA *) &cliaddr, &len))<0)
                {
                        if(errno==EINTR)
                        {
                                continue;
                        }
                        else
                                err_sys("Accept error");
                }
                if(clinum<3)
                {
                        clinum++;
                        if ((childpid=Fork())==0)
                    {
                        /* code */
                        Close(listenfd);
                        Writen(connfd,exp,strlen(exp));
//                          while(strcmp(revline,"exit")!=0)   //while(1)
//                              {
                                   // printf("n=%d\n",n);
again:

                                        while((n=read(connfd,revline,MAXLINE))>0)
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
                                                                snprintf(buff,sizeof(buff),"Input 1 2 3 4 for detail\r\n");
                                                                Writen(connfd,buff,strlen(buff));
                                                                while(n=read(connfd,revline,MAXLINE)>0)
                                                                {
                                                                        revline[n]=0;
                                                                        temp=revline[0];
                                                                        detailsrv(temp-'0',connfd);
                                                                        break;
                                                                }
                                                                break;
                                                        default:
                                                                printf("reput\n");
                                                                break;
                                                }
                                        }
                                        if(n<0&&errno==EINTR)
                                        {
                                                goto again;
                                        }
                                        else if(n<0)
                                        {
                                                err_sys("read erroe");
                                        }

                        //      }
                                exit(0);
                        }
                        Close(connfd);
                }
                else
                {
        //              connfd = Accept(listenfd,NULL,NULL);
                        Writen(connfd,"cli too much connect denied",strlen("cli too much connect denied"));
                        Close(connfd);
                        continue;
                }


                // printf("KangZewei connection from %s, port %d\n",Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),ntohs(cliaddr.sin_port));
           // while(strcmp(revline,"exit")!=0)




        }
}
