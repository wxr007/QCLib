
#ifndef _parser_callback_hpp__
#define _parser_callback_hpp__

#include "HttpParser.hpp"

inline int on_message_begin(http_parser* _) 
{
    (void)_;
    //printf("\n***MESSAGE BEGIN***\n\n");
    return 0;
}

inline int on_headers_complete(http_parser* _) 
{
    (void)_;
    //printf("\n***HEADERS COMPLETE***\n\n");
    return 0;
}

inline int on_message_complete(http_parser* _) 
{
    (void)_;
    //printf("\n***MESSAGE COMPLETE***\n\n");
    return 0;
}

inline int on_url(http_parser* p, const char* at, size_t length) 
{
    CHttpParser* member = static_cast<CHttpParser*>(p->out_obj);
    //printf("Url: %.*s\n", (int)length, at);
    member->set_url(at,length);
    return 0;
}

inline int on_header_field(http_parser* p, const char* at, size_t length) 
{
    CHttpParser* member = static_cast<CHttpParser*>(p->out_obj);
    //printf("Header field: %.*s\n", (int)length, at);
    member->save_filed(at,length);
    return 0;
}

inline int on_header_value(http_parser* p, const char* at, size_t length) 
{
    CHttpParser* member = static_cast<CHttpParser*>(p->out_obj);
    //printf("Header value: %.*s\n", (int)length, at);
    member->add_headers(at,length);
    return 0;
}

inline int on_body(http_parser* p, const char* at, size_t length) 
{
    CHttpParser* member = static_cast<CHttpParser*>(p->out_obj);
    //printf("Body: %.*s\n", (int)length, at);
    member->add_body(at,length);
    return 0;
}



#endif //_parser_callback_hpp__


