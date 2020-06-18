#include "server/request.h"
#define BUF_SIZE 512

void user_request(struct meta *m)
{
        char buf[BUF_SIZE];

        fgets(buf, BUF_SIZE, stdin);

        if (!strcmp("help\n", buf))
                help_command();      else

        if (!strcmp("clients\n", buf))
                clients_command(m);  else

        if (!strcmp("clear\n", buf))
                clear_command(m);    else

        if (!strcmp("\n", buf))
                return;              else

        printf("command not found\n\n");
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
                        shutdown(cl->fd, SHUT_RDWR);
                        break;
                }

                pthread_mutex_lock(cl->mutex);

                switch (header) {
                case 0:
                        print_message(cl->fd);
                        break;
                case 1:
                        status_to_open(cl);
                        break;
                case 2:
                        connect_id(cl->fd, &peer, cl->list);
                        break;
                case 5:
                        ready_connection(cl, peer);
                        running = 0;
                        break;
                case 6:
                        while (cl->status != 4)
                                pthread_cond_wait(&cl->cond, cl->mutex);
                        running = 0;
                        break;
                default:
                        printf("invalid command\n");
                }

                pthread_mutex_unlock(cl->mutex);
        }

        close(cl->fd);
        list_remove(cl->list, cl);
        return NULL;
}
