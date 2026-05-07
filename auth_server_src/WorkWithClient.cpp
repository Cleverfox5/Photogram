#include <iostream>
#include <memory>
#include "WorkWithClient.hpp"
#include "Request.hpp"
#include "WorkWithPostgreSQL.hpp"
#include "HttpParser.hpp"
#include "cstring"

WorkWithClient::WorkWithClient(SOCKET clientSocket, const std::string& clientAddress)
	: clientSocket(clientSocket), dbAPI(nullptr) {}

void WorkWithClient::run() {
	const short BUFFER_SIZE = 2048;
	char buffer[BUFFER_SIZE];
	ResponseSender sendResponse(clientSocket);
	try {
		int hostdb = 5432;
		std::string Nmaedb = "Photogram";
		
		const std::string connection_str = "host = localhost port = 5432 dbname = photogram user = postgres password = 1";
		dbAPI = std::make_shared<WorkWithPostgreSQL>(connection_str);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		if (clientSocket != INVALID_SOCKET)
			closesocket(clientSocket);
		return;
	}

	std::string request;

	size_t bytesRead;

	while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
		request.append(buffer, bytesRead);

		//std::cout << request << "\n\n";

		HttpParser::http_types_info info_block;
		auto status = HttpParser::parse(request, info_block);

		if (status == HttpParser::e_status::not_full_req)
			continue;
		
		if (status == HttpParser::e_status::bad_http || status == HttpParser::e_status::not_support_type)
		{
			request.clear();
			continue;
		}

		if (std::holds_alternative<HttpParser::options_t_info>(info_block))
		{
			auto& options_info = std::get<HttpParser::options_t_info>(info_block);
			sendResponse.sendAnswerOptions(options_info.origin);
		}
		else if (std::holds_alternative<HttpParser::post_t_info>(info_block))
		{
			auto& post_info = std::get<HttpParser::post_t_info>(info_block);

			switch(post_info.msg)
			{
			case(HttpParser::e_post_msg::login):
			{
				std::string id;
				try {
					id = dbAPI->checkPassword(post_info.jsonBody["login"], post_info.jsonBody["password"]);

					std::string refreshToken = JwtToken::createRefreshToken(id, secret);
					std::string accessToken = JwtToken::createAccessToken(id, secret);

					std::string nickname = dbAPI->getNicknameById(id);

					sendResponse.sendAnswerOK(post_info.origin, refreshToken, accessToken, nickname);
				}
				catch (const std::exception& e) {
					std::cerr << e.what() << std::endl;
					if (!std::strcmp(e.what(), "Invalid login or password"))
						sendResponse.sendError(post_info.origin, "401", "Invalid login or password");
					else
						sendResponse.sendError(post_info.origin, "500", "Internal Server Error");
				}
				break;
			}
			case(HttpParser::e_post_msg::registration):
			{
				std::cout << "request to registration was getting\n";
				nlohmann::json registrAnswerJson;
				try {
					std::string id = dbAPI->addNewUser(post_info, registrAnswerJson);

					sendResponse.sendAnswerOK(post_info.origin);
				}
				catch (const std::exception& e) {
					std::cerr << e.what() << std::endl;
					if (!std::strcmp(e.what(), "This email is already occupied") || !std::strcmp(e.what(), "The nickname is already occupied"))
						sendResponse.sendError(post_info.origin, "401", e.what());
					else
						sendResponse.sendError(post_info.origin, "500", "Internal Server Error");
				}
				break;
			}
			case(HttpParser::e_post_msg::update_profile_photo):
			{
				std::cout << "request to registration was getting\n";
				std::string id;
				try {
					id = checkAccesstoken(post_info.origin, post_info.accessToken, sendResponse);
				}
				catch (const std::exception& e) { continue; }

				try {
					dbAPI->updateProfilePhoto(post_info, id);

					sendResponse.sendAnswerOK(post_info.origin);
				}
				catch (const std::exception& e) {
					std::cerr << e.what() << std::endl;
					if (!std::strcmp(e.what(), "This email is already occupied") || !std::strcmp(e.what(), "The nickname is already occupied"))
						sendResponse.sendError(post_info.origin, "400", e.what());
					else if (!std::strcmp(e.what(), "400")) {
						sendResponse.sendError(post_info.origin, "400", "Bad request");
					}
					else
						sendResponse.sendError(post_info.origin, "500", "Internal Server Error");
				}
				break;
			}
			case(HttpParser::e_post_msg::update_profile_data):
			{
				std::string id;
				try {
					id = checkAccesstoken(post_info.origin, post_info.accessToken, sendResponse);
				}
				catch (const std::exception& e) { continue; }

				try {
					dbAPI->updateProfileData(post_info.jsonBody, id);

					sendResponse.sendAnswerOK(post_info.origin);
				}
				catch (const std::exception& e) {
					std::cerr << e.what() << std::endl;
					if (!std::strcmp(e.what(), "This email is already occupied") || !std::strcmp(e.what(), "The nickname is already occupied"))
						sendResponse.sendError(post_info.origin, "400", e.what());
					else
						sendResponse.sendError(post_info.origin, "500", "Internal Server Error");
				}
				break;
			}
			case(HttpParser::e_post_msg::update_profile_password):
			{
				std::string hashed_password = post_info.jsonBody.get<std::string>();
				std::string id;
				try {
					id = checkAccesstoken(post_info.origin, post_info.accessToken, sendResponse);
				}
				catch (const std::exception& e) { continue; }

				try {
					dbAPI->updateProfilePassword(hashed_password, id);

					sendResponse.sendAnswerOK(post_info.origin);
				}
				catch (const std::exception& e) {
					std::cerr << e.what() << std::endl;
					if (!std::strcmp(e.what(), "This email is already occupied") || !std::strcmp(e.what(), "The nickname is already occupied"))
						sendResponse.sendError(post_info.origin, "400", e.what());
					else
						sendResponse.sendError(post_info.origin, "500", "Internal Server Error");
				}
				break;
			}
			}
			
			
		}
		else if (std::holds_alternative<HttpParser::get_t_info>(info_block))
		{
			auto& get_info = std::get<HttpParser::get_t_info>(info_block);

			std::string id;
			if (get_info.msg != HttpParser::e_get_msg::get_new_token)
			{
				try 
				{
					id = checkAccesstoken(get_info.origin, get_info.accessToken, sendResponse);
				}
				catch (const std::exception& e) { continue; }
			}

			switch (get_info.msg)
			{
			case(HttpParser::e_get_msg::get_new_token):
				try {
					id = JwtToken::checkRefreshToken(get_info.refreshToken, secret);
					std::string accessToken = JwtToken::createAccessToken(id, secret);
					std::string nickname = dbAPI->getNicknameById(id);

					sendResponse.sendAnswerOK(get_info.origin, get_info.refreshToken, accessToken, nickname);
				}
				catch (const std::exception& e) {
					sendResponse.sendError(get_info.origin, "401", "Invalid refresh token");
					std::cerr << e.what() << std::endl;
				}
				break;
			case(HttpParser::e_get_msg::get_profile_by_nickname):
			{
				size_t nicknameStart = 4 + sizeof("/getProfileByNickname?nickname=") - 1;
				size_t nicknameEnd = request.find(" ", nicknameStart) - 1;
				std::string nickname = request.substr(nicknameStart, nicknameEnd - nicknameStart + 1);
				try {
					nlohmann::json body = dbAPI->getProfileInformation(nickname);
					sendResponse.sendAnswerOK(get_info.origin, body);
				}
				catch (const std::exception& e) {
					if (!std::strcmp(e.what(), "404"))
						sendResponse.sendError(get_info.origin, "404", "Not found");
					else
						sendResponse.sendError(get_info.origin, "500", "Internal Server Error");
				}
			}
				break;
			case(HttpParser::e_get_msg::get_profile_photo):
			{
				size_t nicknameStart = 4 + sizeof("/getProfilePhoto?nickname=") - 1;
				size_t nicknameEnd = request.find(" ", nicknameStart) - 1;
				std::string nickname = request.substr(nicknameStart, nicknameEnd - nicknameStart + 1);
				std::string type;
				try {
					std::vector<char> binData = dbAPI->getProfilePhoto(nickname, type);
					sendResponse.sendAnswerOKBinData(get_info.origin, type, binData);
				}
				catch (const std::exception& e) {
					if (!std::strcmp(e.what(), "404"))
						sendResponse.sendError(get_info.origin, "404", "Not found");
					else
						sendResponse.sendError(get_info.origin, "500", "Internal Server Error");
				}
			}
				break;
			case(HttpParser::e_get_msg::get_users_list):
			{
				std::unordered_map<std::string, std::string> properties;
				try {
					nlohmann::json json;

					Request::URLParser(request, properties, sizeof("GET ") - 1);
					dbAPI->getUsers(properties, json, id);
					sendResponse.sendAnswerOK(get_info.origin, json);
				}
				catch (const std::exception& e) {
					if (!std::strcmp(e.what(), "bad http"))
						sendResponse.sendError(get_info.origin, "400", e.what());
					else
						sendResponse.sendError(get_info.origin, "500", "Internal Server Error");
				}
			}
				break;
			case(HttpParser::e_get_msg::get_photo_by_id):
			{
				std::unordered_map<std::string, std::string> properties;
				try {
					Request::URLParser(request, properties, sizeof("GET ") - 1);
					std::string id = properties.begin()->second;

					std::string type;
					std::vector<char> binData = dbAPI->getPhotoById(id, type);
					sendResponse.sendAnswerOKBinData(get_info.origin, type, binData);
				}
				catch (const std::exception& e) {
					if (!std::strcmp(e.what(), "bad http"))
						sendResponse.sendError(get_info.origin, "400", e.what());
					else
						sendResponse.sendError(get_info.origin, "500", "Internal Server Error");
				}
			}
				break;
			case(HttpParser::e_get_msg::make_friend):
			{
				std::unordered_map<std::string, std::string> properties;

				try {
					nlohmann::json json;
					Request::URLParser(request, properties, sizeof("GET ") - 1);

					dbAPI->makeFriend(id, properties["friend_id"]);
					sendResponse.sendAnswerOK(get_info.origin);
				}
				catch (const std::exception& e) {
					if (!std::strcmp(e.what(), "bad http"))
						sendResponse.sendError(get_info.origin, "400", e.what());
					else if (!std::strcmp(e.what(), "404"))
						sendResponse.sendError(get_info.origin, "404", "Not found");
					else
						sendResponse.sendError(get_info.origin, "500", "Internal Server Error");
				}
			}
				break;
			case(HttpParser::e_get_msg::delete_friend):
			{
				std::unordered_map<std::string, std::string> properties;
				try {
					nlohmann::json json;
					Request::URLParser(request, properties, sizeof("GET ") - 1);

					dbAPI->deleteFriend(id, properties["friend_id"]);
					sendResponse.sendAnswerOK(get_info.origin);
				}
				catch (const std::exception& e) {
					if (!std::strcmp(e.what(), "bad http"))
						sendResponse.sendError(get_info.origin, "400", e.what());
					else if (!std::strcmp(e.what(), "404"))
						sendResponse.sendError(get_info.origin, "404", "Not found");
					else
						sendResponse.sendError(get_info.origin, "500", "Internal Server Error");
				}
			}
				break;
			case(HttpParser::e_get_msg::get_status):
			{
				std::unordered_map<std::string, std::string> properties;

				try {
					nlohmann::json json;

					Request::URLParser(request, properties, sizeof("GET ") - 1);
					dbAPI->getStatus(id, properties.begin()->second, json);
					sendResponse.sendAnswerOK(get_info.origin, json);
				}
				catch (const std::exception& e) {
					if (!std::strcmp(e.what(), "bad http"))
						sendResponse.sendError(get_info.origin, "400", e.what());
					else if (!std::strcmp(e.what(), "404"))
						sendResponse.sendError(get_info.origin, "404", "Not found");
					else
						sendResponse.sendError(get_info.origin, "500", "Internal Server Error");
				}
			}
				break;
			case(HttpParser::e_get_msg::get_id_fs_by_access):
			{
				try {
					nlohmann::json body;
					body["id"] = id;
					sendResponse.sendAnswerOK(get_info.origin, body);
				}
				catch (const std::exception& e) {
					sendResponse.sendError(get_info.origin, "500", "Internal Server Error");
				}
			}
				break;
			}
		}

		
		request.clear();
	}
	if (clientSocket != INVALID_SOCKET) {
		closesocket(clientSocket);
		std::cout << "Socket closed\n";
	}
}

std::string WorkWithClient::checkAccesstoken(std::string & origin, std::string& accessToken, ResponseSender & sendResponse) {
	try {
		std::string id = JwtToken::checkAccessToken(accessToken, secret);
		return id;
	}
	catch (const std::exception& e) {
		if (!std::strcmp(e.what(), "bad http"))
			sendResponse.sendError(origin, "400", e.what());
		if (!std::strcmp(e.what(), "invalid access token"))
			sendResponse.sendError(origin, "401", e.what());
		else
			sendResponse.sendError(origin, "500", "Internal Server Error");

		throw;
	}
}