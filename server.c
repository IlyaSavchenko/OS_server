#include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>


main(){
	struct sockaddr_in clientaddr; //sockaddr — обобщённая структура адреса, к которой, в зависимости от используемого семейства протоколов, приводится соответствующая структура -> Wiki
    socklen_t addrlen; // Представляет длину структуры sockaddr -> Wiki
}