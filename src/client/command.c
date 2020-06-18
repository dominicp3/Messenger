#include "client/command.h"
#define BUF_SIZE 512

void peer2peer(int fd)
{
        char buf[BUF_SIZE];

        struct pollfd fds[2];
        fds[0] = (struct pollfd) {.fd = STDIN_FILENO, .events = POLLIN, .revents = 0};
        fds[1] = (struct pollfd) {.fd = fd, .events = POLLIN, .revents = 0};

        int n;
        while (1) {
                if (poll(fds, 2, -1) == -1) {
                        perror("poll");
                        break;
                }

                if (fds[0].revents) {
                        fgets(buf, BUF_SIZE, stdin);
                        send_all(fd, buf, BUF_SIZE);
                } else if (fds[1].revents) {
                        n = recv_all(fd, buf, BUF_SIZE);
                        if (n == -1 || n == 0) {
                                n ? perror("client") : printf("lost connection\n");
                                break;
                        }
                        printf("%s", buf);
                }
        }
}

void print_message(int fd)
{
        uint8_t buf[BUF_SIZE];
        recv_all(fd, buf, BUF_SIZE);
        printf("%s", buf);
}

void connection_found(struct data *d)
{
        uint8_t header = (d->status == 1) ? 6 : 5;
        send_all(d->fd, &header, 1);

        d->status = 4;

        printf("Connection successful!\n\n");
        d->accept_command = 0;

        peer2peer(d->fd);

        shutdown(d->fd, SHUT_RDWR);
        close(d->fd);
}

void recieve_id(struct data *d)
{
        char buf[BUF_SIZE];
        recv_all(d->fd, buf, BUF_SIZE);
        d->id = atoi(buf);
}

char **split_string(char *str)
{
        int len = 2;
        char **s = malloc(len * sizeof *s);

        s[0] = strtok(str, " ");

        for (int i = 1; i < len; i++)
                s[i] = strtok(NULL, " ");

        return s;
}

void open_client(struct data *d)
{
        printf("client open for connections\n");
        d->status = 1;

        uint8_t header = 1;
        send_all(d->fd, &header, 1);
}

void connect_id(int fd, char *id_str)
{
        char buf[BUF_SIZE];
        strcpy(buf, id_str);
        uint8_t header = 2;
        send_all(fd, &header, 1);
        send_all(fd, buf, BUF_SIZE);
}

void command_help(void)
{
        printf("\nCOMMANDS:\n");
        printf("open: allow incoming connections\n");
        printf("connect <id>: connect to client with <id>\n");
        printf("clear: clear screen\n");
        printf("help: display this\n");
        printf("\n");
}
