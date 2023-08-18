#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Allow multiple connections
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                  &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Set address family, IP address, and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to specified address and port
    if (bind(server_fd, (struct sockaddr *)&address,
            sizeof(address)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    // Accept connection from client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
        perror("Accepting connection failed");
        exit(EXIT_FAILURE);
    }

    // Communication loop
    while (1) {
        char buffer[1024] = {0};
        char message[1024] = "Lion Leo: ";

        // Receive message from the client
        if (read(new_socket, buffer, 1024) < 0) {
            perror("Reading input failed");
            exit(EXIT_FAILURE);
        }

        printf("Message from Miko: %s\n", buffer);

        // Prompt lion to enter a message
        printf("Enter your message (type 'quit' to exit): ");
        fgets(message + 10, sizeof(message) - 10, stdin);

        // Send lion's message to the client
        if (send(new_socket, message, sizeof(message), 0) < 0) {
            perror("Sending message failed");
            exit(EXIT_FAILURE);
        }

        // Check if lion wants to quit
        if (strncmp(message + 10, "quit", 4) == 0) {
            printf("Closing connection...\n");
            break;
        }
    }

    // Close the socket
    close(new_socket);

    return 0;
}
