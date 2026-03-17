#include "../include/Server.h"


Server::Server(const short port) : port_(port)
{
    set_up_server();
}

Server::~Server()
{
    if(socket_ != -1)
    {
        close(socket_);
    }
}

void Server::main_cycle()
{
    while(cycle_flag_)
    {
        int client = accept ( socket_, nullptr, nullptr );
        
        /* ОБМЕН С КЛИЕНТОМ */
        
        shutdown ( client, SHUT_RDWR );
        close ( client );
    }
}

void Server::set_up_server()
{
    socket_ = socket ( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    if (socket_ == -1)
    {
        throw std::runtime_error("Creating server socket failed!");
    }

    int opt = 1;
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in sock_addr{};
    sock_addr.sin_family = PF_INET;
    sock_addr.sin_port = htons(port_);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind( socket_, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) == -1)
    {
        close(socket_);

        throw std::runtime_error("Binding socket failed!");
    };

    if(listen( socket_, 10 ) == -1)
    {
        close(socket_);

        throw std::runtime_error("Listening socket failed!");
    };

    cycle_flag_ = true;
}

void handle_client()
{
    
}