#pragma once
#include <string>
#include <map>
#include <variant>
#include <stdint.h>
#include <nlohmann/json.hpp>

namespace HttpParser
{

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

	const std::map<std::string, e_post_msg> post_msg_names
	{
		{"login", e_post_msg::login},
		{"registration", e_post_msg::registration},
		{"updateProfilePhoto", e_post_msg::update_profile_photo},
		{"updateProfileData", e_post_msg::update_profile_data},
		{"updateProfilePassword", e_post_msg::update_profile_password}
	};

	enum class e_get_msg : uint8_t
	{
		get_new_token,
		get_profile_by_nickname,
		get_profile_photo,
		get_users_list,
		get_photo_by_id,
		make_friend,
		delete_friend,
		get_status,
		get_id_fs_by_access
	};

	const std::map<std::string, e_get_msg> get_msg_names
	{
		{"getNewToken", e_get_msg::get_new_token},
		{"getProfileByNickname", e_get_msg::get_profile_by_nickname},
		{"getProfilePhoto", e_get_msg::get_profile_photo},
		{"getUsersList", e_get_msg::get_users_list},
		{"getPhotoById", e_get_msg::get_photo_by_id},
		{"makeFriend", e_get_msg::make_friend},
		{"deleteFriend", e_get_msg::delete_friend},
		{"getStatus", e_get_msg::get_status},
		{"getIdForServerByAccess", e_get_msg::get_id_fs_by_access}
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
		e_post_msg msg;
		nlohmann::json jsonBody;
		std::string accessToken;
	};

	struct get_t_info
	{
		std::string origin; //optimize
		e_get_msg msg;
		std::string refreshToken;
		std::string accessToken;
	};

	using http_types_info = std::variant<std::monostate, options_t_info, post_t_info, get_t_info>;

};