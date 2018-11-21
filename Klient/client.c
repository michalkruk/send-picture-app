#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
void gotoxy(int x,int y)
 {
 printf("%c[%d;%df",0x1B,y,x);
 }
int main(int argc, char *argv[])
{
    system("czysto");
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[1024];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;

    /* Tworzenie gniazda*/
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Blad! Nie mozna stworzyc gniazdka\n");
        return 1;
    }

    /* Inicjalizacja sockaddr_in w strukturze danych */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); // port
    char ip[50];
if (argc < 2) 
{
    printf("Z jakim adresem IP mam sie polaczyc: ");
    gets(ip);
}
else
  strcpy(ip,argv[1]);

    serv_addr.sin_addr.s_addr = inet_addr(ip);

    /* Próba połączenia*/
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Blad! Nie moge sie polaczyc \n");
        return 1;
    }

    printf("Polaczono z adresem IP: %s : %d\n",inet_ntoa(serv_addr.sin_addr),ntohs(serv_addr.sin_port));

   	 /* Tworzymy plik, do przechowywania danych */
    	FILE *fp;
	char fname[100];
	read(sockfd, fname, 256);
	
	printf("Plik o nazwie: %s\n",fname);
	printf("Odbieranie pliku");
   	 fp = fopen(fname, "ab"); 
    	if(NULL == fp)
    	{
       	 printf("Blad otwarcia pliku");
         return 1;
    	}
    long double sz=1;
    /* Odbieramy dane po 256 bitów*/
    while((bytesReceived = read(sockfd, recvBuff, 1024)) > 0)
    { 
        sz++;
        gotoxy(0,4);
        printf("Odebrano: %llf Mb",(sz/1024));
	fflush(stdout);

        fwrite(recvBuff, 1,bytesReceived,fp);

    }

    if(bytesReceived < 0)
    {
        printf("\n Blad odczytu! \n");
    }
    printf("\nTransakcja zakonczona\n");
    return 0;
}
