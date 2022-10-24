
#ifndef COLORS
#define COLORS

#define RESET  "\033[0m"
#define RED    "\033[1;31;40m"
#define WHITE  "\033[1;37;40m"
#define GREEN  "\033[1;32;40m"
#define YELLOW "\033[1;33;40m"
#define PURPLE "\033[1;95;40m"

#define print_err_msg(fmt, ...) fprintf(stdout, "%s"fmt"%s\n", RED, ##__VA_ARGS__, RESET)
#define print_srv_msg(fmt, ...) fprintf(stdout, "%s"fmt"%s\n", PURPLE, ##__VA_ARGS__, RESET)
#define print_clr_msg(fmt, color, ...) fprintf(stdout, "%s"fmt"%s\n", color, ##__VA_ARGS__, RESET)

#endif

#ifndef CALLMSGS
#define CALLMSGS 

#define LOW 1
#define NORMAL 2
#define LARGE 3

#define LOGIN 0x01
#define EXIT 0x02
#define COMMUN 0x03

#endif

#ifndef SEM
#define SEM

#define MSG_SEM_ID 1396
#define CB_SEM_ID 1397

#endif

#ifndef CLIENT
#define CLIENT

struct Client {
    char name[16];
    unsigned int id;
};

struct Message {
    struct Client user;
    unsigned short int priority;
    char type_msg;
    char msg[64];
};

struct PthreadClientArgs {
    struct Client user;
    int shm_id;
};

char* sgets(char* buff, unsigned int size);
int MsgClientHandler(struct PthreadClientArgs* args);

#endif