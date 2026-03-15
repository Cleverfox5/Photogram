//#include "PhotogramAuth.h"
//#include "MyServer.h"

#include <nlohmann/json.hpp>
#include <jwt-cpp/jwt.h>
#include <pqxx/pqxx>
#include <sodium.h>


const short PORT = 8080;


int main(int argc, char* argv[]) {
    //MyServer server(PORT);
    //server.run();
    getchar();
    std::cout << "The server is shutting down\n";
    //server.stop();

    return 0;
}