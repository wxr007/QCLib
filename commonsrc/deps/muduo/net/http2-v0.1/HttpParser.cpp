
//#define DEBUG_TEST_HTTP_PARSER
#ifdef DEBUG_TEST_HTTP_PARSER
#include <stdio.h>
#endif

#include "parser_callbacks.hpp"
#include "HttpParser.hpp"


void CHttpParser::init_callbacks()
{
    //这里，设置对应的数据回调函数.[Content-Length][Entity-Body]
    m_callbacks.on_message_begin = on_message_begin;
    m_callbacks.on_url = on_url;
    m_callbacks.on_status = NULL;
    //on_header_field/on_header_value是对应header取值.
    m_callbacks.on_header_field = on_header_field;
    m_callbacks.on_header_value = on_header_value;
    m_callbacks.on_headers_complete = on_headers_complete;
    m_callbacks.on_body = on_body;
    m_callbacks.on_message_complete = on_message_complete;
}

#include <stdio.h>

void CHttpParser::req_show()
{
//#ifdef DEBUG_TEST_HTTP_PARSER
#define PRINTHEADER(FILED,VALUE) printf("\t%.*s: %.*s\n",FILED.size(),FILED.c_str(),VALUE.size(),VALUE.c_str())
    printf("\n***MESSAGE BEGIN***\n\n");
    //1-url;
    printf("Url: %.*s\n", m_req.url.size(), m_req.url.c_str());
    //2-header;
    printf("******[%d]HEADERS BEGIN***\n",(int)m_req.headers.size());
    CITR pos = m_req.headers.begin();
    while (pos != m_req.headers.end())
    {
        PRINTHEADER(pos->first,pos->second);
        ++pos;
    }
    printf("******HEADERS COMPLETE***\n");
    //3-body;
    int sz = m_req.body.size();
    if (sz > 128)
    {
        sz = sz -128;
        printf("Body : %.*s ...(left %d characters)\n",128,m_req.body.c_str(),sz);
    }
    else
    {
        printf("Body : [%.*s] \n",sz,m_req.body.c_str());
    }    
    printf("\n***HEADERS COMPLETE***\n========================================\n");
//#endif
}


//------------------------------------for test----------------------------------
#ifdef DEBUG_TEST_HTTP_PARSER

void check_parse_result(CHttpParser &p);
//--------常规测试.
void test_http_parser1(CHttpParser &p);

void test_http_parser2(CHttpParser &p);

void test_http_parser3(CHttpParser &p);
//--------body 测试.
void test_http_parser_body1(CHttpParser &p);

void test_http_parser_body2(CHttpParser &p);

void test_http_parser_body3(CHttpParser &p);

void test_http_parser_body4(CHttpParser &p);

//--------

int main()
{
    CHttpParser parser;
    test_http_parser1(parser);
    //test_http_parser2(parser);
    //test_http_parser3(parser);
    //test_http_parser_body1(parser);
    //test_http_parser_body2(parser);
    //test_http_parser_body3(parser);
    //test_http_parser_body4(parser);
}

void check_parse_result(CHttpParser &p)
{
    if (p.is_end())
    {
        printf("CHttpParser::parse OK!\n");
    }
    else
    {
        printf("CHttpParser::parse ***FAILED***!\n");
    }
}


//正常情况(body is empty).
void test_http_parser1(CHttpParser &p)
{
	muduo::string httpstr("GET /test?[test] HTTP/1.1\r\nUser-Agent: curl/7.18.0 (i486-pc-linux-gnu) libcurl/7.18.0 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.1\r\n");
    httpstr.append("Host: 0.0.0.0=5000\r\n");
    httpstr.append("Accept: */*\r\n");
    httpstr.append("\r\n");

    p.clear();
    p.parse(httpstr.c_str(),httpstr.size());
    check_parse_result(p);
    p.req_show();
}

