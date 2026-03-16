#include <iostream>
#include <memory>
#include "WorkWithClient.h"
#include "Request.h"
#include "ResponseSender.h"

WorkWithClient::WorkWithClient(SOCKET clientSocket, const std::string& clientAddress)
	: clientSocket(clientSocket), dbAPI(nullptr) {}

void WorkWithClient::run() {
	const short BUFFER_SIZE = 2048;
	char buffer[BUFFER_SIZE];
	ResponseSender sendResponse(clientSocket);
	Request utilsRequest;
	try {
		int hostdb = 5432;
		std::string Nmaedb = "Photogram";
		const std::string connection_str = "host = localhost port = 5432 dbname = MessengerPhotogram user = postgres password = Postgre123password";
		dbAPI = std::make_shared<WorkWithPostgreSQL>(connection_str);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		if (clientSocket != INVALID_SOCKET)
			closesocket(clientSocket);
		return;//поменять
	}

	std::string request;

	size_t bytesRead;

	while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
		request.append(buffer, bytesRead);

		//std::cout << request << std::endl;

		if (request.size() > sizeof("OPTIONS") && request.substr(0, sizeof("OPTIONS") - 1) == "OPTIONS") {
			if (request.find("\r\n\r\n") != std::string::npos)
				sendResponse.sendAnswerOptions(request);
		}
		else if (request.size() > sizeof("POST") && request.substr(0, sizeof("POST") - 1) == "POST") {
			size_t headersEnd;
			if ((headersEnd = request.find("\r\n\r\n")) != std::string::npos) {
				std::string body = request.substr(headersEnd + sizeof("\r\n\r\n") - 1);
				if (body.size() == std::stoi(utilsRequest.getValueSomeHeader(request, "Content-Length: "))) {
					if (request.substr(5, sizeof("/login") - 1) == "/login") {
						nlohmann::json jsonBody;
						std::string nickname;
						try { 
							jsonBody = nlohmann::json::parse(body);
							nickname = dbAPI->checkPassword(jsonBody["login"], jsonBody["password"]);
						}
						catch (const std::exception& e) {
							std::cerr << e.what() << std::endl;
							break;
						}
						
						sendResponse.sendResponseAuthtorisation(request, nickname);
					}
					else if (request.substr(5, sizeof("/authorization") - 1) == "/authorization") {
						std::cout << "request to registration was getting\n";
						nlohmann::json registrAnswerJson;
						try {
							dbAPI->addNewUser(request, registrAnswerJson);
							//sendToken();
						}
						catch (const std::exception& e) {
							std::cerr << e.what() << std::endl;
						}
						//if (dbAPI->addNewUser(body, registrAnswerJson)) sendResponse.sendAnswerOK();
						
					}
				}
			}
		}
		else if (request.size() > sizeof("GET") && request.substr(0, sizeof("GET") - 1) == "GET") {

		}
	}
	if (clientSocket != INVALID_SOCKET)
		closesocket(clientSocket);
}