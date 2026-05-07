#pragma once
#include <string>
#include <unordered_map>
#include "Request.hpp"

class FormData {
	static void addPhoto(std::string_view request, size_t& argPostion, size_t startPositon, size_t finishPositon,
		std::map<std::string, std::pair<std::size_t, std::size_t>>& filesCoordinates,
		std::map<std::string, std::string>& userProperties);
	
	static std::string getSeparator(std::string_view request);
	
	static void separatorToPropertis(std::string_view request, std::string& separator,
		std::map<std::string, std::string>& userProperties,
		std::map<std::string, std::pair<std::size_t, std::size_t>>& FilesCoordinates);
public:
	static void parse(std::string_view request,
		std::map<std::string, std::string> & userProperties, 
		std::map<std::string, std::pair<std::size_t, std::size_t>> & FilesCoordinates);

	static void parsePhotoOnly(std::string_view request,
		std::map<std::string, std::string>& userProperties,
		std::map<std::string, std::pair<std::size_t, std::size_t>>& filesCoordinates);
};
