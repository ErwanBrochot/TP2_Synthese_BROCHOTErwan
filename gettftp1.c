#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define CMD_SIZE 100

const char needMoreArgs[]= "Désolé, le nombre d'arguments doit être supérieur a 3 \n";
char displayArg[CMD_SIZE];

int main  (int argc, char ** argv){
	

	
	if (argc<3){
	write (STDOUT_FILENO,needMoreArgs,strlen(needMoreArgs));
	exit(EXIT_FAILURE);
		}
	else{
		
		for (int i=0; i<argc ; i++){
		sprintf(displayArg, "argument %d : %s \n", i,argv[i]);
		write (STDOUT_FILENO, displayArg, strlen(displayArg));
}
}
	exit(EXIT_SUCCESS);
	}

