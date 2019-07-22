#include <stdio.h>

#include "chat_packets.h"

static size_t unpack_uint32 (const void *b, uint32_t * val) {
  uint32_t v32 = 0;
  memcpy (&v32, b, sizeof (uint32_t));
  *val = ntohl (v32);
  return sizeof (uint32_t);
}

chat_packet_t* create_chat_packet(client_id_t id, uint32_t type, uint32_t payload_size, char *msg) {
    chat_packet_t *tmp = (chat_packet_t *) calloc(sizeof(sizeof(uint32_t) * 3 + strlen(msg)), sizeof(char));
    tmp->client_id = htonl(id);
    tmp->message_type = htonl(type);
    tmp->payload_size = htonl(payload_size);
    tmp->payload = msg;

    return tmp;
}

int write_chat_packet(chat_packet_t* packet) {
    const char *fifo = "/tmp/wspipein.fifo";
    int fd = open(fifo, O_WRONLY);
    int err = write(fd, packet, sizeof(packet));
    close(fd);
    free(packet);
    return err > 0;
}

static void read_message (int fd, fd_set set, chat_output_t *loader) {
  uint32_t size = 0, listener = 0, type = 0;
  char hdr[PIPE_BUF] = { 0 }, buf[PIPE_BUF] = { 0 };
  char *ptr = NULL;

  FD_ZERO (&set);
  FD_SET (fd, &set);

  if ((select (fd + 1, &set, NULL, NULL, NULL)) < 1)
    exit (1);
  if (!FD_ISSET (fd, &set))
    return;

  if (hdr[0] == '\0') {
    if (read (fd, hdr, sizeof(uint32_t) * 3) < 1)
      return;
  }

  ptr = hdr;
  ptr += unpack_uint32(ptr, &listener);
  ptr += unpack_uint32(ptr, &type);
  ptr += unpack_uint32(ptr, &size);

    printf("bout to read message\n");
  if (read (fd, buf, size) < 1) {
    return;
  }

  printf("read message\n");
  loader->id = listener;
  loader->message = (char *)malloc(strlen(buf) + 1);
  strcpy(loader->message, buf);

  return;
}


chat_output_t* read_chat_packet(int fd, fd_set set) {
    
    chat_output_t* output = malloc(1024 * 4);
    printf("reading packet\n");
    read_message(fd, set, output);

    return output;
}