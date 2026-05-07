#include "Request.hpp"
#include <iostream>


void Request::URLParser(std::string_view httpString, std::map<std::string, std::string>& result, std::size_t start) 
{
	if (httpString.empty()) return;
	size_t p = start, properStart = 0, properEnd = 0, separator = 0;
	while (p < httpString.size() && httpString[p] != ' ') {
		char currChar = httpString[p];
		if (currChar == '?')
			properStart = p + 1;
		else if (currChar == '=')
			separator = p;
		else if (currChar == '&') {
			if (properStart == 0 || separator == 0)
				throw std::runtime_error("bad http");
			result[std::string(httpString.substr(properStart, separator - properStart))] =
				httpString.substr(separator + 1, p - (separator + 1));
			properStart = p + 1;
		}
		properEnd = p++;
	}
	result[std::string(httpString.substr(properStart, separator - properStart))] = httpString.substr(separator + 1, p - separator - 1);
}

std::optional<std::string> Request::getValueSomeHeader(std::string_view request, std::string_view header, size_t startSerch) {
	size_t positonHeader;
	positonHeader = request.find(header, startSerch);
	if (positonHeader == std::string::npos)
		return std::nullopt;

	size_t positonEndHeader = request.find("\r\n", positonHeader);
	size_t startPositon = positonHeader + header.size() - 1;
	return std::string(request.substr(startPositon, positonEndHeader - startPositon));
}

std::string Request::getValueWithSpace(std::string_view request, std::string header, size_t startSerch) {
	size_t positonHeader;
	if ((positonHeader = request.find(header, startSerch)) == std::string::npos)
		throw std::runtime_error("Не удалось найти");
	size_t positonEndHeader = request.find("\r\n", positonHeader);
	size_t startPositon = positonHeader + header.size();
	return std::string(request.substr(startPositon, positonEndHeader - startPositon));
}

std::string Request::getType(std::string_view request, std::string header, size_t startSerch) {
	size_t positonHeader;
	if ((positonHeader = request.find(header, startSerch)) == std::string::npos)
		throw std::runtime_error("Не удалось найти");
	size_t positonEndHeader = request.find("\r\n", positonHeader);
	size_t startPositon = positonHeader + header.size() - 1;
	return std::string(request.substr(startPositon + 1, positonEndHeader - startPositon - 1));
}

std::string Request::getSeparator(std::string_view request, std::string header, size_t startSerch) {
	size_t positonHeader;
	if ((positonHeader = request.find(header, startSerch)) == std::string::npos)
		throw std::runtime_error("Не удалось найти");
	size_t positonEndHeader = request.find("\r\n", positonHeader);
	size_t startPositon = positonHeader + header.size() + 1;
	return std::string(request.substr(startPositon, positonEndHeader - startPositon));
};

std::string Request::getPropertisName(std::string_view request, size_t startPostion, std::string serchinTypeName) {
	size_t startNamePos, endNamePos;
	if ((startNamePos = request.find(serchinTypeName + "=\"", startPostion)) != std::string::npos) {
		startNamePos += serchinTypeName.size() + sizeof("=\"") - 1;
		endNamePos = request.find("\"\r\n", startNamePos);
	}
	else return ""; //throw
	
	if (endNamePos != std::string::npos) {
		--endNamePos;
		return std::string(request.substr(startNamePos, endNamePos - startNamePos + 1));
	}
	
	return "";//throw
}

