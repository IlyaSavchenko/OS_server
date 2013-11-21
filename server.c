# include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define BACKLOG 1000
#define BUFF 1024

char *CLIENT;
int listenfd, clients[BACKLOG];
void Server(char *);
void Client(int);

struct http
{
  char *header;
  char *body;
} reply;

int main(int argc, char* argv[]){
	struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char PORT[4];
    CLIENT = getenv("PWD");  
    strcpy(PORT,"9080");
    printf("localhost: %s%s%s\n","\033[92m",PORT,"\033[0m");
    
    int i;
    for (i = 0; i < BACKLOG; i++)
        clients[i] = -1;
    Server(PORT);

    int conn = 0;
	while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[conn] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[conn] < 0)
            error ("ERROR in accept!!!");
        else
        {
            if (fork() == 0)
            {
                Client(conn);
                exit(0);
            }
        }

        while (clients[conn] != -1) 
        	conn  = (conn  + 1) % BACKLOG;
    }
    return 0;
	
}

void Server(char *port)
{
    struct addrinfo hints, *res, *p;
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    //AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  //Указание своего хоста
    if (getaddrinfo(NULL, port, &hints, &res) != 0)
    {
        perror ("ERROR in getaddrinfo!!!");
        exit(1);
    }
    for (p = res; p != NULL; p = p -> ai_next)
    {
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) 
        	continue;
        if (bind(listenfd, p -> ai_addr, p -> ai_addrlen) == 0) 
        	break;
    }
    if (p == NULL) {
    	perror ("ERROR in socket!!!");
    	exit(1);
    }

    freeaddrinfo(res);  //освобождает память, предназначенную для динамически выделяемого связанного списка res.

    if (listen (listenfd, BACKLOG) != 0 ){
        perror("ERROR in listen!!!");
        exit(1);
    }
}

// int send_all(int socket, const void *buffer, size_t length, int flags)
// {
//     ssize_t n;
//     const char *p = buffer;
//     while (length > 0)
//     {
//         n = send(socket, p, length, flags);
//         if (n <= 0) break;
//         p += n;
//         length -= n;
//     }
//     return (n <= 0) ? -1 : 0;
// }

void Client(int n){
	char mesg[99999], *reqline[3], data_to_send[BUFF], path[99999];
    int rcvd, fd, bytes_read;

    memset( (void*)mesg, (int)'\0', 99999 );
    rcvd = read(clients[n], mesg, sizeof(mesg));

    if (rcvd < 0)   
        fprintf(stderr,("ERROR in read!!!\n"));
    else if (rcvd == 0)    // receive socket closed
        fprintf(stderr,"Client disconnected.\n");
    else {
    	printf("%s", mesg);
        reqline[0] = strtok (mesg, " \t\n");
        if (strncmp(reqline[0], "GET\0", 4) == 0 )
        {
            reqline[1] = strtok(NULL," ");

            if (strncmp(reqline[1],"/", 2) == 0)
            	reqline[1] = "/index.html";

            if ((fd = open(reqline[1] + 1, O_RDONLY)) != -1)
            {
            	reply.header = "HTTP/1.1 200 OK\n\n";
            	send(clients[n], reply.header, strlen(reply.header), 0);

            while((bytes_read = read(fd, data_to_send, BUFF)) > 0)
            {
                write(clients[n], data_to_send, bytes_read);
            }
            close(fd);
        }
        else
        {
            reply.header = "HTTP/1.1 404 Not Found\n\n";
            reply.body = "<html><body><h1>404 Not Found</h1></body></html>";
            send(clients[n], reply.header, strlen(reply.header), 0);
            send(clients[n], reply.body, strlen(reply.body), 0);
        }
    }
}

shutdown (clients[n], SHUT_RDWR);
close(clients[n]);
clients[n] = -1;

}
