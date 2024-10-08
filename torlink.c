/* torlink.c */


/*

1. Turn the client into a lib (shared library) .so
2. Turn main() into our own connect()
3. Replace regular connect()
4. Grab the IP and PORT from the orginal connect()

*/


#include "torlink.h"


Req *request(struct sockaddr_in *sock2){
    Req *req;

    req = malloc(reqsize);

    req->vn = 4;
    req->cd = 1;
    req->dstport = sock2->sin_port;
    req->dstip = sock2->sin_addr.s_addr;
    strncpy(req->userid, USERNAME, 8);

    return req;
}

/*

./torlink 1.2.3.4 80

*/

int connect(int s2, const struct sockaddr *sock2, socklen_t addrlen){

    int s;      /*The port number in the server.*/
    Req *req;
    Res *res;
    char buf[ressize];
    int success;    /*predicate*/
    char tmp[512];

    int (*p)(int, const struct sockaddr*, socklen_t);

    struct sockaddr_in sock;

    
    p = dlsym(RTLD_NEXT, "connect");
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s<0){
	perror("socket");

	return -1;
    }    

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    if(p(s, (struct sockaddr *)&sock, sizeof(sock))<0){
        perror("connect");

        return -1;
    }

    printf("Connected to proxy.\n");

    req = request((struct sockaddr_in *)sock2);
    write(s, req, reqsize);

    memset(buf, 0, ressize);
    if (read(s, buf, ressize)<1){
        perror("read");
        free(req);
        close(s);

        return -1;
    }


    res = (Res *)buf;
    success = (res->cd == 90);

    if (!success){
        fprintf(stderr, "Unable to traverse the proxy, error code: %d\n", res->cd);
        close(s);
        free(req);

        return -1;
    }

    printf("Successfully connected throught the proxy.\n");


    dup2(s, s2);
    free(req);

    return 0;

}