//"Host"内容为空.
void test_http_parser2(CHttpParser &p)
{
	muduo::string httpstr("GET /test HTTP/1.1\r\nUser-Agent: curl/7.18.0 (i486-pc-linux-gnu) libcurl/7.18.0 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.1\r\n");
    httpstr.append("Host:\r\n");
    httpstr.append("Accept: */*\r\n");
    httpstr.append("\r\n");

    p.clear();
    p.parse(httpstr.c_str(),httpstr.size());
    check_parse_result(p);
    p.req_show();
}

//"Host"内容为空,与Accept之间没有"\r\n"
void test_http_parser3(CHttpParser &p)
{
	muduo::string httpstr("GET /body HTTP/1.1\r\n");
    httpstr.append("Host: 172.16.9.196\r\n");
    httpstr.append("Accept: */*\r\n");
    httpstr.append("\r\n");

    p.clear();
    p.parse(httpstr.c_str(),httpstr.size());
    check_parse_result(p);
    p.req_show();
}
//-----------test body;
void test_http_parser_body1(CHttpParser &p)
{
	muduo::string body("[just for test body]");
    char number[128];
    snprintf(number,128,"Content-Length: %d\r\n",(int)body.size());
    //
    muduo::string httpstr("GET /body HTTP/1.1\r\n");
    httpstr.append("Host: 172.16.9.196\r\n");
    httpstr.append(number);
    httpstr.append("Accept: */*\r\n");
    httpstr.append("\r\n");

    p.clear();
    int ret = p.parse(httpstr.c_str(),httpstr.size());
    printf("CHttpParser::parse= %d\n",ret);
    check_parse_result(p);
    p.req_show();

}

void test_http_parser_body2(CHttpParser &p)
{
	muduo::string body("[just for test body]");
    char number[128];
    snprintf(number,128,"Content-Length: %d\r\n",(int)body.size());
    //
    muduo::string httpstr("GET /body HTTP/1.1\r\n");
    httpstr.append("Host: 172.16.9.196\r\n");
    httpstr.append(number);
    httpstr.append("Accept: */*\r\n");
    httpstr.append("\r\n");

    p.clear();
    int ret = p.parse(httpstr.c_str(),httpstr.size());
    ret = p.parse(body.c_str(),body.size());
    printf("CHttpParser::parse= %d\n",ret);
    check_parse_result(p);
    p.req_show();
}

void test_http_parser_body3(CHttpParser &p)
{
	muduo::string body("[just for test body]");
    char number[128];
    snprintf(number,128,"Content-Length: %d\r\n",(int)body.size());
    //
    muduo::string httpstr("GET /body HTTP/1.1\r\n");
    httpstr.append("Host: 172.16.9.196\r\n");
    httpstr.append(number);
    httpstr.append("Accept: */*\r\n");
    httpstr.append("\r\n");

    p.clear();
    int ret = p.parse(httpstr.c_str(),httpstr.size());
    ret = p.parse(body.c_str(),5);
    ret = p.parse(body.c_str(),body.size()-5);
    printf("CHttpParser::parse= %d\n",ret);
    check_parse_result(p);
    p.req_show();

}

void test_http_parser_body4(CHttpParser &p)
{
	muduo::string body("[just for test body]");
    char number[128];
    snprintf(number,128,"Content-Length: %d\r\n",(int)body.size());
    //
    muduo::string httpstr("GET /body HTTP/1.1\r\n");
    httpstr.append("Host: 172.16.9.196\r\n");
    httpstr.append(number);
    httpstr.append("Accept: */*\r\n");
    httpstr.append("\r\n");

    p.clear();
    int ret = p.parse(httpstr.c_str(),httpstr.size());
    ret = p.parse(body.c_str(),5);
    ret = p.parse(body.c_str()+5,body.size()-5);
    printf("CHttpParser::parse= %d\n",ret);
    check_parse_result(p);
    p.req_show();

}

#endif//DEBUG_TEST







