#include "talk.h"

#define BUFF_SIZE 512

int main(int argc, char **argv)
{
        if (argc != 3)
                return 0;

        int fd1 = atoi(argv[1]);
        int fd2 = atoi(argv[2]);

        setup_client_comm(fd1, fd2);
        return 0;
}

void setup_client_comm(int fd1, int fd2)
{
        pthread_t thread_incoming;
        pthread_t thread_outgoing;

        struct client_fds c1_to_c2 = {.fd1 = fd1, .fd2 = fd2};
        pthread_create(&thread_incoming, NULL, cl2cl, &c1_to_c2);

        struct client_fds c2_to_c1 = {.fd1 = fd2, .fd2 = fd1};
        pthread_create(&thread_outgoing, NULL, cl2cl, &c2_to_c1);

        pthread_join(thread_incoming, NULL);
        pthread_join(thread_outgoing, NULL);

        shutdown(fd1, SHUT_RDWR);
        close(fd1);

        shutdown(fd2, SHUT_RDWR);
        close(fd2);
}

void *cl2cl(void *cl_fds_ptr)
{
        int fd1 = ((struct client_fds*) cl_fds_ptr)->fd1;
        int fd2 = ((struct client_fds*) cl_fds_ptr)->fd2;

        struct pollfd fds = {.fd = fd1, .events = POLLIN, .revents = 0};

        char buff[BUFF_SIZE];
        while (1) {
                if (poll(&fds, 1, -1) == -1) {
                        perror("incoming poll");
                        break;
                }

                if (fds.revents == 0)
                        continue;

                fds.revents = 0;
                if (recv_all(fd1, buff, BUFF_SIZE) == 0) {
                        printf("lost connection\n");
                        return NULL;
                }
                send_all(fd2, buff, BUFF_SIZE);
        }

        return NULL;
}