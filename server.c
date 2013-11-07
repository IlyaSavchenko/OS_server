#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

char *CLIENT;
int listenfd, clients[1000];

main{
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
        if (bind(listenfd, p -> ai_addr, p -> ai_addrlen) == 0) 
        	break;
    }
    if (p == NULL) {
    	perror ("ERROR in socket!!!");
    	exit(1);
    }

    
    freeaddrinfo(res);  //освобождает память, предназначенную для динамически выделяемого связанного списка res.

    if ( listen (listenfd, 1000) == 0 ){
        perror("ERROR in listen!!!");
        exit(1);
    }
}

void Client(int n){
	char mesg[], *reqline[3], data_to_send[1024], path[];
    int rcvd, fd, bytes_read;

    memset( (void*)mesg, (int)'\0', 1000 );
    rcvd = recv(clients[n], mesg, 1000, 0);

    if (rcvd != 0)   
        fprintf(stderr,("ERROR in recv!!!\n"));
    else {
    	printf("%s", mesg);
        reqline[0] = strtok (mesg, " \t\n");
        if (strncmp(reqline[0], "GET\0", 4) == 0 )
        {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");
            if ( strncmp( reqline[2], "HTTP/1.0", 8) != 0 && strncmp( reqline[2], "HTTP/1.1", 8) != 0 )
            {
                send(clients[n], "HTTP/1.1 400 Bad Request\n", 25,0);
            }
            else
            {
                strcpy(path, CLIENT);
                strcpy(&path[strlen(CLIENT)], reqline[1]);
                if ( (fd = open(path, O_RDONLY)) != -1 )    
                {
                    send(clients[n], "HTTP/1.1 200 OK\r\n", 17, 0);
                    while ( (bytes_read = read(fd, data_to_send, 1024)) > 0 )
                        write (clients[n], data_to_send, bytes_read);
                }
                else    
                    send(clients[n], "HTTP/1.1 404 Not Found\n", 23,0); 
            }
    }

    shutdown (clients[n], SHUT_RDWR);
    close(clients[n]);
    clients[n] = -1;

}
