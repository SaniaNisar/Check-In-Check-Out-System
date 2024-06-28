#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h> 
#include <unistd.h>

#define SERVER_PORT 2000
#define SERVER_IP "127.0.0.1"

int main(void) {
    int socket_desc;
    struct sockaddr_in server_address;
    char server_response[2000], client_message[2000];
    int server_address_length = sizeof(server_address);

    memset(server_response, '\0', sizeof(server_response));
    memset(client_message, '\0', sizeof(client_message));

    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_desc < 0) {
        printf("Could Not Create Socket. Error!!!!!\n");
        return -1;
    }

    printf("Socket Created\n");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (1) {
        printf("Enter Message (Format: YY-AAAA-CI/CO or 'exit' to quit): ");
        fgets(client_message, sizeof(client_message), stdin);

        size_t len = strlen(client_message) - 1;
        if (client_message[len] == '\n') {
            client_message[len] = '\0';
        }

        if (strcmp(client_message, "exit") == 0) {
            break;
        }

        if (sendto(socket_desc, client_message, strlen(client_message), 0, (struct sockaddr*)&server_address, server_address_length) < 0) {
            printf("Send Failed. Error!!!!\n");
            return -1;
        }

        if (recvfrom(socket_desc, server_response, sizeof(server_response), 0, (struct sockaddr*)&server_address, &server_address_length) < 0) {
            printf("Receive Failed. Error!!!!!\n");
            return -1;
        }

        printf("Server Response: %s\n\n", server_response);

        memset(server_response, '\0', sizeof(server_response));
        memset(client_message, '\0', sizeof(client_message));
    }

    close(socket_desc);
    return 0;
}
