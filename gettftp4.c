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
#define SIZE_BUFFER 1500

const char* port="69";
const char needMoreArgs[]= "Désolé, le nombre d'arguments doit être supérieur a 3 \n";
char displayArg[CMD_SIZE];

/* Fonctions */

/* Fonction defRRQ 
 * Cette fonction permet de créer un paquet a envoyer en Request Read
 * elle prend en argument filename le nom du fichier, le mode et cmd la commande
 * à effectuer. Elle retourne un int qui correspond à la taille de la cmd */
 
 int defRRQ(char* filename, char* mode, char* cmd){
	 //cmd= malloc(2+strlen(filename)+1+strlen(mode)+1); //2= nombre d'octet pour OPCODE et 1= nombre d'octet pour la fin de la chaine de caractere de filename et mode
	 cmd[0]=0;
	 cmd[1]=1;
	 strcpy(&cmd[2],filename);
	 strcpy(&cmd[3+strlen(filename)],mode);
	 return strlen(filename)+strlen(mode)+4;
 }


int main  (int argc, char ** argv){
    struct addrinfo hints;
    struct addrinfo *result, *tmp;	
	struct sockaddr src_adress;
	socklen_t len_addsrc= sizeof(src_adress);
	
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
   
   //Question 4-a:
   char cmd[CMD_SIZE];
   int sizeofCmd= defRRQ(argv[2],"octet",cmd);
   
   
   int isiterror=sendto(retSock, cmd, sizeofCmd,0,result->ai_addr,result->ai_addrlen); // send le paquet request, visible sur wireshark
   
    if ( isiterror== -1) {
        fprintf(stderr, "erreur transmission: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);}
        
        char buffer[SIZE_BUFFER];
        int ret_recv=recvfrom(retSock, buffer, SIZE_BUFFER,0,&src_adress,&len_addsrc);
        
        write(STDOUT_FILENO, buffer,ret_recv);
        
	exit(EXIT_SUCCESS);
	}



