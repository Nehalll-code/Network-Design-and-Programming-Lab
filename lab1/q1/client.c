#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 2048

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
        int choice, n, arr[100], key;

        printf("\n1.Search\n2.Sort Ascending\n3.Sort Descending\n");
        printf("4.Split Odd/Even\n5.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 5) {
            strcpy(buffer, "5");
            write(sock, buffer, strlen(buffer));
            read(sock, response, BUF_SIZE);
            printf("%s", response);
            break;
        }

        printf("Enter number of elements: ");
        scanf("%d", &n);

        printf("Enter elements: ");
        for (int i = 0; i < n; i++)
            scanf("%d", &arr[i]);

        if (choice == 1) {
            printf("Enter element to search: ");
            scanf("%d", &key);
            sprintf(buffer, "%d %d ", choice, n);
            for (int i = 0; i < n; i++) {
                char temp[20];
                sprintf(temp, "%d ", arr[i]);
                strcat(buffer, temp);
            }
            char temp[20];
            sprintf(temp, "%d", key);
            strcat(buffer, temp);
        }
        else {
            sprintf(buffer, "%d %d ", choice, n);
            for (int i = 0; i < n; i++) {
                char temp[20];
                sprintf(temp, "%d ", arr[i]);
                strcat(buffer, temp);
            }
        }

        write(sock, buffer, strlen(buffer));
        int nbytes = read(sock, response, BUF_SIZE - 1);
        response[nbytes] = '\0';
        printf("\nServer response:\n%s", response);
    }

    close(sock);
    return 0;
}
