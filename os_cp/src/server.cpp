#include "functions.cpp"

int main()
{
    cout << "Enter all user's logins. Insert 'end' to stop:\n";
    fflush(stdout);

    vector<vector<string>> history(HISTORY_SIZE, vector<string>(3)); // 0 - пол, 1 - отпр, 2 - сообщение

    int index = 0;

    vector<string> logins;
    string login;

    ofstream out;
    out.open("logins.txt");
    while (login != "end")
    {
        cin >> login;

        if (in(logins, login) == -1)
        {
            if (login != "end")
            {
                logins.push_back(login);
                out << login << "\n";
            }
        }
        else
            cout << "Already exists! Write new login!\n";
        fflush(stdout);
    }
    out.close();

    // ZEROMQ

    void *context = zmq_ctx_new();

    void *requester = zmq_socket(context, ZMQ_PULL); // для приема сообщений сервером
    char conn_pull[] = "tcp://127.0.0.1:1234";
    int rc = zmq_bind(requester, conn_pull); // привязка сокета
    assert(rc == 0);

    void *publisher = zmq_socket(context, ZMQ_PUB); // для отправления сообщений сервером
    char conn_pub[] = "tcp://127.0.0.1:4321";
    rc = zmq_bind(publisher, conn_pub);
    assert(rc == 0);

    //

    MessageData buf; // сообщение (логин отправителя, логин получателя, сообщение)

    cout << "OK. Server is working!\n";

    while (1)
    {
        receive_struct(requester, &buf);

        int pos = h.find(buf.recipient_login);

        if (pos != -1) // history
        {
            pos = a.find(buf.msg_txt);
            if (pos != -1)
            {
                cout << "\n"
                     << "Message history of all users:\n";
                for (int i = 0; i < index; ++i)
                {
                    cout << "Recipient: " << history[i][0] << " Sender: " << history[i][1];
                    cout << " Message: " << history[i][2] << "\n";
                    fflush(stdout);
                }
            }
            else
            {
                cout << "\n"
                     << buf.sender_login << "'s message history with word '" << buf.msg_txt << "' :\n";
                fflush(stdout);

                char send[SIZE];
                char recp[SIZE];

                int s, r, q = 0;

                char m[SIZE];
                strcpy(m, buf.msg_txt);

                // цикл по кол-ву сообщений в истории
                for (int i = 0; i < index + 1; ++i)
                {
                    r = history[i][0].find(buf.sender_login);
                    s = history[i][1].find(buf.sender_login);
                    pos = history[i][2].find(m);

                    if ((r != -1 || s != -1) && pos != -1)
                    {
                        q++;
                        cout << "Recipient: " << history[i][0] << " Sender: " << history[i][1];
                        cout << " Message: " << history[i][2] << "\n";
                        fflush(stdout);
                    }
                }
                if (q == 0)
                {
                    cout << "No messages with word '" << buf.msg_txt << "'\n";
                    fflush(stdout);
                }
            }
        }
        else
        {
            // добавляем в историю

            char buffer[SIZE];
            string q = "";

            strcpy(buffer, buf.recipient_login);
            q = "";
            q += buf.recipient_login;
            history[index][0] = q;

            strcpy(buffer, buf.sender_login);
            q = "";
            q += buf.sender_login;
            history[index][1] = q;

            strcpy(buffer, buf.msg_txt);
            q = "";
            q += buf.msg_txt;
            history[index][2] = q;

            index++;

            // отправляем сообщение
            send_struct(publisher, buf);
        }
    }

    zmq_close(requester);
    zmq_term(context);
    remove("logins.txt");
    return 0;
}
