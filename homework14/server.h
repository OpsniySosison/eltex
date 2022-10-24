
#ifndef COLORS
#define COLORS

#define RESET  "\033[0m"
#define RED    "\033[1;31;40m"
#define WHITE  "\033[1;37;40m"
#define GREEN  "\033[1;32;40m"
#define YELLOW "\033[1;33;40m"
#define PURPLE "\033[1;95;40m"

#define print_err_msg(fmt, ...) fprintf(stdout, "%s"fmt"%s\n", YELLOW, ##__VA_ARGS__, RESET)
#define print_srv_msg(fmt, ...) fprintf(stdout, "%s"fmt"%s\n", PURPLE, ##__VA_ARGS__, RESET)
#define print_clr_msg(fmt, color, ...) fprintf(stdout, "%s"fmt"%s\n", color, ##__VA_ARGS__, RESET)

#endif

#ifndef SERVER
#define SERVER

#define MSG_NOT_DEFINED -1

struct Room {
    struct Client* users[4];
    unsigned int user_count;
    unsigned int num_of_room;
    char name[32];
};

struct RoomList {
    struct Room* room;
    struct RoomList* next;
};

struct CallBackMsg {
    int num_room;
    char type;
    struct Client user;
};

struct PthreadArgs {
    struct RoomList* root;
    int shm_id;
};

int RoomListInit(struct RoomList** root);
int MsgHandler(void* root_arg);

#endif