#include "ResponseSender.h"
#include <iostream>

void chackLoginHere(nlohmann::json& jsonRequestBody, bool& isRight, std::string& login) {
	login = "";
	isRight = false;
	if (jsonRequestBody["login"] == "sasha" && jsonRequestBody["password"] == std::to_string(123)) {
		isRight = true;
		login = "someSasha";
	}
}

ResponseSender::ResponseSender(SOCKET socket) : clientSocket(socket) {}

void ResponseSender::sendResponseAuthtorisation(std::string& request, std::string& nickname) {
	try {
		bool isRight = true;
		std::string userName;
		if (nickname == "") throw;
		sendLogin(isRight, userName, request);
	}
	catch(const std::exception & e) {
		throw;
	}
}

void ResponseSender::sendAnswerOK(std::string& request, nlohmann::json & jsonBody) {
	std::string clientAddres = utils.getValueSomeHeader(request, "Origin: ");
	std::string body = jsonBody.dump();
	std::string response = "HTTP/1.1 200 OK\r\n"
		"Version: HTTP/1.1\r\n"
		"Access-Control-Allow-Origin: " + clientAddres + "\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: " + std::to_string(body.size()) +
		"\r\n\r\n";
	response.append(body);
	send(clientSocket, response.c_str(), response.size(), 0);
	request.clear();
}

void ResponseSender::sendAnswerOptions(std::string& request) {
	std::string clientAddres = utils.getValueSomeHeader(request, "Origin: ");
	std::string response = "HTTP/1.1 204 No Content\r\n"
		"Access-Control-Allow-Origin: " + clientAddres + "\r\n"
		"Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
		"Access-Control-Allow-Headers: Content-Type\r\n"
		"Access-Control-Allow-Credentials: true\r\n"
		"\r\n";
	send(clientSocket, response.c_str(), response.size(), 0);
	request.clear();
}

void ResponseSender::sendLogin(bool isRight, std::string& userName, std::string& request) {
	nlohmann::json jsonBody;
	jsonBody["isRight"] = isRight;
	jsonBody["userName"] = userName;
	sendAnswerOK(request, jsonBody);
}
