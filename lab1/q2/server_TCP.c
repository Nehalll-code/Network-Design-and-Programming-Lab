#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
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
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE], response[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    printf("TCP Server running...\n");
    client_fd = accept(server_fd, NULL, NULL);

    while (1) {
        int n = read(client_fd, buffer, BUF_SIZE - 1);
        if (n <= 0) break;
        buffer[n] = '\0';

        if (strcmp(buffer, "Halt") == 0) {
            printf("Server terminating...\n");
            break;
        }

        int len = strlen(buffer);
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
                len, a, e, i, o, u);

        write(client_fd, response, strlen(response));
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
