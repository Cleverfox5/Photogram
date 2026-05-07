#pragma once
#include "FormData.hpp"
#include "HashTools.hpp"
#include "HttpParser.hpp"
#include <nlohmann/json.hpp>
#include <memory>
#include <unordered_map>
#include <pqxx/pqxx>

class WorkWithPostgreSQL {
	std::shared_ptr<pqxx::connection> connect;
	std::shared_ptr<HashTools> hashTools;

	std::string addNewPhoto(HttpParser::post_t_info& post_info);

	std::string addProperties(std::map<std::string, std::string>& userProperties);
	void connectPropertiesAndPhoto(std::string userId, std::string photoId);
	void deleteRow(std::string userId);
	void deletePhoto(std::string PhotoId);
	void updatePhoto(HttpParser::post_t_info& post_info, std::string& id);
	void updateData(nlohmann::json& jsonBody, std::string& id);
	void updatePassword(std::string & password, std::string& id);
public:
	WorkWithPostgreSQL(const std::string & connection_str);
	
	std::string getNicknameById(std::string id);
	std::string addNewUser(HttpParser::post_t_info& post_info, nlohmann::json& registrAnswerJson);
	std::string checkPassword(std::string login, std::string password);
	nlohmann::json getProfileInformation(std::string nickname);
	std::vector<char> getProfilePhoto(std::string nickname, std::string& type);
	void getUsers(std::map<std::string, std::string>& properties, nlohmann::json& json, std::string id);
	std::vector<char> getPhotoById(std::string id, std::string& type);
	void makeFriend(std::string & user_id, std::string & friend_id);
	void deleteFriend(std::string& user_id, std::string& friend_id);
	void getStatus(const std::string & user_id, const std::string & friend_id, nlohmann::json& json);
	void updateProfilePhoto(HttpParser::post_t_info& post_info, std::string& id);
	void updateProfileData(nlohmann::json& jsonBody, std::string& id);
	void updateProfilePassword(std::string& password, std::string& id);
};