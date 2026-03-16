#pragma warning(disable : 4996)
#include "WorkWithPostgreSQL.h"
#include <iostream>

WorkWithPostgreSQL::WorkWithPostgreSQL(const std::string & connection_str) 
	: connect(new pqxx::connection (connection_str))
{
	try {
		if (connect->is_open()) {
			std::cout << "Opened database successfully: " << connect->dbname() << std::endl;
			connect->prepare("addPhoto", "INSERT INTO profile_picture(photo_type, photo) "
				"VALUES ($1, $2) "
				"RETURNING id;");
			connect->prepare("addNewUser", 
				"INSERT INTO users(nickname, first_name, last_name, email, is_male, birthdate, password, photo_id) "
				"VALUES ($1, $2, $3, $4, $5, $6, $7, $8)");
			connect->prepare("checkPassword",
				"SELECT id FROM users WHERE email = $1 AND password = $2");
			connect->prepare("getNickname",
				"SELECT nickname FROM users WHERE id = $1");
		}
		else {
			std::cerr << "Can't open database\n";
			throw std::runtime_error("Can't open db");
		}
	}
	catch (const std::exception& e) {
		throw;
	}
}

std::string WorkWithPostgreSQL::addNewPhoto(std::string & request) {
	try {
		pqxx::work work(*connect);
		pqxx::row res;

		auto element = filesCoordinates.begin();
		std::string file = request.substr(element->second.first, element->second.second);
		std::vector<std::byte> binaryData(file.size());
		std::memcpy(binaryData.data(), file.data(), file.size());
		res = work.exec_prepared1("addPhoto",
			userProperties["photo_type"],
			pqxx::binarystring(reinterpret_cast<const char*>(binaryData.data()), binaryData.size())
		);
		
		std::string id = res[0].c_str();

		work.commit();

		return id;
	}
	catch (const std::exception & e) {
		throw;
	}
}

std::string WorkWithPostgreSQL::getNicknameById(std::string id) {
	try {
		pqxx::work work(*connect);
		pqxx::row res;

		res = work.exec_prepared1("getNickname",
			id);
		return res[0].c_str();
	}
	catch (const std::exception& e) {
		throw;
	}
}

std::string WorkWithPostgreSQL::checkPassword(std::string login, std::string password) {
	try {
		pqxx::work work(*connect);
		pqxx::row res;

		res = work.exec_prepared1("checkPassword", 
			login, 
			password);

		work.commit();

		return getNicknameById(res[0].c_str());

	} catch(const std::exception & e) {
		std::cerr << e.what() << std::endl;
		throw;
	}
}

void WorkWithPostgreSQL::addProperties(std::string photo_id) {
	try {
		pqxx::work work(*connect);
		pqxx::result res;

		res = work.exec_prepared("addNewUser",
			userProperties["nickname"],
			userProperties["first_name"],
			userProperties["last_name"],
			userProperties["email"],
			userProperties["is_male"],
			userProperties["birthdate"],
			userProperties["password"],
			photo_id
		);

		work.commit();
	}
	catch (const std::exception & e) {
		throw;
	}
}

void WorkWithPostgreSQL::addNewUser(std::string& request, nlohmann::json& registrAnswerJson) {
	try {
		formData.parse(request, userProperties, filesCoordinates);

		std::string photo_id = addNewPhoto(request);
		addProperties(photo_id);
	}
	catch (std::exception const & e) {
		std::cerr << e.what() << std::endl;
		throw e;
	}
}