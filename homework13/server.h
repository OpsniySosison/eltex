
#include "client.h"

#ifndef SERVER
#define SERVER

struct Room {
    unsigned int id;
    unsigned int count;
    struct Client clients[8];
};

struct RoomList {
    struct Room* room;
    struct RoomList* next;
};

#endif