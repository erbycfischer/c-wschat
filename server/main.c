#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdbool.h>

#include "chat_packets.h"

int main (int argc, char **argv) {

    fd_set set;
    const char *fifo = "/tmp/wspipeout.fifo";
    int fd = open(fifo, O_RDWR | O_NONBLOCK);
    printf("opened file\n");

    while(true) {
      chat_output_t *incoming = read_chat_packet(fd, set);
      printf("id: %d| wrote: %s\n", incoming->id, incoming->message);
      free(incoming);
    }

  return EXIT_SUCCESS;
}