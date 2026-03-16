#pragma once
//#include <WinSock2.h>
#include <string>
#include <nlohmann/json.hpp>
#include "Request.h"
#include "WorkWithPostgreSQL.h"

class ResponseSender {
	Request utils;
	SOCKET clientSocket;
	void sendLogin(bool isRight, std::string & userName, std::string& request);
public:
	ResponseSender(SOCKET Socket);
	void sendResponseAuthtorisation(std::string & request, std::string & nickname);
	void sendAnswerOK(std::string& request, nlohmann::json& jsonBody);
	void sendAnswerOptions(std::string& request);
};
