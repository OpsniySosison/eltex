
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "client.h"
#include "server.h"

#define RT_R (*root)->room
#define TMP_USR_ARR temp->room->users[temp->room->user_count]
#define TMP_USR_CNT temp->room->user_count

int msg_sem;
int cb_sem;

int msg_shm;
int cb_shm;

static int SendCallBack(struct CallBackMsg* cb_msg) {
    if (!cb_msg) {
        print_err_msg("CallBack message not defined");
        return MSG_NOT_DEFINED;
    }
    print_clr_msg("Try to send CallBack to client (id %d, name %s)", WHITE, cb_msg->user.id, cb_msg->user.name);
    while (semctl(cb_sem, 0, GETVAL));
    semctl(cb_sem, 0, SETVAL, 1);
    if (cb_msg->type == LOGIN) {
        size_t len = lseek(cb_shm, 0, SEEK_END);
        if (write(cb_shm, cb_msg, sizeof(struct CallBackMsg)) < sizeof(struct CallBackMsg)) {
            print_err_msg("Write data of client (id %d, name %s) error", cb_msg->user.id, cb_msg->user.name);
        }
        else {
            print_clr_msg("Data of client (id %d, name %s) succesfully send to client", WHITE, cb_msg->user.id, cb_msg->user.name);
        }
        lseek(cb_shm, len, SEEK_SET);
    }
    free(cb_msg);
    return 0;
}

static int AddUser(struct Message* log_msg, struct RoomList* root, struct CallBackMsg* cb_msg) {
    struct RoomList* temp = root;
    static unsigned int user_id = 1;
    static unsigned int room_num = 0;
    while (temp->next) {
        if (TMP_USR_CNT < 4) {
            TMP_USR_ARR = (struct Client*)malloc(sizeof(struct Client));
            TMP_USR_ARR->id = user_id;
            if (!strcpy(TMP_USR_ARR->name, log_msg->user.name)) {
                print_err_msg("strcpy failed");
                free(TMP_USR_ARR);
                return 0;
            }
            TMP_USR_CNT++;
            user_id++;
            cb_msg->num_room = temp->room->num_of_room;
            return user_id - 1;
        }
        temp = temp->next;
    }
    if (temp->room->user_count < 4) {
        TMP_USR_ARR = (struct Client*)malloc(sizeof(struct Client));
        TMP_USR_ARR->id = user_id;
        if (!strcpy(TMP_USR_ARR->name, log_msg->user.name)) {
            print_err_msg("strcpy failed");
            free(TMP_USR_ARR);
            return 0;
        }
        TMP_USR_CNT++;
        user_id++;
        cb_msg->num_room = temp->room->num_of_room;
        return user_id - 1;
    }
    temp->next = (struct RoomList*)malloc(sizeof(struct RoomList));
    temp = temp->next;
    temp->room = (struct Room*)malloc(sizeof(struct Room));
    TMP_USR_CNT = 0;
    TMP_USR_ARR = (struct Client*)malloc(sizeof(struct Client));
    TMP_USR_ARR->id = user_id;
    if (!strcpy(TMP_USR_ARR->name, log_msg->user.name)) {
        print_err_msg("strcpy failed");
        free(TMP_USR_ARR);
        return 0;
    }
    TMP_USR_CNT++;
    temp->room->num_of_room = room_num;
    cb_msg->num_room = temp->room->num_of_room;
    room_num++;
    user_id++;
    return user_id - 1;
}

static int MsgAnalysis(struct Message* msg, struct RoomList* root) {
    pthread_t pt_cb_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (msg->priority == LARGE) {
        if (msg->type_msg == LOGIN) {
            struct CallBackMsg* cb_msg = (struct CallBackMsg*)malloc(sizeof(struct CallBackMsg));
            print_srv_msg("Client (name %s) try connect to server", msg->user.name);
            unsigned int user_id = AddUser(msg, root, cb_msg);
            if (!user_id) {
                print_err_msg("Failed to connected user (name %s)", msg->user.name);
                return MSG_NOT_DEFINED;
            }
            print_srv_msg("The client named '%s' successfully connected to the server, now it has the id %d", msg->user.name, user_id);
            cb_msg->user.id = user_id;
            cb_msg->type = LOGIN;
            strcpy(cb_msg->user.name, msg->user.name);
            pthread_create(&pt_cb_id, &attr, SendCallBack, (void*)cb_msg);
        }
        else if (msg->type_msg == EXIT) {
            print_srv_msg("Client disconnect from server");
        }
    }
    else if (msg->priority == NORMAL) {
        print_srv_msg("Client (id %d, name %s) send message '%s'", msg->user.id, msg->user.name, msg->msg);
    }
    else if (msg->priority == LOW) {
    
    }
    else {
        print_clr_msg("THE MESSAGE (user id %d, username %s) HAS NO PRIORITY, MAYBE SOMEBODY TRY TO HACK THE SERVER!!! RETURNED", RED, msg->user.id, msg->user.name);
        return MSG_NOT_DEFINED;
    }
    
    return 0;
}

int MsgHandler(void* root_arg) {
    struct RoomList* root = (struct RoomList*)root_arg;
    struct Message msg;
    while (1) {
        int result = read(msg_shm, &msg, sizeof(struct Message));
        if (result == -1 || result == 0) {
            sleep(2);
            continue;
        }
        MsgAnalysis(&msg, root);
    }
}

int RoomListInit(struct RoomList** root) {
    *root = (struct RoomList*)malloc(sizeof(struct RoomList));
    RT_R = (struct Room*)malloc(sizeof(struct Room));
    for (unsigned int i = 0; i < 4; i++) {
        RT_R->users[i] = NULL;
    }
    RT_R->user_count = 0;
    RT_R->num_of_room = 1;
    if (!strcpy(RT_R->name, "Room")) {
        print_err_msg("strcpy failed");
        free(RT_R);
        free(*root);
        return 1;
    }
    return 0;
}
