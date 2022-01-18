#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#define CMD_SIZE 100
#define SIZE_HOST 256

const char* port="69";
const char needMoreArgs[]= "Désolé, le nombre d'arguments doit être supérieur a 3 \n";
char displayArg[CMD_SIZE];



int main  (int argc, char ** argv){
    struct addrinfo hints;
    struct addrinfo *result, *tmp;	

	
	if (argc<3){
	write (STDOUT_FILENO,needMoreArgs,strlen(needMoreArgs));
	exit(EXIT_FAILURE);
		}

	/*Utilisé dans la question 1*/	
		for (int i=0; i<argc ; i++){
		sprintf(displayArg, "argument %d : %s \n", i,argv[i]);
		write (STDOUT_FILENO, displayArg, strlen(displayArg));
		}
		
	memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* On se met sur INET car on est en IPV4*/
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */ //UDP donc on met SOCK_DGRAM
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_UDP;          /* on change car on est en UDP*/

   int s = getaddrinfo(argv[1], port, &hints, &result); // MOdifier car notre adresse est en argv[1] et le port est 69
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
    char host [SIZE_HOST];
    char retIpServ[SIZE_HOST];

    for (tmp = result; tmp != NULL; tmp = tmp->ai_next) {
        getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
        sprintf(retIpServ, "%s \n", host);
        write(STDOUT_FILENO,retIpServ, strlen (retIpServ)); // Retour de l'adresse IP du Serveur quand on rentre le nom de l'hote en argument.
    }
    
    int retSock=socket (result->ai_family,result->ai_socktype,result->ai_protocol); 
    
    if (retSock ==-1){
		char dispRetSock[CMD_SIZE]="Socket non reservé \n";
		write (STDOUT_FILENO,dispRetSock, strlen(dispRetSock));
		exit(EXIT_FAILURE);
		}
   
	exit(EXIT_SUCCESS);
	}


