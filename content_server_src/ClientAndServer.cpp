#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <boost/asio.hpp>
#include "TcpServer.hpp"
#include <jwt-cpp/jwt.h>
#include <chrono>

using boost::asio::ip::tcp;
namespace http = boost::beast::http;

int main()
{
    auto token = jwt::create()
        .set_type("JWS")
        .set_issuer("PhotogramAuth")
        .set_subject("1")
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(7))
        .sign(jwt::algorithm::hs256{ "1312123" });


    try
    {
        boost::asio::io_context io_context_client;

        TcpServer server(io_context_client);

        io_context_client.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
