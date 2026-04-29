/*
 * 程序名：demo8.cpp，此程序用于演示封装socket通讯的服务端
 */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>  // 必须加
using namespace std;

class ctcpserver         // TCP通讯的服务端类。
{
private:
    int         m_listenfd;      // 监听的socket，-1表示未初始化。
    int         m_clientfd;      // 客户端连上来的socket，-1表示客户端未连接。
    string      m_clientip;      // 客户端的IP。
    unsigned short m_port;       // 服务端用于通讯的端口。
public:
    ctcpserver():m_listenfd(-1),m_clientfd(-1) {}

    // 初始化服务端用于监听的socket。
    bool initserver(const unsigned short in_port)
    {
        // 第1步：创建服务端的socket。
        if ((m_listenfd=socket(AF_INET, SOCK_STREAM, 0))==-1) return false;

        m_port=in_port;

        // 第2步：把服务端用于通信的IP和端口绑定到socket上。
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(m_port);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        // 绑定
        if (bind(m_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
        {
            close(m_listenfd);
            m_listenfd = -1;
            return false;
        }
        
        // 监听
        if (listen(m_listenfd, 5) == -1) {
            close(m_listenfd);  // 修复：只关闭监听fd
            m_listenfd = -1;
            return false;
        }
        return true;
    }

    // 接受客户端连接
    bool accept() {
        if (m_listenfd == -1) return false;

        struct sockaddr_in caddr;
        socklen_t addrlen = sizeof(caddr);
        
        m_clientfd = ::accept(m_listenfd, (struct sockaddr *)&caddr, &addrlen);
        if (m_clientfd == -1) return false;

        // 正确获取客户端IP
        m_clientip = inet_ntoa(caddr.sin_addr);
        return true;
    }

    const string & clientip() const {
        return m_clientip;
    }

    // 发送数据
    bool send(const string &buffer) {
        if (m_clientfd == -1) return false;
        int ret = ::send(m_clientfd, buffer.data(), buffer.size(), 0);
        return (ret > 0);
    }

    // 接收数据
    bool recv(string &buffer, size_t maxlen) {
        buffer.clear();
        buffer.resize(maxlen);
        int readn = ::recv(m_clientfd, &buffer[0], buffer.size(), 0);
        
        if (readn <= 0) {
            buffer.clear();
            return false;
        }
        
        buffer.resize(readn);
        return true;
    }

    // 关闭监听socket
    bool closelisten() {
        if (m_listenfd == -1) return false;
        ::close(m_listenfd);
        m_listenfd = -1;
        return true;
    }

    // 关闭客户端socket
    bool closeclient() {
        if (m_clientfd == -1) return false;
        ::close(m_clientfd);
        m_clientfd = -1;
        return true;
    }

    ~ctcpserver() {
        closeclient();
        closelisten();
    }
};

int main(int argc,char *argv[])
{
    if (argc != 2)
    {
        cout << "Using:./demo8 通讯端口\nExample:./demo8 5005\n\n";
        cout << "注意：运行服务端程序的Linux系统的防火墙必须要开通5005端口。\n";
        cout << "      如果是云服务器，还要开通云平台的访问策略。\n\n";
        return -1;
    }

    ctcpserver tcpserver;
    if (tcpserver.initserver(atoi(argv[1])) == false)
    {
        perror("initserver()");
        return -1;
    }

    // 接受客户端连接
    if (tcpserver.accept() == false) {
        perror("accept()");
        return -1;
    }
    cout << "客户端已连接(" << tcpserver.clientip() << ")" << endl;

    string buffer;
    while (true) {
        if (tcpserver.recv(buffer, 1024) == false) {
            cout << "客户端断开连接" << endl;
            break;
        }
        cout << "接收: " << buffer << endl;
        
        string sendbuf = "ok";
        if (tcpserver.send(sendbuf) == false) {
            perror("send");
            break;
        }
        cout << "发送: " << sendbuf << endl;
    }

    return 0;
}