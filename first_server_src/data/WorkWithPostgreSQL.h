#pragma once
#include "FormData.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <unordered_map>
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>

class WorkWithPostgreSQL {
	FormData formData;
	std::unordered_map<std::string, std::string> userProperties;
	std::unordered_map<std::string, std::pair<std::size_t, std::size_t>> filesCoordinates;
	std::shared_ptr<pqxx::connection> connect;

	std::string addNewPhoto(std::string& request);
	void addProperties(std::string photo_id);
	std::string getNicknameById(std::string id);
public:
	WorkWithPostgreSQL(const std::string & connection_str);
	void addNewUser(std::string& request, nlohmann::json & registrAnswerJson);
	std::string checkPassword(std::string login, std::string password);
};