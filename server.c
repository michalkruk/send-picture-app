#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>


struct sockaddr_in c_addr;
char fname[100];

void* SendFileToClient(int *arg)
{
      int connfd=(int)*arg;
      printf("Polaczenie zaakceptowane, id polaczenia: %d\n",connfd);
      printf("Polaczono z klientem: %s:%d\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
       write(connfd, fname,256);

        FILE *fp = fopen(fname,"rb");
        if(fp==NULL)
        {
            printf("Blad otwarcia pliku");
            return 1;   
        }   

        /* Odczytujemy dane i je wysylamy */
        while(1)
        {
            /* Czytaj plik po 256 bitow */
            unsigned char buff[1024]={0};
            int nread = fread(buff,1,1024,fp);
      

            /* Jesli udalo sie przeczytac, to wyslij dane */
            if(nread > 0)
            {

                write(connfd, buff, nread);
            }
            if (nread < 1024)
            {
                if (feof(fp))
		{
                    printf("Koniec pliku\n");
		    printf("Transfer powiodl sie dla transkacji o numerze id: %d\n",connfd);
		}
                if (ferror(fp))
                    printf("Blad czytania danych!\n");
                break;
            }
        }
printf("Zamykam polaczenie dla transkacji o id: %d\n",connfd);
close(connfd);
shutdown(connfd,SHUT_WR);
sleep(2);
}

int main(int argc, char *argv[])
{
    int connfd = 0,err;
    pthread_t tid; 
    struct sockaddr_in serv_addr;
    int listenfd = 0,ret;
    char sendBuff[1025];
    int numrv;
    size_t clen=0;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0)
	{
	  printf("Blad w tworzeniu gniazda!\n");
	  exit(2);
	}

    printf("Udalo sie odzyskac gniazdko\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    ret=bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(ret<0)
    {
      printf("Blad przypisania!\n");
      exit(2);
    }

    if(listen(listenfd, 10) == -1)
    {
        printf("Nie udalo sie wykryc!\n");
        return -1;
    }

if (argc < 2) 
{
	printf("Wpisz nazwe pliku do wyslania: ");
        gets(fname);
}
else
   strcpy(fname,argv[1]);

    while(1)
    {
        clen=sizeof(c_addr);
        printf("Oczekiwanie\n");
        connfd = accept(listenfd, (struct sockaddr*)&c_addr,&clen);
        if(connfd<0)
        {
	  printf("Blad w akceptacji\n");
	  continue;	
	}
        //err = pthread_create(&tid, NULL, &SendFileToClient, &connfd);
        if (err != 0)
            printf("\nNie moge stworzyc watku:[%s]", strerror(err));
   }
    close(connfd);
    return 0;
}
