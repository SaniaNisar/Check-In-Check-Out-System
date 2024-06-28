#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h> 
#include <unistd.h> 

#define CHECK_IN "CI"
#define CHECK_OUT "CO"

struct StudentStatus {
    int isCheckedIn;
};

#define MAX_STUDENTS 100
struct StudentStatus student_db[MAX_STUDENTS];
int student_count = 0;

int process_message(char* message, char* response) {
    char* token = strtok(message, "-");
    int roll_number = atoi(token); 

    token = strtok(NULL, "-"); 
    token = strtok(NULL, "-"); 

    if (strcmp(token, CHECK_IN) == 0) {
        if (roll_number < 0 || roll_number >= MAX_STUDENTS) {
            sprintf(response, "Invalid roll number.");
            return -1;
        }

        if (student_db[roll_number].isCheckedIn) {
            sprintf(response, "You are already here.");
        } else {
            student_db[roll_number].isCheckedIn = 1;
            sprintf(response, "Welcome Student %d", roll_number);
        }
    } else if (strcmp(token, CHECK_OUT) == 0) {
        if (roll_number < 0 || roll_number >= MAX_STUDENTS) {
            sprintf(response, "Invalid roll number.");
            return -1;
        }

        if (!student_db[roll_number].isCheckedIn) {
            sprintf(response, "You didn’t check in today. Contact System Administrator.");
        } else {
            student_db[roll_number].isCheckedIn = 0;
            sprintf(response, "Good Bye Student %d! Have a nice day.", roll_number);
        }
    } else {
        sprintf(response, "Invalid action.");
        return -1;
    }

    return 0;
}

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int client_struct_length = sizeof(client_addr);

    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    memset(student_db, 0, sizeof(student_db)); 

    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_desc < 0) {
        printf("Could Not Create Socket. Error!!!!!\n");
        return -1;
    }

    printf("Socket Created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Bind Failed. Error!!!!!\n");
        return -1;
    }

    printf("Bind Done\n");
    printf("Listening for Messages...\n\n");

    while (1) {
        if (recvfrom(socket_desc, client_message, sizeof(client_message), 0, (struct sockaddr*)&client_addr, &client_struct_length) < 0) {
            printf("Receive Failed. Error!!!!!\n");
            continue; 
        }

        printf("Received Message from IP: %s and Port No: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Client Message: %s\n", client_message);

        process_message(client_message, server_message);

        if (sendto(socket_desc, server_message, strlen(server_message), 0, (struct sockaddr*)&client_addr, client_struct_length) < 0) {
            printf("Send Failed. Error!!!!!\n");
            continue; 
        }

        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));
    }

    close(socket_desc);
    return 0;
}
