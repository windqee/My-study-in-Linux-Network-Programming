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

class ctcpclient {
public:
    int m_clientfd;
    string m_ip;
    unsigned short m_port;
    ctcpclient():m_clientfd(-1){}
    //像服务端发起连接请求，成功返回true，失败返回false 
    bool connect(const string &in_ip,const unsigned short in_port) {
        if (m_clientfd!=-1) return false; // 如果socket已连接，直接返回失败。

        m_ip=in_ip; m_port=in_port;       // 把服务端的IP和端口保存到成员变量中。

        // 第1步：创建客户端的socket。
        if ((m_clientfd = socket(AF_INET, SOCK_STREAM, 0))==-1) return false;

        // 第2步：向服务器发起连接请求。
        struct sockaddr_in servaddr;                   // 用于存放协议、端口和IP地址的结构体。
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;                 // ①协议族，固定填AF_INET。
        servaddr.sin_port = htons(m_port);             // ②指定服务端的通信端口。

        struct hostent* h;                              // 用于存放服务端IP地址(大端序)的结构体的指针。
        if ((h = gethostbyname(m_ip.c_str())) == nullptr )      // 把域名/主机名/字符串格式的IP转换成结构体。
        {
            ::close(m_clientfd); m_clientfd=-1; return false;
        }
        memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);

        if((::connect(m_clientfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)) {
            ::close(m_clientfd); m_clientfd=-1; return false;
        }
        return true;
    }
	bool send(const string &buffer) {
		if(m_clientfd==-1)return false;

		if((::send(m_clientfd,buffer.data(),buffer.size(),0))<=0) {
			return false;
		}
		return true;
	}

	bool recv(string &buffer,const size_t maxlen) {
		buffer.clear();
		buffer.resize(maxlen);
		int readn=::recv(m_clientfd,&buffer[0],buffer.size(),0);
		if(readn<=0){buffer.clear();return false;}
		buffer.resize(readn);
		return true;
	}
	bool close() {
		if(m_clientfd==-1)return false;
		::close(m_clientfd);
		m_clientfd=-1;
		return true;
	}
    ~ctcpclient(){close();}
} ;

int main(int argc, char *argv[])
{
    if (argc!=3)
    {
        cout << "Using:./demo7 服务端的IP 服务端的端口\nExample:./demo7 192.168.101.138 5005\n\n";
        return -1;
    }

    ctcpclient tcpclient;
    if((tcpclient.connect(argv[1],atoi(argv[2])))==false) {
        perror("connect()");return -1;
    }
    
	int iret;
	string buffer;
	for(int i=0; i<10; i++) {
		buffer="这是第"+to_string(i+1)+"条信息\n";

        if(tcpclient.send(buffer)==false) {
			perror("send()");return -1;
		}
        cout<<"发送: "<<buffer<<endl;

        if(tcpclient.recv(buffer,1024)==false) {
            cout<<"iret="<<iret<<endl;break;
        }
        cout<<"接收:"<<buffer<<endl;
        sleep(1);
    }
    return 0;
}