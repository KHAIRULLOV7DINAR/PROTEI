#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
public:
    Server(const short port);
    ~Server();
    void main_cycle();

private:
    bool cycle_flag_;
    const short port_;
    int socket_;

    void set_up_server();
    void handle_client();
};