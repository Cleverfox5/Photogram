#pragma once
#include <string>
#include <optional>
#include <nlohmann/json.hpp>

class Request {
public:
	static std::optional<std::string> getValueSomeHeader(std::string_view request, std::string_view header, size_t startSerch = 0);
	static std::string getValueWithSpace(std::string_view request, std::string header, size_t startSerch = 0);
	static std::string getPropertisName(std::string_view  request, size_t startPostion, std::string serchinTypeName = "name");
	static std::string getSeparator(std::string_view  request, std::string header, size_t startSerch = 0);
	static std::string getType(std::string_view  request, std::string header, size_t startSerch = 0);
	static void URLParser(std::string_view  httpString, std::unordered_map<std::string, std::string>& result, std::size_t start);
};
