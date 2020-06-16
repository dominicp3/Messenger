
#include "diagnostic.h"

#define BUF_SIZE 32

void *diagnostic(void *d_void)
{
        struct diag *d = d_void;

        printf("type 'help' to get a list of diagnostic commands\n\n");
        char buff[BUF_SIZE];
        while (1) {
                printf("> ");

                fgets(buff, BUF_SIZE, stdin);

                if (!strcmp("help\n", buff))
                        help_command(); else
                
                if (!strcmp("clients\n", buff))
                        clients_command(d); else
                
                if (!strcmp("clear\n", buff))
                        clear_command(d); else
                
                printf("command not found\n\n");
        }

        return NULL;
}

void clients_command(struct diag *d)
{
        list_print(d->list);
}

void clear_command(struct diag *d)
{
        system("clear");
        printf("\nIP Address : %s\n", d->ip);
        printf("Port Number: %d\n\n", d->port);
        printf("type 'help' to get a list of diagnostic commands\n\n");
}

void help_command(void)
{
        printf("\nCOMMANDS:\n");
        printf("clients: list tracked clients\n");
        printf("clear: clear screen\n");
        printf("\n");
}