
#ifndef CLIENT
#define CLIENT

struct Client {
    unsigned int id;
    char name[16];
    char msg[64];
};

#endif