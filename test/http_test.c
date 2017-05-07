#include <http_parser.h>

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// 开始解析报文，在url之前
int on_message_begin (http_parser *parser){
    printf("on_message_begin\n");
    return 0;
}

// 解析到url数据，可能只是一部分
int on_url(http_parser *parser, const char *at, size_t length){
    char *url = (char*)calloc(1,length+1);
    memcpy(url,at,length);
    url[length] = 0;
    printf("on_url:%s\n",url);
    return 0;
}

int on_status(http_parser *parser, const char *at, size_t length){
    char *status = (char*)calloc(1,length+1);
    memcpy(status,at,length);
    status[length] = 0;
    printf("on_status:%s\n",status);
    return 0;
}

int on_header_field(http_parser *parser, const char *at, size_t length){
    char *field = (char*)calloc(1,length+1);
    memcpy(field,at,length);
    field[length] = 0;
    printf("on_header_field:%s\n",field);
    return 0;
}

int on_header_value(http_parser *parser, const char *at, size_t length){
    char *value = (char*)calloc(1,length+1);
    memcpy(value,at,length);
    value[length] = 0;
    printf("on_header_value:%s\n",value);
    return 0;
}

int on_headers_complete(http_parser *parser){
    printf("on_headers_complete\n");
    return 0;
}

int on_body(http_parser *parser, const char *at, size_t length){
    char *body = (char*)calloc(1,length+1);
    memcpy(body,at,length);
    body[length] = 0;
    printf("on_body:%s\n",body);
    return 0;
}

int on_message_complete(http_parser *parser){
    printf("on_message_complete\n");
    return 0;
}

int main(){

    http_parser_settings settings = {
        .on_message_begin = on_message_begin,
        .on_url = on_url,
        .on_status = on_status,
        .on_header_field = on_header_field,
        .on_header_value = on_header_value,
        .on_headers_complete = on_headers_complete,
        .on_body = on_body,
        .on_message_complete = on_message_complete,
    };
    http_parser parser;
    http_parser_init(&parser, HTTP_BOTH);


    char buff[] = "GET /demo HTTP/1.1\r\n"
                 "Host: www.baidu.com\r\n"
                 "Content-Length: 5\r\n"
                  "\r\n"
                  "hello";
    char buff_web[] = "GET /demo HTTP/1.1\r\n"
                      "Upgrade: WebSocket\r\n"
                      "Connection: Upgrade\r\n"
                      "Host: example.com\r\n"
                      "Origin: http://example.com\r\n"
                      "WebSocket-Protocol: sample\r\n\r\n";

    http_parser_execute(&parser,&settings,buff_web,strlen( buff_web ));
    if ( parser.upgrade )
    {
        printf( "http upgrade check !!!!!!!!!!!!!!!!!!\n" );
    }
    
    http_parser_execute(&parser,&settings,buff,strlen( buff ));
    if ( parser.upgrade )
    {
        printf( "http upgrade ==================\n" );
    }

    return 0;
}
