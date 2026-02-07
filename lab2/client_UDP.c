#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 4096

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

    printf("Enter file name: ");
    scanf("%s", buffer);

    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    len = sizeof(server_addr);
    recvfrom(sockfd, response, BUF_SIZE, 0,
             (struct sockaddr *)&server_addr, &len);

    if (strncmp(response, "File not present", 16) == 0) {
        printf("File not present\n");
        return 0;
    }

    while (1) {
        int option;
        printf("\n1.Search 2.Replace 3.Reorder 4.Exit\n");
        scanf("%d", &option);
        sprintf(buffer, "%d", option);

        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (option == 4) break;

        recvfrom(sockfd, response, BUF_SIZE, 0,
                 (struct sockaddr *)&server_addr, &len);
        printf("%s", response);
    }

    close(sockfd);
    return 0;
}
