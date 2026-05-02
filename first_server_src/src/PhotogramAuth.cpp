#include "MyServer.hpp"

#include <iostream>


const short PORT = 8080;


int main(int argc, char* argv[]) {
    MyServer server(PORT);
    server.run();
    std::cout << "The server has started\n";
    getchar();
    std::cout << "The server is shutting down\n";
    //server.stop();

    return 0;
}
