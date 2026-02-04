#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "TcpConnection.hpp"


class TcpServer
{
public:
    TcpServer(boost::asio::io_context& io_context_client);

private:
    void startAccept();

    void handle_accept(std::shared_ptr<TcpConnection> new_connection,
        const boost::system::error_code& error);

    boost::asio::io_context& io_context_client_;
    tcp::acceptor acceptor_;
};