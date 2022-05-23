#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SERVER_PORT 54321
#define MAX_LINE 256

int main (int argc, char *argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin; 
	char *host;
	char buf[MAX_LINE];
	int s;
	int len;
	struct in_addr addr;
	char input_text[100];
	char final_text[100];
	
	/* abertura e leitura de arquivo de entrada */
	if((fp=fopen("input.txt","r"))==NULL)
		printf("file open fail\n");
	else
		printf("file open sucess\n");
	
	/* verificação da passagem do endereço IP como parametro */
	if(argc==2){
		host=argv[1];
		
	}		
	else{
		fprintf(stderr, "usage: simples-talk host\n");
		exit(1);
	}
	
	/*traduz nome do host para endereço IP do host*/
	inet_aton(host, &addr);
	hp = gethostbyaddr(&addr, sizeof(addr), AF_INET);
	if(!hp){
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}
	
	/*monta estrutura de dados do endereço*/
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);
	
	/*abertura ativa*/
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("simplex-talk: socket");
		exit(1); 
	}
	if(connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		close(s);
		exit(1); 
	}
	
	printf("connect sucess\n");
	/* laço principal: obtém e envia linhas de texto */
	while (fgets(buf, sizeof(buf), stdin)){
		buf[MAX_LINE-1] = '\0';
		len = strlen(buf) + 1;
		send(s, buf, len, 0);
	}
	fclose(fp);
	printf("end of transmission\n");
}
