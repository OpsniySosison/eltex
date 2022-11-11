
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define _GNU_SOURCE

#include <unistd.h>
#include <linux/ip.h>
#include <linux/udp.h>

#define THIS_PORT 52134
#define SERV_PORT 51234

int main(void) {
    
    int raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_sock < 0) {
        fprintf(stderr, "raw sock not created, try to use sudo main\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(SERV_PORT);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    socklen_t s_size = sizeof(server);

    struct iphdr ip_msg_hdr;

    char msg_buff[20] = "privet";

    char* packed = (char*)malloc(sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(msg_buff));
    char* callback_buff = (char*)malloc(sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(msg_buff));

    if (!packed || !callback_buff) {
        fprintf(stderr, "memory allocation error, packed was %p, callback_buff was %p", packed, callback_buff);
        if (packed) free(packed);
        if (callback_buff) free(callback_buff);
        exit(EXIT_FAILURE);
    }

    ((struct iphdr*)packed)->version = 0b01000101;
    ((struct iphdr*)packed)->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(msg_buff));
    ((struct iphdr*)packed)->id = htons(0);
    ((struct iphdr*)packed)->frag_off = htons(0x40);
    ((struct iphdr*)packed)->ttl = htons(64);
    ((struct iphdr*)packed)->protocol = htons(0x11);
    ((struct iphdr*)packed)->check = htons(0);
    ((struct iphdr*)packed)->saddr = htonl(INADDR_LOOPBACK);
    ((struct iphdr*)packed)->daddr = htonl(INADDR_LOOPBACK);
    ((struct iphdr*)packed)->tos = htons(0);

    packed += sizeof(struct iphdr);

    ((struct udphdr*)packed)->source = htons(THIS_PORT);
    ((struct udphdr*)packed)->dest = htons(SERV_PORT);
    ((struct udphdr*)packed)->len = htons(sizeof(struct udphdr) + sizeof(msg_buff));
    ((struct udphdr*)packed)->check = htons(0);

    if (!strncpy(packed + sizeof(struct udphdr), msg_buff, 20)) {
        fprintf(stderr, "buffer full error\n");
        exit(EXIT_FAILURE);
    }
 
    if (sendto(raw_sock, (void*)packed, sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(msg_buff), 0, (struct sockaddr*)&server, s_size) < 0) {
        fprintf(stderr, "sendto error\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        if (recvfrom(raw_sock, (void*)callback_buff, sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(msg_buff), 0, (struct sockaddr*)&server, &s_size) < 0) {
            fprintf(stderr, "recvfrom error\n");
        }
        fprintf(stdout, "--package--\n");
        fprintf(stdout, "%s\n", callback_buff + sizeof(struct iphdr) + sizeof(struct udphdr));
    }

    free(packed);
    free(callback_buff);
    close(raw_sock);

    exit(EXIT_SUCCESS);
}