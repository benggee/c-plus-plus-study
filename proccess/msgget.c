//
// Created by HEADS on 2021/2/19.
//
// 可以使用 ipcs -q 查看消息队列对象

#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>

int main() {
    int messagequeueid;
    key_t key;

    if ((key = ftok("/Users/HEADS/messagequeue/messagequeuekey", 1024)) < 0) {
        perror("ftok error");
        return -1;
    }

    printf("Message Queue key: %d.\n", key);

    if ((messagequeueid = msgget(key, IPC_CREAT|0777))==-1) {
        perror("msgget error");
        return -1;
    }

    printf("Message queue id: %d.\n", messagequeueid);
}