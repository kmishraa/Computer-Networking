#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set address family, IP address, and port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Communication loop
    while (1) {
        // Prompt Miko to enter a message
        printf("Enter your message (type 'quit' to exit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send Miko's message to the server
        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            perror("Sending message failed");
            exit(EXIT_FAILURE);
        }

        // Check if Miko wants to quit
        if (strncmp(buffer, "quit", 4) == 0) {
            printf("Closing connection...\n");
            break;
        }

        // Receive reply from the server
        if (read(sock, buffer, sizeof(buffer)) < 0) {
            perror("Reading input failed");
            exit(EXIT_FAILURE);
        }

        printf("Reply from Leo: %s\n", buffer);
    }

    // Close the socket
    close(sock);

    return 0;
}
