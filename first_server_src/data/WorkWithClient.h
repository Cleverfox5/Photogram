#pragma once
#include <string>
#include "WorkWithPostgreSQL.h"

class WorkWithClient {
	SOCKET clientSocket;
	std::shared_ptr<WorkWithPostgreSQL> dbAPI;
public:
	WorkWithClient(SOCKET clientSocket, const std::string& clientAddress);
	
	void run();
};