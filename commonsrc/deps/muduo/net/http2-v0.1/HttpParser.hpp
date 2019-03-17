//http_parser为C语言写的一个http解析库,是蜕化于NGINX上面的http解析代码.
//注意1:http解析,有字段"Content-Length"，那么就会解析body;不区分GET/POST;
//    2:GET中的body,没有解析,放到url中了.可以另外解析.
//

#ifndef _http_parser_hpp__
#define _http_parser_hpp__

#include <map>
//#include <string>
//#include <string.h>
#include "http_parser.h"//c file;
#include <muduo/base/Types.h>
#include <algorithm>

#include <stdio.h>
//using std::string;
using std::map;
using namespace muduo;

struct THttpReq
{
    //暂时需要的,仅仅是body;POST;
	muduo::string url;
    map<muduo::string,muduo::string> headers;
    muduo::string body;
};

class CHttpParser
{
public :
    CHttpParser(http_parser_type type = HTTP_REQUEST)
    {
        //initialize http_parser;
        http_parser_init(&m_parser,HTTP_REQUEST);
        m_parser.out_obj = static_cast<void*>(this); 
        //
        init_callbacks();
    }
    ~CHttpParser()
    {
    	printf("-----distory[%p]\n",this);
    }
		
    void init()
    {
        clear();
    }

    void clear()
    {
        m_req.url.clear();
        m_req.headers.clear();
        m_req.body.clear();
        http_parser_init(&m_parser,HTTP_REQUEST);
        m_parser.out_obj = static_cast<void*>(this); 
    }

    bool is_error()
    {
        return m_parser.http_errno != 0;
    }

    muduo::string error_string()
    {
        http_errno errno = static_cast<http_errno>(m_parser.http_errno);
        muduo::string errstr(http_errno_name(errno));
        errstr.append(":");
        errstr.append(http_errno_name(errno));
        return errstr;
    }

    size_t parse(const char *data ,size_t len)
    {
        return http_parser_execute(&m_parser,&m_callbacks,data,(len));
    }

    void set_url(const char *data ,int len)
    {
        switch (m_parser.method)
        {
        case HTTP_GET:
        {
            parse_get_url_body(data,len);
            break;
        }
        case HTTP_POST:
        default :
        {
            m_req.url.assign(static_cast<const char*>(data),len);
            break;
        }
        }
    }

    unsigned int get_method()
    {
    	return m_parser.method;
    }

    const muduo::string& get_body() const
    {
        return m_req.body;
    }

    void add_body(const char *data ,int len)
    {
        //这里必须要用append;可能只解析了一半.
        m_req.body.append(data,len);
    }

    void add_headers(const char *data ,int len)
    {
        if (!m_temp_filed.empty())
        {
        	muduo::string d(data,len);
            m_req.headers[m_temp_filed] = d;
            m_temp_filed.clear();
        }
    }

    void save_filed(const char *data ,int len)
    {
        m_temp_filed.assign(data,len);
    }

    bool is_end()
    {
        //该函数为本人添加.
        return http_parse_end(&m_parser);
    }

    const std::map<muduo::string, muduo::string>& headers() const
    { return m_req.headers; }

    const muduo::string& path() const
    { return m_req.url; }

    muduo::string getHeader(const muduo::string& field) const
    {
      muduo::string result;
      std::map<muduo::string, muduo::string>::const_iterator it = m_req.headers.find(field);
      if (it != m_req.headers.end())
      {
        result = it->second;
      }
      return result;
    }

    void req_show();

private :
    void init_callbacks();

    void parse_get_url_body(const char *data ,int len)
    {
        const char *pos = std::find(data, data+len, '?');
        //strchr(data,'?');
        
        if (pos != NULL)
        {
            size_t sz = pos - data;
            m_req.url.assign(data,sz);
            m_req.body.assign(pos+1,len-sz);
        }
        else
        {
            m_req.url.assign(data,len);
        }
    }

    
public :
    http_parser m_parser;
    THttpReq m_req;
    http_parser_settings m_callbacks;
    muduo::string m_temp_filed;
    typedef map<muduo::string,muduo::string>::const_iterator CITR;
    typedef map<muduo::string,muduo::string>::iterator ITR;
};

#endif //_http_parser_hpp__

