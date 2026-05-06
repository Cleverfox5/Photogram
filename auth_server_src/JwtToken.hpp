#pragma once
#include <string>
class JwtToken {
public:
	static std::string createRefreshToken(std::string id, std::string secret);
	static std::string createAccessToken(std::string id, std::string secret);
	static std::string checkRefreshToken(std::string & refreshToken, std::string secret);
	static std::string checkAccessToken(std::string  & accessToken, std::string secret);
};