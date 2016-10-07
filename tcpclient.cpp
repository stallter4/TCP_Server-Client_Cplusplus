
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
using namespace std;

int main(int argc, char* argv[])
{

    //CODE because I'm creating on a window system
    WSADATA WsaDat;
    if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0) {
        cout << "WSA FAILED\n";
        cin.get();
        return 0;
    }
    //CODE because I'm creating on a window system
    //The rest of the code would suffice alone on Linux

    int sockfd;
    int addrcheck;
    int numbytes;
    struct addrinfo hints;
    struct addrinfo *servinfo, *p;
    char string1[] = "Nice to meet you";
    int bytes_sent = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; //IP version agnostic
    hints.ai_socktype = SOCK_STREAM; //TCP stream

    //load up struct servinfo
    if ((addrcheck = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(addrcheck));
        return 1;
    }

    //get a reliable result
    for (p = servinfo; p != NULL; p = p->ai_next) {
        //create a socket based on the getaddrinfo return struct
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            //if it fails show the error and continue the loop
            perror("error on sockfd \n");
            continue;
        }
        else {
            printf("socket created \n");
        }

        //connect
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("error on connect \n");
            continue;
        }
        else {
            printf("connected \n");
        }
        //when one successfully creates a sockets, and connects move on
        break;
    }

    //send message
    if ((bytes_sent = send(sockfd, string1, strlen(string1) + 1, 0)) == -1) {
        perror("error on send \n");
    }
    else {
        printf("sent %i\n", bytes_sent);
    }

    //close connection
    close(sockfd);

    return 0;
}
