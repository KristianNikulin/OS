#include "functions.cpp"

typedef struct args_thrd_
{
    void *socket;
    char login[SIZE];
} Args_thrd_t;

void *accept_message(Args_thrd_t *args)
{
    while (1)
    {
        MessageData buf;
        receive_struct(args->socket, &buf);
        if (!strcmp(buf.recipient_login, args->login))
        {
            cout << "message from " << buf.sender_login << ": " << buf.msg_txt << "\n";
            fflush(stdout);
            cout << args->login << " > ";
            fflush(stdout);
        }
    }
}

int main()
{
    // ZEROMQ

    void *context = zmq_ctx_new();

    void *requester = zmq_socket(context, ZMQ_PUSH); // отсылает
    char conn_push[] = "tcp://127.0.0.1:1234";
    int rc = zmq_connect(requester, conn_push);
    assert(rc == 0);

    void *subscriber = zmq_socket(context, ZMQ_SUB); // принимает
    char conn_sub[] = "tcp://127.0.0.1:4321";
    rc = zmq_connect(subscriber, conn_sub);
    assert(rc == 0);

    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    assert(rc == 0);

    //

    ifstream in;
    in.open("logins.txt");
    string s;
    char c;
    while (!in.eof())
    {
        in.get(c);
        s.push_back(c);
    }
    in.close();

    cout << "Insert your login:\n";
    char login[SIZE];
    while (1)
    {
        cin >> login;
        int pos = s.find(login);
        if (pos != -1)
        { // логин есть
            cout << "Welcome! You have signed as " << login;
            cout << ". Now you can send and recieve messages! ";
            cout << "\nTo search in message history insert: history <word>\n";
            fflush(stdout);
            break;
        }
        else
            cout << "Wrong login! Please, try again! Insert your login:\n";
        fflush(stdout);
    }

    MessageData buf;

    Args_thrd_t args;
    strcpy(args.login, login);
    args.socket = subscriber;
    thread thrd(accept_message, &args);

    while (1)
    {
        char rec[SIZE] = {};
        char mes[SIZE] = {};

        int pos;
        cout << login << " > ";
        fflush(stdout);
        strcpy(buf.sender_login, login);

        cin >> rec;
        strcpy(buf.recipient_login, rec);

        string mm = "";
        getline(cin, mm);
        copy(mm.begin() + 1, mm.end(), mes);
        mes[mm.size()] = 0;
        strcpy(buf.msg_txt, mes);

        pos = h.find(rec);
        if (pos != -1) // history
        {
            cout << "Check " << login << "'s history on server!\n";
            fflush(stdout);
            send_struct(requester, buf);
        }
        else
        {
            pos = s.find(rec);
            if (pos != -1) // логин есть
            {
                send_struct(requester, buf);
            }
            else
            {
                cout << "Invalid recipient user login!\n";
                fflush(stdout);
            }
        }
    }

    thrd.detach();
    zmq_close(requester);
    zmq_close(subscriber);
    zmq_term(context);
    return 0;
}
