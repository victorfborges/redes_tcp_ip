#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define SERVER_PORT 54321
#define MAX_LINE 256
#define MAX_PENDING 5

int main(){

	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int len;
	int s, new_s;

	/*monta estrutura de dados de endereço*/
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);

	/* prepara abertura passiva */
	if((s=socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("simplex-talk: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0){
		perror("simplex-talk:bind");
		exit(1);
	}
	
	listen(s, MAX_PENDING);
	
	len = sizeof(sin);

	while(1){
		// espera uma conexão ser realizada
		if((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0){
		perror("simplex-talk: accept");
		exit(1);
		}
		// espera a chegada de uma mensagem, quando chega é
		// printado na tela e enviado a mesma mensagem de volta
		while(len = recv(new_s, buf, sizeof(buf), 0)){
			fputs(buf, stdout);
			buf[MAX_LINE-1] = '\0';
			send(new_s, buf, sizeof(buf), 0);
		}
		printf("porta fechada\n");
		close(new_s);
	}
}
	
