#include "HttpParser.hpp"
#include "FormData.hpp"
#include <optional>

namespace HttpParser
{
namespace
{
std::optional<http_req_t> get_req_t(std::string_view req)
{
    const size_t max_hdr_size = 7;
    if (req.size() < max_hdr_size)
        return std::nullopt;

    auto shift = req.find(' ');
    if (shift == std::string::npos || shift > max_hdr_size)
        return std::nullopt;

    auto type_str = req.substr(0, shift);
    HttpParser::e_http_req type;

    if (type_str == std::string("OPTIONS"))
        type = e_http_req::options;
    else if (type_str == std::string("POST"))
        type = e_http_req::post;
    else if (type_str == std::string("GET"))
        type = e_http_req::get;
    else
        type = e_http_req::not_support;

    return http_req_t{ type, shift };
}

nlohmann::json get_body(std::string_view req)
{
    const std::string separator = "\r\n\r\n";
    auto headersEnd = req.find(separator);

    auto body = req.substr(headersEnd + separator.size());
    return nlohmann::json::parse(body);
}

std::optional<e_post_msg> get_post_msg(std::string_view req)
{
    auto shift_start = req.find('/');
    auto shift_end = req.substr(shift_start).find_first_of(" ?");
    if (shift_end == std::string::npos || shift_start == std::string::npos)
        return std::nullopt;

    auto msg_t_str = std::string(req.substr(shift_start + 1, shift_end - shift_start));
    auto msg_t_p = post_msg_names.find(msg_t_str);
    if (msg_t_p == post_msg_names.end())
        return std::nullopt;

    return msg_t_p->second;
}

std::optional<e_get_msg> get_get_msg(std::string_view req)
{
    auto shift_start = req.find('/');
    auto shift_end = req.substr(shift_start).find_first_of(" ?");
    if (shift_end == std::string::npos || shift_start == std::string::npos)
        return std::nullopt;

    auto msg_t_str = std::string(req.substr(shift_start + 1, shift_end - shift_start));
    auto msg_t_p = get_msg_names.find(msg_t_str);
    if (msg_t_p == get_msg_names.end())
        return std::nullopt;

    return msg_t_p->second;
}

bool parse_get_t(std::string_view req, http_types_info& info_block)
{
    auto& get_info = info_block.emplace<get_t_info>();

    auto origin_o = Request::getValueSomeHeader(req, "Origin: ");
    if (!origin_o)
        return false;
    get_info.origin = *origin_o;

    auto msg_o = get_get_msg(req);
    if (!msg_o)
        return false;
    get_info.msg = *msg_o;

	switch (get_info.msg)
	{
	case(e_get_msg::get_new_token):
        get_info.refreshToken = Request::getValueWithSpace(req, "refreshToken=");
        break;
	case(e_get_msg::get_profile_by_nickname):
        get_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
		break;
	case(e_get_msg::get_profile_photo):
        get_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
		break;
	case(e_get_msg::get_users_list):
        get_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
	    break;
	case(e_get_msg::get_photo_by_id):
        get_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
	    break;
	case(e_get_msg::make_friend):
        get_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
	    break;
	case(e_get_msg::delete_friend):
        get_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
	    break;
	case(e_get_msg::get_status):
        get_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
	    break;
	case(e_get_msg::get_id_fs_by_access):
        get_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
	    break;
    }

    return true;
}

bool parse_post_t(std::string_view req, http_types_info& info_block)
{
    auto& post_info = info_block.emplace<post_t_info>();

    auto origin_o = Request::getValueSomeHeader(req, "Origin: ");
    if (!origin_o)
        return false;
    post_info.origin = *origin_o;

    auto msg_o = get_post_msg(req);
    if (!msg_o)
        return false;
    post_info.msg = *msg_o;

    switch(post_info.msg)
    {
    case(e_post_msg::login):
        post_info.jsonBody = get_body(req);
        
        break;
    case(e_post_msg::registration):
    {
        FormData::parse(req, post_info.userProperties, post_info.filesCoordinates);

        auto& element = post_info.filesCoordinates.begin();
        auto file = req.substr(element->second.first, element->second.second);

        post_info.binaryData.resize(file.size());
        std::memcpy(post_info.binaryData.data(), file.data(), file.size());

        break;
    }
    case(e_post_msg::update_profile_photo):
    {
        post_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
        FormData::parsePhotoOnly(req, post_info.userProperties, post_info.filesCoordinates);

        auto& element = post_info.filesCoordinates.begin();
        auto file = req.substr(element->second.first, element->second.second);

        post_info.binaryData.resize(file.size());
        std::memcpy(post_info.binaryData.data(), file.data(), file.size());
        break;
    }
    case(e_post_msg::update_profile_data):
        post_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
        post_info.jsonBody = get_body(req);
        break;
    case(e_post_msg::update_profile_password):
        post_info.accessToken = Request::getValueWithSpace(req, "Bearer ");
        post_info.jsonBody = get_body(req);
        break;
    }

    return true;
}

bool parse_options_t(std::string_view req, http_types_info& info_block)
{
    auto& opt_info = info_block.emplace<options_t_info>();

    auto origin_o = Request::getValueSomeHeader(req, "Origin: ");
    if (!origin_o)
        return false;

    opt_info.origin = *origin_o;

    return true;
}

e_status check_is_full_request(std::string_view requ, HttpParser::e_http_req type)
{
    const std::string separator = "\r\n\r\n";
    auto headers_end = requ.find(separator);

    if (headers_end == std::string::npos)
        return e_status::not_full_req;

    if (type != e_http_req::post)
        return e_status::success;

    size_t body_size = requ.size() - headers_end - separator.size();
    size_t header_value = std::stoi(*Request::getValueSomeHeader(requ, "Content-Length: "));

    if (body_size == header_value)
        return e_status::success;

    if (body_size < header_value)
        return e_status::not_full_req;

    return e_status::bad_http;
}

};

e_status parse(std::string_view request, http_types_info& info_block)
{
    //getting the request code
    auto type = get_req_t(request);
    if (!type)
        return e_status::bad_http;
    size_t shift = type->size;

    //chaking if the request is full
    auto check_status = check_is_full_request(request, type->type);
    if (check_status != e_status::success)
        return check_status;

    //processing of request fields
    switch (type->type)
    {
    case(e_http_req::get):
        if (!parse_get_t(request.substr(shift), info_block))
            return e_status::bad_http;
        break;
    case(e_http_req::post):
        if (!parse_post_t(request.substr(shift), info_block))
            return e_status::bad_http;
        break;
    case(e_http_req::options):
        if (!parse_options_t(request.substr(shift), info_block))
            return e_status::bad_http;
        break;
    default:
        return e_status::not_support_type;
    }

    return e_status::success;
}

};