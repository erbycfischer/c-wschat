#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>

#define MESSAGE_TYPE_BINARY 0x2
#define MESSAGE_TYPE_TEXT 0x1

typedef uint32_t client_id_t;

typedef struct {
    client_id_t client_id;
    uint32_t message_type;
    uint32_t payload_size;
    char *payload;
} chat_packet_t;

typedef struct {
    client_id_t id;
    char *message;
} chat_output_t;

static size_t unpack_uint32 (const void *b, uint32_t * val);
chat_packet_t* create_chat_packet(client_id_t id, uint32_t type, uint32_t payload_size, char *msg);
int write_chat_packet(chat_packet_t* packet);
chat_output_t* read_chat_packet(int fd, fd_set set);