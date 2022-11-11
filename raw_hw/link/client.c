
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _GNU_SOURCE

#include <unistd.h>

#include <sys/socket.h>
#include <net/ethernet.h>
#include <netinet/in.h> // for to htons
#include <net/if.h>
#include <arpa/inet.h>

#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/if_packet.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>

#define THIS_PORT 52134
#define SERV_PORT 51234
#define DATA_PACK_LEN 20

static int IpCheckSum(char* ip_hdr_ptr) {
    short* ptr = (short*)ip_hdr_ptr;

    for (short* i = (short*)ip_hdr_ptr; i < (short*)ip_hdr_ptr + 10; i++) {
        ((struct iphdr*)ip_hdr_ptr)->check += *i;
    }

    int tmp = ((struct iphdr*)ip_hdr_ptr)->check >> 16;
    ((struct iphdr*)ip_hdr_ptr)->check = (((struct iphdr*)ip_hdr_ptr)->check & 0xffff) + tmp;
    ((struct iphdr*)ip_hdr_ptr)->check = ~(((struct iphdr*)ip_hdr_ptr)->check);
}

int main(void) {

    int raw_sock_eth = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (raw_sock_eth < 0) {
        fprintf(stderr, "socket is broken\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_ll server;

    unsigned char src_mac[6] = {0xe0, 0x2b, 0xe9, 0xcb, 0x37, 0x07};
    unsigned char dst_mac[6] = {0x00, 0x18, 0xf3, 0x05, 0x2f, 0x44};

    char msg_buff[DATA_PACK_LEN] = "privet";

    server.sll_family = AF_PACKET;
    server.sll_ifindex = if_nametoindex("wlo1");
    server.sll_halen = htons(6);
    strncpy(server.sll_addr, dst_mac, 6);

    socklen_t server_len = sizeof(server);

    size_t msg_size = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + DATA_PACK_LEN;

    char* msg = (char*)malloc(msg_size);
    char* callback = (char*)malloc(msg_size);

    strncpy(((struct ethhdr*)msg)->h_source, src_mac, 6);
    strncpy(((struct ethhdr*)msg)->h_dest, dst_mac, 6);
    ((struct ethhdr*)msg)->h_proto = htons(0x08);

    msg += sizeof(struct ethhdr);

    ((struct iphdr*)msg)->version = htons(0x29);
    ((struct iphdr*)msg)->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + DATA_PACK_LEN);
    ((struct iphdr*)msg)->id = htons(0);
    ((struct iphdr*)msg)->frag_off = htons(0x40);
    ((struct iphdr*)msg)->ttl = htons(64);
    ((struct iphdr*)msg)->protocol = htons(0x11);
    ((struct iphdr*)msg)->saddr = inet_addr("192.168.0.12");
    ((struct iphdr*)msg)->daddr = inet_addr("100.86.41.10");
    ((struct iphdr*)msg)->tos = htons(0);
    IpCheckSum(msg);

    msg += sizeof(struct iphdr);

    ((struct udphdr*)msg)->source = htons(THIS_PORT);
    ((struct udphdr*)msg)->dest = htons(SERV_PORT);
    ((struct udphdr*)msg)->check = htons(0);
    ((struct udphdr*)msg)->len = htons(sizeof(struct udphdr) + DATA_PACK_LEN);

    if (!strncpy(msg + sizeof(struct udphdr), msg_buff, DATA_PACK_LEN)) {
        fprintf(stderr, "buffer full error\n");
        exit(EXIT_FAILURE);
    }

    if (sendto(raw_sock_eth, (void*)msg, msg_size, 0, (struct sockaddr*)&server, server_len) < 0) {
        fprintf(stderr, "sendto() error, can you get broken socket or server not found\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (recvfrom(raw_sock_eth, (void*)callback, msg_size, 0, (struct sockaddr*)&server, &server_len) < 0) {
            fprintf(stderr, "recvfrom() error, can you get broken socket or server not found\n");
        }
        fprintf(stdout, "\n--package--\nmsg: %s\n", callback + msg_size - DATA_PACK_LEN);
    }

    free(msg);
    free(callback);
    close(raw_sock_eth);

    exit(EXIT_SUCCESS);
}