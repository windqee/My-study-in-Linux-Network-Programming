#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc!=2)
    {
        cout << "Using:./demo2 通讯端口\nExample:./demo2 5005\n\n";   // 端口大于1024，不与其它的重复
        cout << "注意：运行服务端程序的Linux系统的防火墙必须要开通5005端口。\n";
        cout << "      如果是云服务器，还要开通云平台的访问策略。\n\n";
        return -1;
    }
    // 第1步：创建服务端的socket。
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd==-1)
    {
        perror("socket"); return -1;
    }

    // 第2步：把服务端用于通信的IP和端口绑定到socket上。
    struct sockaddr_in servaddr;          // 用于存放服务端IP和端口的数据结构。
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;        // 指定协议。
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 服务端任意网卡的IP都可以用于通讯。
    servaddr.sin_port = htons(atoi(argv[1]));    // 指定通信端口，普通用户只能用1024以上的端口。
    // 绑定服务端的IP和端口。
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0 )
    {
        perror("bind"); close(listenfd); return -1;
    }
    return 0;
}