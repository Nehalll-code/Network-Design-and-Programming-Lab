#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 8080
#define BUF_SIZE 4096

void reorder_file(FILE *fp) {
    char text[BUF_SIZE];
    int i, j, n;

    rewind(fp);
    n = fread(text, 1, BUF_SIZE, fp);

    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (text[i] > text[j]) {
                char t = text[i];
                text[i] = text[j];
                text[j] = t;
            }

    rewind(fp);
    ftruncate(fileno(fp), 0);
    fwrite(text, 1, n, fp);
}

int count_occurrences(char *text, char *pat) {
    int count = 0;
    char *p = text;
    while ((p = strstr(p, pat)) != NULL) {
        count++;
        p++;
    }
    return count;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE], response[BUF_SIZE];
    FILE *fp;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    printf("TCP File Server running...\n");
    client_fd = accept(server_fd, NULL, NULL);

    /* Receive file name */
    int n = read(client_fd, buffer, BUF_SIZE - 1);
    buffer[n] = '\0';

    fp = fopen(buffer, "r+");
    if (!fp) {
        write(client_fd, "File not present", 16);
        close(client_fd);
        close(server_fd);
        return 0;
    }

    write(client_fd, "File present", 12);

    while (1) {
        n = read(client_fd, buffer, BUF_SIZE - 1);
        buffer[n] = '\0';

        int option;
        sscanf(buffer, "%d", &option);

        if (option == 4) break;

        rewind(fp);
        char text[BUF_SIZE];
        fread(text, 1, BUF_SIZE, fp);
        text[BUF_SIZE - 1] = '\0';

        if (option == 1) {
            char key[100];
            sscanf(buffer, "1 %s", key);
            int c = count_occurrences(text, key);
            if (c)
                sprintf(response, "String found %d times\n", c);
            else
                st
