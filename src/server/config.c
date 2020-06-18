#include "server/config.h"

int main(int argc, char **argv)
{
        if (argc != 3) {
                printf("%s <ip> <port>\n", argv[0]);
                return 0;
        }

        char ip[16];
        strcpy(ip, argv[1]);

        uint16_t port = atoi(argv[2]);

        printf("\nIP Address : %s\n", ip);
        printf("Port Number: %d\n\n", port);

        run_server(config_server(port, ip), ip, port);
}

int config_server(uint16_t port, char *ip)
{
        int fd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in server_name;
        memset(&server_name, 0, sizeof server_name);

        server_name.sin_family = AF_INET;
        server_name.sin_port = port;
        inet_aton(ip, (struct in_addr*) &server_name.sin_addr);

        bind(fd, (struct sockaddr*) &server_name, sizeof server_name);
        listen(fd, SOMAXCONN);

        return fd;
}