#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

main{
	char *CLIENT;
	int listenfd, clients[1000];
	struct sockaddr_in clientaddr;
    char *CLIENT;
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char PORT[4];
    CLIENT = getenv("PWD");  
    strcpy(PORT,"9080");

    for (int i = 0; i < 1000; i++)
        clients[i] = -1;
    Server(PORT);

    int conn = 0;
	while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[conn] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[conn] < 0)
            error ("ERROR in accept!!!");

        while (clients[conn] != -1) conn  = (conn  + 1) % 1000;
    }
    return 0;
	
}

void Server(char *port)
{
    struct addrinfo hints, *res, *p;
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    //AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port, &hints, &res) != 0)
    {
        perror ("ERROR in getaddrinfo!!!");
        exit(1);
    }
    for (p = res; p != NULL; p = p -> ai_next)
    {
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
    }
}
