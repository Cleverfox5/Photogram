#pragma once
#include <string>
#include <nlohmann/json.hpp>

class Request {
private:
	std::string request;
public:
	std::string getValueSomeHeader(std::string & request, std::string header, size_t startSerch = 0);
	std::string getPropertisName(std::string & request, size_t startPostion, std::string serchinTypeName = "name");
	std::string getSeparator(std::string& request, std::string header, size_t startSerch = 0);
};