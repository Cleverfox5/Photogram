#pragma once
#include <string>
#include <unordered_map>
#include "Request.h"

class FormData {
	Request utils;
	std::string getSeparator(std::string& request);
	
	void addPhoto(std::string& request, size_t& argPostion, size_t startPositon, size_t finishPositon,
		std::unordered_map<std::string, std::pair<std::size_t, std::size_t>>& filesCoordinates, 
		std::unordered_map<std::string, std::string>& userProperties);
	
	void separatorToPropertis(std::string& request, std::string& separator,
		std::unordered_map<std::string, std::string>& userProperties,
		std::unordered_map<std::string, std::pair<std::size_t, std::size_t>>& FilesCoordinates);
public:
	void parse(std::string & request,
		std::unordered_map<std::string, std::string> & userProperties, 
		std::unordered_map<std::string, std::pair<std::size_t, std::size_t>> & FilesCoordinates);
};