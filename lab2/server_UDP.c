#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 4096

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE], response[BUF_SIZE];
    socklen_t len;
    FILE *fp;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    len = sizeof(client_addr);
    recvfrom(sockfd, buffer, BUF_SIZE, 0,
             (struct sockaddr *)&client_addr, &len);

    fp = fopen(buffer, "r+");
    if (!fp) {
        sendto(sockfd, "File not present", 16, 0,
               (struct sockaddr *)&client_addr, len);
        return 0;
    }

    sendto(sockfd, "File present", 12, 0,
           (struct sockaddr *)&client_addr, len);

    while (1) {
        recvfrom(sockfd, buffer, BUF_SIZE, 0,
                 (struct sockaddr *)&client_addr, &len);

        int option = buffer[0] - '0';
        if (option == 4) break;

        strcpy(response, "Operation done\n");
        sendto(sockfd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, len);
    }

    fclose(fp);
    close(sockfd);
    return 0;
}
