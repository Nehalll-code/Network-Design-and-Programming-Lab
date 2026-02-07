#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 4096

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

    printf("Enter file name: ");
    scanf("%s", buffer);
    write(sock, buffer, strlen(buffer));

    read(sock, response, BUF_SIZE);
    if (strncmp(response, "File not present", 16) == 0) {
        printf("File not present\n");
        return 0;
    }

    while (1) {
        int option;
        printf("\n1.Search 2.Replace 3.Reorder 4.Exit\n");
        scanf("%d", &option);

        if (option == 1) {
            char s[100];
            printf("Enter string: ");
            scanf("%s", s);
            sprintf(buffer, "1 %s", s);
        }
        else if (option == 2) {
            char s1[100], s2[100];
            printf("Enter str1 and str2: ");
            scanf("%s %s", s1, s2);
            sprintf(buffer, "2 %s %s", s1, s2);
        }
        else if (option == 3) strcpy(buffer, "3");
        else if (option == 4) {
            strcpy(buffer, "4");
            write(sock, buffer, strlen(buffer));
            break;
        }

        write(sock, buffer, strlen(buffer));
        read(sock, response, BUF_SIZE);
        printf("%s", response);
    }

    close(sock);
    return 0;
}
