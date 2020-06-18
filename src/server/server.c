#include "server/server.h"

#define BUF_SIZE 512

void run_server(int sockfd, char *ip, uint16_t port)
{
        list_t *list = list_init();
        struct meta meta = {.ip = ip, .port = port, .list = list};

        pthread_mutex_t *mutex = malloc(sizeof *mutex);
        pthread_mutex_init(mutex, NULL);
        
        int cur_id = 0;
        struct pollfd fds[2];
        fds[0] = (struct pollfd) {.fd = STDIN_FILENO, .events = POLLIN, .revents = 0};
        fds[1] = (struct pollfd) {.fd = sockfd, .events = POLLIN, .revents = 0};

        struct client *cl;

        printf("type 'help' to get a list of commands\n\n");

        while (1) {
                if (poll(fds, 2, -1) == -1) {
                        perror("run_server poll error");
                        break;
                }

                if (fds[0].revents) {
                        user_request(&meta);
                } else if (fds[1].revents && (cl = incoming_client_connection(sockfd, cur_id + 1, list))) {
                        pthread_create(&cl->thread, NULL, client_server_talk, cl);
                        cl->list = list;
                        cl->mutex = mutex;
                        cur_id++;
                }
        }

        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
        pthread_mutex_destroy(mutex);
        free(mutex);
}

struct client *incoming_client_connection(int sockfd, int cur_id, list_t *list)
{
        struct client *cl = accept_client_connection(sockfd);

        if (cl == NULL) {
                perror("run_server accept run_client error");
                return NULL;
        }

        list_add(list, cl);

        cl->id = cur_id;
        printf("\nnew client accepted, ID = %d\n", cl->id);

        uint8_t header = 0;
        char buf[BUF_SIZE];
        sprintf(buf, "\nWelcome to the server!\n\nID = %d\n", cl->id);

        send_all(cl->fd, &header, 1);
        send_all(cl->fd, buf, BUF_SIZE);

        header = 7;
        sprintf(buf, "%d", cl->id);

        send_all(cl->fd, &header, 1);
        send_all(cl->fd, buf, BUF_SIZE);
        return cl;
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