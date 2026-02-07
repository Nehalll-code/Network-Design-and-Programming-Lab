#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

int is_palindrome(char str[]) {
    int i = 0, j = strlen(str) - 1;
    while (i < j) {
        if (str[i] != str[j])
            return 0;
        i++;
        j--;
    }
    return 1;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE], response[BUF_SIZE];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("UDP Server running...\n");

    while (1) {
        len = sizeof(client_addr);
        int n = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0,
                         (struct sockaddr *)&client_addr, &len);

        buffer[n] = '\0';

        if (strcmp(buffer, "Halt") == 0) {
            printf("Server terminating...\n");
            break;
        }

        int length = strlen(buffer);
        int a = 0, e = 0, i = 0, o = 0, u = 0;

        for (int k = 0; buffer[k]; k++) {
            switch (buffer[k]) {
                case 'a': case 'A': a++; break;
                case 'e': case 'E': e++; break;
                case 'i': case 'I': i++; break;
                case 'o': case 'O': o++; break;
                case 'u': case 'U': u++; break;
            }
        }

        sprintf(response,
                "Palindrome: %s\nLength: %d\nVowels:\n"
                "a=%d e=%d i=%d o=%d u=%d\n",
                is_palindrome(buffer) ? "Yes" : "No",
                length, a, e, i, o, u);

        sendto(sockfd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, len);
    }

    close(sockfd);
    return 0;
}
