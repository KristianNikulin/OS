#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <fstream>

#include "zmq.hpp"

using namespace std;

#define SIZE 128
#define HISTORY_SIZE 30

typedef struct message_
{
    char sender_login[SIZE];
    char recipient_login[SIZE];
    char msg_txt[SIZE];
} MessageData;

string h = "history-";
string a = "ALL-";

int in(vector<string> logins, string str)
{
    for (int i = 0; i < logins.size(); ++i)
    {
        if (logins[i] == str)
            return i;
    }
    return -1;
}

void receive_struct(void *socket, MessageData *md)
{
    zmq_recv(socket, md->sender_login, sizeof(md->sender_login), 0);
    zmq_recv(socket, md->recipient_login, sizeof(md->recipient_login), 0);
    zmq_recv(socket, md->msg_txt, sizeof(md->msg_txt), 0);
}

void send_struct(void *socket, MessageData md)
{
    zmq_send(socket, md.sender_login, sizeof(md.sender_login), 0);
    zmq_send(socket, md.recipient_login, sizeof(md.recipient_login), 0);
    zmq_send(socket, md.msg_txt, sizeof(md.msg_txt), 0);
}
