
#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#define DEFAULT_PORT "2020"
#define MYHOST "192.168.0.8"
using namespace std;

int main(void)
{

    //CODE because I'm creating on a window system
    WSADATA WsaDat;
    if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0) {
        cout << "WSA FAILED\n";
        cin.get();
        return 0;
    }
    //CODE because I'm creating on a window system
    //The rest	of	the code	would	suffice alone on Linux

    int new_fd;
    int sockfd;
    int addrcheck;
    int numbytes;
    struct addrinfo hints;
    struct addrinfo *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    char buffer[256];
    char client_ip[16];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; //IP version agnostic
    hints.ai_socktype = SOCK_STREAM; //TCP stream
    hints.ai_flags = AI_PASSIVE; //	use my IP

    //load up struct	servinfo
    if ((addrcheck = getaddrinfo(MYHOST, DEFAULT_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(addrcheck));
        return 1;
    }

    //get a	reliable	result
    for (p = servinfo; p != NULL; p = p->ai_next) {
        //create a socket based on the	getaddrinfo	return struct
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            //if it fails show the error and	continue	the loop
            perror("error on sockfd \n");
            continue;
        }
        else {
            printf("socket created \n");
        }

        //bind
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("error on bind \n");
            continue;
        }
        else {
            printf("bound \n");
        }
        //when	one successfully creates a	socket, and binds move on
        break;
    }

    //free the	structure
    freeaddrinfo(servinfo);

    //listen
    if (listen(sockfd, 10) == -1) {
        perror("error on listen \n");
        exit(1);
    }
    else {
        printf("listening \n");
    }

    //accept an incoming connection
    new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
    if (new_fd == -1) {
        perror("error on accept");
    }
    else {
        printf("accepted \n");
    }

    //receive from the connection
    if ((numbytes = recv(new_fd, buffer, 255, 0)) == -1) {
        perror("error on recv \n");
        printf("number of bytes: %i", numbytes);
    }

    //print the received message
    if (numbytes > 0) {
        printf("DATA RECEIVED: %s (%i bytes)\n", buffer, numbytes);
    }

    //close	connection
    close(new_fd);

    return 0;
}
