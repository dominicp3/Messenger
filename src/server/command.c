#include "server/command.h"
#define BUF_SIZE 512

void clients_command(struct meta *m)
{
        lst_print(m->list);
}

void clear_command(struct meta *m)
{
        system("clear");
        printf("\nIP Address : %s\n", m->ip);
        printf("Port Number: %d\n\n", m->port);
        printf("type 'help' to get a list of diagnostic commands\n\n");
}

void help_command(void)
{
        printf("\nCOMMANDS:\n");
        printf("clients: list tracked clients\n");
        printf("clear: clear screen\n");
        printf("\n");
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
        printf("client #%d set to open\n\n", cl->id);
        cl->status = 1;
}

void connect_id(int fd, struct client **peer_ptr, list_t *list)
{
        char buf[BUF_SIZE];
        recv_all(fd, buf, BUF_SIZE);

        int id = atoi(buf);
        struct client *peer = lst_get(lst_find(list, &id));

        uint8_t header;
        if (peer && peer->status != 1) {
                header = 0;
                sprintf(buf, "client #%d not open for connections\n\n", id);
                send_all(fd, &header, 1);
                send_all(fd, buf, BUF_SIZE);
                return;
        }

        if (peer == NULL) {
                header = 0;
                sprintf(buf, "client with ID %d does not exists\n", id);
                send_all(fd, &header, 1);
                send_all(fd, buf, BUF_SIZE);
        }

        header = peer ? 3 : 4;
        send_all(fd, &header, 1);
        if (peer)
                send_all(peer->fd, &header, 1);

        *peer_ptr = peer;
}

void ready_connection(struct client *cl, struct client *peer)
{
        int fd[2];
        if (pipe(fd) == -1) {
                perror("ready_connection pipe");
                return;
        }

        if (!fork()) {
                const char *path = "./bin/talk";

                char fd1[10];
                sprintf(fd1, "%d", cl->fd);

                char fd2[10];
                sprintf(fd2, "%d", peer->fd);

                char w[10];
                sprintf(w, "%d", fd[1]);
                close(fd[0]);

                execl(path, path, fd1, fd2, w, NULL);
        }

        cl->status = 4;
        peer->status = 4;

        uint8_t wait;
        close(fd[1]);
        read(fd[0], &wait, 1);
        close(fd[0]);

        pthread_cond_signal(&peer->cond);
}
