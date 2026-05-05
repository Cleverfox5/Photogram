#pragma once
#include "Request.hpp"
#include <string>
#include <map>
#include <stdint.h>
#include <optional>
#include <variant>

class HttpParser 
{
public:

	enum class e_status : uint8_t
	{
		success,
		bad_http,
		not_support_type,
		not_full_req
	};

	enum class e_http_req : uint8_t
	{
		get,
		head,
		post,
		put,
		delete_m,
		connect,
		options,
		trace,
		patch,
		not_support
	};

	enum class e_post_msg : uint8_t
	{
		login,
		registration,
		update_profile_photo,
		update_profile_data,
		update_profile_password
	};

	std::map<std::string, e_post_msg> post_msg_names
	{
		{"login", e_post_msg::login},
		{"registration", e_post_msg::registration},
		{"updateProfilePhoto", e_post_msg::update_profile_photo},
		{"updateProfileData", e_post_msg::update_profile_data},
		{"updateProfilePassword", e_post_msg::update_profile_password}
	};

	struct http_req_t 
	{
		e_http_req type;
		size_t size;
	};

	struct options_t_info
	{
		std::string origin;
	};

	struct post_t_info
	{
		std::string origin; //optimize
	};

	struct get_t_info
	{
		std::string origin; //optimize
	};

	using http_types_info = std::variant<std::monostate, options_t_info, post_t_info, get_t_info>;

	static e_status parse(std::string_view request, http_types_info& info_block);

private:
	static std::optional<HttpParser::http_req_t> HttpParser::get_req_t(std::string_view req);
	static void parse_get_t();
	static void parse_post_t();
	static post_t_info parse_post_t(std::string_view req);
	static options_t_info parse_options_t(std::string_view req);
	static e_status check_is_full_request(std::string_view requ, HttpParser::e_http_req type);
};
