
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
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

    char* udp_msg = (char*)malloc(sizeof(struct udphdr) + 20);

    ((struct udphdr*)udp_msg)->source = htons(THIS_PORT);
    ((struct udphdr*)udp_msg)->dest = htons(SERV_PORT);
    ((struct udphdr*)udp_msg)->len = htons(sizeof(struct udphdr) + 20);
    ((struct udphdr*)udp_msg)->check = htons(0);
    strcpy((udp_msg + sizeof(struct udphdr)), "privet");

    char* buff = (char*)malloc(sizeof(struct udphdr) + 42);

    socklen_t s_size = sizeof(server);

    if (sendto(raw_sock, (void*)udp_msg, sizeof(struct udphdr) + 20, 0, (struct sockaddr*)&server, s_size) < 0) {
        fprintf(stderr, "sendto error\n");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        if (recvfrom(raw_sock, (void*)buff, sizeof(struct udphdr) + 42, 0, (struct sockaddr*)&server, &s_size) < 0) {
            fprintf(stderr, "recvfrom error\n");
        }
        char* buff_ptr = buff + 20;
        fprintf(stdout, "\n--package--\nsrc_port %d\ndestin_port %d\nlen %d\ncheck_sum %d\nmsg: %s\n\n", 
        ntohs(((struct udphdr*)buff_ptr)->source), 
        ntohs(((struct udphdr*)buff_ptr)->dest), 
        ntohs(((struct udphdr*)buff_ptr)->len), 
        ntohs(((struct udphdr*)buff_ptr)->check), 
        (buff_ptr + sizeof(struct udphdr)));
    }

    exit(EXIT_SUCCESS);
}