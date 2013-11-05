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

    for (int i = 0; i < 1000; i++)
        clients[i] = -1;
    startServer(PORT);

    int conn = 0;
	while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[conn] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[conn] < 0)
            error ("ERROR!!!");

        while (clients[conn] != -1) conn  = (conn  + 1) % 1000;
    }
    return 0;
	
}
