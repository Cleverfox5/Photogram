#include "Request.h"
#include <iostream>
/*
Request::Request(std::string request) : request(request) {}

void Request::clear() {request.clear();}

std::string Request::getString() {return request;}

size_t Request::find(std::string str, size_t startPosion) {
	if (!startPosion) return request.find(str, startPosion);
	return request.find(str);
}

std::string Request::substr(size_t startPositon, size_t biteCount) {
	if (biteCount == 0) return request.substr(startPositon);
	return request.substr(startPositon, biteCount);
}

size_t Request::size() {return request.size();}

void Request::appendPart(const std::string& someData, size_t bytesRead) {
	request.append(someData, bytesRead);
}
*/

std::string Request::getValueSomeHeader(std::string & request, std::string header, size_t startSerch) {
	size_t positonHeader;
	if ((positonHeader = request.find(header, startSerch)) == std::string::npos)
		return "NOT DEFINED";
	size_t positonEndHeader = request.find("\r\n", positonHeader);
	size_t startPositon = positonHeader + header.size() - 1;
	return request.substr(startPositon, positonEndHeader - startPositon);
}

std::string Request::getSeparator(std::string& request, std::string header, size_t startSerch) {
	size_t positonHeader;
	if ((positonHeader = request.find(header, startSerch)) == std::string::npos)
		return "NOT DEFINED";
	size_t positonEndHeader = request.find("\r\n", positonHeader);
	size_t startPositon = positonHeader + header.size() + 1;
	return request.substr(startPositon, positonEndHeader - startPositon);
};

std::string Request::getPropertisName(std::string & request, size_t startPostion, std::string serchinTypeName) {
	size_t startNamePos, endNamePos;
	if ((startNamePos = request.find(serchinTypeName + "=\"", startPostion)) != std::string::npos) {
		startNamePos += serchinTypeName.size() + sizeof("=\"") - 1;
		endNamePos = request.find("\"\r\n", startNamePos);
	}
	else return ""; //throw
	
	if (endNamePos != std::string::npos) {
		--endNamePos;
		return request.substr(startNamePos, endNamePos - startNamePos + 1);
	}
	
	return "";//throw
}