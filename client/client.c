#include "client.h"
#define BUF_SIZE 512

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
        *d = (struct data) {.fd = fd, .status = 0, .s = NULL};

        pthread_create(&d->in_thread, NULL, incomming_commands, d);
        pthread_create(&d->out_thread, NULL, outgoing_commands, d);

        pthread_join(d->in_thread, NULL);
        pthread_join(d->out_thread, NULL);
        pthread_join(d->comm_thread, NULL);

        return 0;
}

int config_client(uint16_t port_number, char *ip_addr)
{
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof serv_addr);

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = port_number;
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

char **split_string(char *str)
{
        int len = 2;
        char **s = malloc(len * sizeof *s);

        s[0] = strtok(str, " ");

        for (int i = 1; i < len; i++)
                s[i] = strtok(NULL, " ");

        return s;
}

void print_message(int fd)
{
        uint8_t buf[BUF_SIZE];
        recv_all(fd, buf, BUF_SIZE);
        printf("%s", buf);
}

void connect_id(int fd, char *id_str)
{
        char buf[BUF_SIZE];
        strcpy(buf, id_str);
        uint8_t header = 2;
        send_all(fd, &header, 1);
        send_all(fd, buf, BUF_SIZE);
}

void open_client(struct data *d)
{
        printf("\nclient open for connections\n\n");
        d->status = 1;

        uint8_t header = 1;
        send_all(d->fd, &header, 1);
}

void connection_found(struct data *d)
{
        uint8_t header = (d->status == 1) ? 6 : 5;
        send_all(d->fd, &header, 1);

        d->status = 4;

        pthread_create(&d->comm_thread, NULL, start_comm, d);
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

void recieve_id(struct data *d)
{
        char buf[BUF_SIZE];
        recv_all(d->fd, buf, BUF_SIZE);
        d->id = atoi(buf);
}

void *outgoing_commands(void *data_ptr)
{
        struct data *d = data_ptr;
        char buf[BUF_SIZE];

        printf("\ntype 'help' to get a list of commands\n\n");

        while (1) {

                printf("> ");

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

        return NULL;
}

void *incomming_commands(void *data_ptr)
{
        struct data *d = data_ptr;

        struct pollfd fds = {.fd = d->fd, .events = POLLIN, .revents = 0};
        uint8_t header;
        int running = 1;

        int n;
        while (running) {
                poll(&fds, 1, -1);

                n = recv_all(d->fd, &header, 1);
                if (n == 0 || n == -1) {
                        printf("connection lost\n");
                        break;
                }

                switch (header) {
                case 0:
                        print_message(d->fd);
                        break;
                case 3:
                        connection_found(d);
                        break;
                case 7:
                        recieve_id(d);
                        break;
                default:
                        printf("invalid command\n");
                }
        }

        return NULL;
}

void *start_comm(void *d_void)
{
        struct data *d = d_void;

        pthread_t thread_incoming;
        pthread_t thread_outgoing;

        printf("Connection successful!\n\n");

        pthread_create(&thread_incoming, NULL, incoming, &d->fd);
        pthread_create(&thread_outgoing, NULL, outgoing, &d->fd);

        pthread_cancel(d->in_thread);
        pthread_cancel(d->out_thread);

        pthread_join(thread_incoming, NULL);
        pthread_cancel(thread_outgoing);

        shutdown(d->fd, SHUT_RDWR);
        close(d->fd);

        return NULL;
}

void *incoming(void *fd_ptr)
{
        int fd = *(int*) fd_ptr;
        struct pollfd fds = {.fd = fd, .events = POLLIN, .revents = 0};
        char buf[BUF_SIZE];

        while (1) {
                if (poll(&fds, 1, -1) == -1) {
                        perror("client incoming poll");
                        break;
                }

                int n = recv_all(fd, buf, BUF_SIZE);
                if (n == -1 || n == 0) {
                        n ? perror("client") : printf("lost connection\n");
                        return NULL;
                }

                printf("%s", buf);
        }

        return NULL;
}

void *outgoing(void *fd_ptr)
{
        int fd = *(int*) fd_ptr;
        char buf[BUF_SIZE];

        while (1) {
                fgets(buf, BUF_SIZE, stdin);
                send_all(fd, buf, BUF_SIZE);
        }

        return NULL;
}