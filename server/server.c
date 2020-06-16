#include "server.h"

#define BUF_SIZE 512

list_t *list = NULL;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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

        list = list_init();

        struct diag d;
        strcpy(d.ip, argv[1]);
        d.port = port;
        d.list = list;

        pthread_t diag_thread;
        pthread_create(&diag_thread, NULL, diagnostic, &d);

        run_server(config_server(port, ip));

        pthread_cancel(diag_thread);
        list_destroy(list);
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

void run_server(int sockfd)
{
        char buf[BUF_SIZE];
        int cur_id = 0;
        struct pollfd fds = {.fd = sockfd, .events = POLLIN, .revents = 0};

        while (1) {
                if (poll(&fds, 1, -1) == -1) {
                        perror("run_server poll error");
                        break;
                }

                struct client *cl = accept_client_connection(sockfd);

                if (cl == NULL) {
                        perror("run_server accept run_client error");
                } else {

                        list_add(list, cl);

                        cl->id = cur_id++;
                        printf("\nnew client accepted, ID = %d\n", cl->id);

                        uint8_t header = 0;
                        sprintf(buf, "\nWelcome to the server!\n\nID = %d\n", cl->id);

                        send_all(cl->fd, &header, 1);
                        send_all(cl->fd, buf, BUF_SIZE);

                        header = 7;
                        sprintf(buf, "%d", cl->id);

                        send_all(cl->fd, &header, 1);
                        send_all(cl->fd, buf, BUF_SIZE);

                        pthread_create(&cl->thread, NULL, client_server_talk, cl);
                }
        }
}

char **split_string(char *str)
{
        char **s = malloc(3 * sizeof *s);
        s[0] = strtok(str, " ");
        s[1] = strtok(NULL, " ");
        s[2] = strtok(NULL, " ");
        return s;
}

void print_message(int fd)
{
        uint8_t buf[BUF_SIZE];
        recv_all(fd, buf, BUF_SIZE);
        printf("%s", buf);
}

void status_to_open(struct client *cl)
{
        printf("\nclient #%d set to open\n\n", cl->id);
        cl->status = 1;
}

void connect_id(int fd, struct client **peer_ptr)
{
        uint8_t buf[BUF_SIZE];
        recv_all(fd, buf, BUF_SIZE);

        int id = atoi((char*) buf);
        struct client *peer = list_find(list, id);

        if (peer) {
                printf("found peer with ID %d\n", id);
        } else {
                printf("did not find peer with ID %d\n", id);
        }

        uint8_t header = peer ? 3 : 4;
        send_all(fd, &header, 1);
        if (peer)
                send_all(peer->fd, &header, 1);
        *peer_ptr = peer;
}

void ready_connection(struct client *cl, struct client *peer)
{
        cl->status = 4;
        peer->status = 4;

        if (fork() == 0) {
                const char *path = "./bin/talk";

                char fd1[10];
                sprintf(fd1, "%d", cl->fd);

                char fd2[10];
                sprintf(fd2, "%d", peer->fd);

                execl(path, path, fd1, fd2, NULL);
        }

        pthread_cond_signal(&peer->cond);
}

void *client_server_talk(void *cl_void)
{
        struct client *cl = cl_void;
        struct pollfd fds = {.fd = cl->fd, .events = POLLIN, .revents = 0};

        struct client *peer = NULL;

        uint8_t header;

        int running = 1;

        int n;
        while (running) {
                poll(&fds, 1, -1);

                n = recv_all(cl->fd, &header, 1);
                if (n == -1 || n == 0) {
                        n ? perror("client_server") : printf("connection lost\n");
                        break;
                }

                pthread_mutex_lock(&mutex);

                switch (header) {
                case 0:
                        print_message(cl->fd);
                        break;
                case 1:
                        status_to_open(cl);
                        break;
                case 2:
                        connect_id(cl->fd, &peer);
                        break;
                case 5:
                        ready_connection(cl, peer);
                        running = 0;
                        break;
                case 6:
                        while (cl->status != 4)
                                pthread_cond_wait(&cl->cond, &mutex);
                        running = 0;
                        break;
                default:
                        printf("invalid command\n");
                }

                pthread_mutex_unlock(&mutex);
        }

        list_remove(list, cl);
        return NULL;
}

struct client *try_client_comm(struct client *client)
{
        uint8_t id;
        if (recv_all(client->fd, &id, 1) == 0)
                return NULL;

        struct client *cl_other = list_find(list, id);

        if (cl_other && cl_other->status != 1)
                cl_other = NULL;

        if (cl_other) {
                cl_other->status = 2;
                client->status = 2;
        }

        uint8_t found_client = (cl_other == NULL) ? 0 : 1;
        send_all(client->fd, &found_client, 1);
        return cl_other;
}

struct client *accept_client_connection(int sockfd)
{
        struct sockaddr_in client_name;
        socklen_t clen = sizeof(struct sockaddr_in);

        int client_fd = accept(sockfd, (struct sockaddr*) &client_name, &clen);

        if (client_fd  == -1)
                return NULL;

        struct client *new_client = malloc(sizeof *new_client);
        new_client->fd = client_fd;
        new_client->ip = client_name.sin_addr.s_addr;
        new_client->status = 0;

        pthread_cond_init(&new_client->cond, NULL);

        new_client->next = NULL;
        new_client->prev = NULL;

        return new_client;
}