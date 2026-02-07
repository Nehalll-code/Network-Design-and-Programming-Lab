#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE], response[BUF_SIZE];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("\nEnter string: ");
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (strcmp(buffer, "Halt") == 0) {
            printf("Client terminating...\n");
            break;
        }

        len = sizeof(server_addr);
        int n = recvfrom(sockfd, response, BUF_SIZE - 1, 0,
                         (struct sockaddr *)&server_addr, &len);

        response[n] = '\0';
        printf("\n--- Server Response ---\n%s", response);
    }

    close(sockfd);
    return 0;
}
