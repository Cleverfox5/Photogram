#include "HttpParser.hpp"
#include <optional>


std::optional<HttpParser::http_req_t> HttpParser::get_req_t(std::string_view req)
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

void HttpParser::parse_get_t()
{

}

void HttpParser::parse_post_t()
{

}

HttpParser::post_t_info HttpParser::parse_post_t(std::string_view req)
{
    HttpParser::post_t_info result;
    
    auto origin_o = Request::getValueSomeHeader(req, "Origin: ");
    if (!origin_o)
        return result;
    
    result.origin = *origin_o;

    return result;
}

HttpParser::options_t_info HttpParser::parse_options_t(std::string_view req)
{
    HttpParser::options_t_info result;
    
    auto origin_o = Request::getValueSomeHeader(req, "Origin: ");
    if (!origin_o)
        return result;
    
    result.origin = *origin_o;

    return result;
}

HttpParser::e_status HttpParser::check_is_full_request(std::string_view requ, HttpParser::e_http_req type)
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


HttpParser::e_status HttpParser::parse(std::string_view request, http_types_info & info_block)
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
        parse_get_t();
        break;
    case(e_http_req::post):
        info_block = parse_post_t(request.substr(shift));
        break;
    case(e_http_req::options):
        info_block = parse_options_t(request.substr(shift));
        break;
    default:
        return e_status::not_support_type;
    }

    return e_status::success;
}
