#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 2048

void sort_asc(int a[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] > a[j]) {
                int t = a[i]; a[i] = a[j]; a[j] = t;
            }
}

void sort_desc(int a[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] < a[j]) {
                int t = a[i]; a[i] = a[j]; a[j] = t;
            }
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

    printf("Server waiting...\n");
    client_fd = accept(server_fd, NULL, NULL);

    while (1) {
        int nbytes = read(client_fd, buffer, BUF_SIZE - 1);
        if (nbytes <= 0) break;
        buffer[nbytes] = '\0';

        int choice;
        sscanf(buffer, "%d", &choice);

        if (choice == 5) {
            strcpy(response, "Exiting...\n");
            write(client_fd, response, strlen(response));
            break;
        }

        int n, arr[100], key;
        char *ptr = buffer;

        sscanf(ptr, "%d %d", &choice, &n);
        ptr = strchr(ptr, ' ') + 1;
        ptr = strchr(ptr, ' ') + 1;

        for (int i = 0; i < n; i++) {
            sscanf(ptr, "%d", &arr[i]);
            ptr = strchr(ptr, ' ');
            if (ptr) ptr++;
        }

        response[0] = '\0';

        if (choice == 1) {   // Search
            sscanf(ptr, "%d", &key);
            int found = 0;
            for (int i = 0; i < n; i++)
                if (arr[i] == key) found = 1;

            if (found)
                sprintf(response, "Number %d found\n", key);
            else
                sprintf(response, "Number %d not found\n", key);
        }

        else if (choice == 2) {  // Sort ascending
            sort_asc(arr, n);
            strcat(response, "Ascending order: ");
            for (int i = 0; i < n; i++) {
                char temp[20];
                sprintf(temp, "%d ", arr[i]);
                strcat(response, temp);
            }
            strcat(response, "\n");
        }

        else if (choice == 3) {  // Sort descending
            sort_desc(arr, n);
            strcat(response, "Descending order: ");
            for (int i = 0; i < n; i++) {
                char temp[20];
                sprintf(temp, "%d ", arr[i]);
                strcat(response, temp);
            }
            strcat(response, "\n");
        }

        else if (choice == 4) {  // Odd / Even
            strcat(response, "Even numbers: ");
            for (int i = 0; i < n; i++)
                if (arr[i] % 2 == 0) {
                    char temp[20];
                    sprintf(temp, "%d ", arr[i]);
                    strcat(response, temp);
                }

            strcat(response, "\nOdd numbers: ");
            for (int i = 0; i < n; i++)
                if (arr[i] % 2 != 0) {
                    char temp[20];
                    sprintf(temp, "%d ", arr[i]);
                    strcat(response, temp);
                }
            strcat(response, "\n");
        }

        write(client_fd, response, strlen(response));
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
