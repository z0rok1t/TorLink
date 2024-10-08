/* torlink.h */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h> 
#include <sys/socket.h> 
#include<arpa/inet.h>
#include<netinet/in.h>
#include<dlfcn.h>


#define PROXY        "127.0.0.1"
#define PROXYPORT    3301
#define USERNAME "torlink"
#define reqsize sizeof(struct proxy_request)
#define ressize sizeof(struct proxy_response)

typedef unsigned char int8;        /*Define a type character of 8 bits*/
typedef unsigned short int int16;        /*Define an integer type of 16 bits*/
typedef unsigned int int32;        /*Define an integer type of 32 bits*/


/* 
1) CONNECT

        +----+----+----+----+----+----+----+----+----+----+....+----+
		| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
		+----+----+----+----+----+----+----+----+----+----+....+----+
    	   1    1      2              4           variable       1

*/

struct  proxy_request{
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8];      /*Username of 7 characters and 1 NULL character*/
};
typedef struct proxy_request Req;

/*

        +----+----+----+----+----+----+----+----+
		| VN | CD | DSTPORT |      DSTIP        |
		+----+----+----+----+----+----+----+----+
    	   1    1      2              4

*/

struct proxy_response{
    int8 vn;
    int8 cd;
    int16 _;
    int32 __;
};
typedef struct proxy_response Res;


Req *request(struct sockaddr_in*);
int connect(int, const struct sockaddr*, socklen_t);