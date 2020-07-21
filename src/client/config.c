#include "client/config.h"

int main(int argc, char **argv)
{
        if (argc != 3) {
                printf("%s <ip> <port>\n", argv[0]);
                return 0;
        }

        char ip[16];
        strcpy(ip, argv[1]);

        uint16_t port = atoi(argv[2]);

        int fd = config_client(port, ip);
        if (fd == -1)
                return 0;

        struct data *d = malloc(sizeof *d);
        *d = (struct data) {.fd = fd, .status = 0, .s = NULL, .accept_command = 1};

        request(d);
        free(d);
        return 0;
}

int config_client(uint16_t port_number, char *ip_addr)
{
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof serv_addr);

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = bswap_16(port_number);
        inet_aton(ip_addr, (struct in_addr*) &serv_addr.sin_addr);

        int fd = socket(AF_INET, SOCK_STREAM, 0);

        if (fd == -1) {
                perror("socket error");
                return -1;
        }

        if (connect(fd, (struct sockaddr*) &serv_addr, sizeof serv_addr) == -1) {
                perror("connect error");
                return -1;
        }

        return fd;
}