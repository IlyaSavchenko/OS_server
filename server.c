#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

main{
	char *CLIENT;
	struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char PORT[4];
    CLIENT = getenv("PWD");
    strcpy(PORT,"9080");

    int i;
    for (i = 0; i < 1000; i++)
        clients[i] = -1;
    startServer(PORT);
   
    return 0;
}
	
}
