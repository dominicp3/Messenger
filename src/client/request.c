#include "client/request.h"
#define BUF_SIZE 512

void request(struct data *d)
{
        printf("\ntype 'help' to get a list of commands\n\n");
        char buf[BUF_SIZE];

        struct pollfd fds_[2];
        fds_[0] = (struct pollfd) {.fd = STDIN_FILENO, .events = POLLIN, .revents = 0};
        fds_[1] = (struct pollfd) {.fd = d->fd, .events = POLLIN, .revents = 0};
        
        int running = 1;
        while (running) {
                poll(fds_, 2, -1);

                if (!d->accept_command)
                        break;

                if (fds_[0].revents) {
                        out_request(d, buf);
                } else if (fds_[1].revents) {
                        if (in_request(d) == -1)
                                break;
                }
        }
}

int in_request(struct data *d)
{
        uint8_t header;
        int n = recv_all(d->fd, &header, 1);
        if (n == 0 || n == -1) {
                printf("connection lost\n");
                return -1;
        }

        switch (header) {
        case 0:
                print_message(d->fd);
                break;
        case 3:
                connection_found(d);
                break;
        case 4:
                break;
        case 7:
                recieve_id(d);
                break;
        default:
                printf("invalid command\n");
        }

        return 0;
}

void out_request(struct data *d, char buf[BUF_SIZE])
{
        fgets(buf, BUF_SIZE, stdin);
        char **s = split_string(buf);

        if (!strcmp("open\n", s[0])) {
                open_client(d);
        } else if (!strcmp("connect", s[0]) && s[1]) {
                connect_id(d->fd, s[1]);
        } else if (!strcmp("help\n", s[0])) {
                command_help();
        } else if (!strcmp("clear\n", s[0])) {
                system("clear");
                printf("\nID = %d\n\n", d->id);
                printf("type 'help' to get a list of commands\n\n");
        } else {
                printf("invalid command\n");
        }

        free(s);
}