#include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>


main(int argc, char* argv[]){
    char *USER;
    int *clients;
    struct sockaddr_in clientaddr; //sockaddr — обобщённая структура адреса, к которой, в зависимости от используемого семейства протоколов, приводится соответствующая структура -> Wiki
    socklen_t addrlen; // Представляет длину структуры sockaddr -> Wiki
    char abr;	
    char PORT[4]; 
    strcpy(PORT,"4444");

    if ((abr = getopt(argc, argv, "p:u:")) < -1)
        switch (abr)
        {
            case 'u':
                USER = malloc(strlen(optarg));
                strcpy(USER,optarg);
                break;
            case 'p':
                strcpy(PORT,optarg);
                break;
            default:
                exit(1);
        }
        
        int con = 0;
        while (true)
    	{
        	addrlen = sizeof(clientaddr);
        	clients[con] = accept(listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        	if (clients[con] < 0)
            	error ("accept() error");

        	while (clients[slot] != -1) 
        		slot = (slot + 1) % CONNMAX;
    	}

    	return 0;
          
}
