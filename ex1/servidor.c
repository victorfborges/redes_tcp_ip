#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 54321
#define MAX_LINE 256
#define MAX_PENDING 5

int main(){
	printf("ponto1\n");
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int len;
	int s, new_s;
	printf("ponto2\n");
	/*monta estrutura de dados de endereço*/
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);
	printf("ponto3\n");
	/* prepara abertura passiva */
	if((s=socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("simplex-talk: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0){
		perror("simplex-talk:bind");
		exit(1);
	}
	printf("ponto4\n");
	listen(s, MAX_PENDING);
	printf("ponto5\n");
	len = sizeof(sin);
	printf("ponto6\n");
	/* espera conexão, depois recebe e imprime texto */
	while(1){
		if((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0){
		perror("simplex-talk: accept");
		exit(1);
		}
		while(len = recv(new_s, buf, sizeof(buf), 0)){
			fputs(buf, stdout);
		}
		close(new_s);
		printf("porta fechada\n");
	}
}
	
