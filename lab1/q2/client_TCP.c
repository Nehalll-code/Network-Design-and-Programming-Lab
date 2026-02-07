#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE], response[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (1) {
        printf("\nEnter string: ");
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        write(sock, buffer, strlen(buffer));

        if (strcmp(buffer, "Halt") == 0) {
            printf("Client terminating...\n");
            break;
        }

        int n = read(sock, response, BUF_SIZE - 1);
        response[n] = '\0';

        printf("\n--- Server Response ---\n%s", response);
    }

    close(sock);
    return 0;
}
