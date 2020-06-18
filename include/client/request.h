#ifndef REQUEST_H
#define REQUEST_H

#include "struct.h"
#include "command.h"
#include "sendrecv/sendrecv.h"

void request(struct data *d);
int in_request(struct data *d);
void out_request(struct data *d, char *buf);

#endif /* REQUEST_H */