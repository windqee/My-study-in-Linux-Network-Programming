#include <iostream>
#include <cstdio>
#include <fstream>
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

	bool send(void *buffer,const size_t size) {
		if(m_clientfd==-1)return false;

		if((::send(m_clientfd,buffer,size,0))<=0) {
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
	bool sendfile(const string &filename,const size_t size) {
		ifstream fin(filename,ios::binary);
		if (fin.is_open()==false) {
			cout<<"打开文件"<<filename<<"失败。\n";
			return false;
		}
		int onread=0;
		int totalbytes=0;
		char buffer[7];
		while(true) {
			memset(buffer,0,sizeof(buffer));

			if(size-totalbytes>7) onread=7;
			else onread=size-totalbytes;

			fin.read(buffer,onread);

			if(send(buffer,onread)==false) return false;

			totalbytes+=onread;
			if(totalbytes==size)break;
		}
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
    if (argc!=5)
    {
        cout << "Using:./demo11 服务端的IP 服务端的端口 \nExample:./demo11 192.168.101.138 5005\n\n";
        return -1;
    }

    ctcpclient tcpclient;
    if((tcpclient.connect(argv[1],atoi(argv[2])))==false) {
        perror("connect()");return -1;
    }
    
	struct st_fileinfo {
		char filename[256];
		int filesize;
	} fileinfo;
	memset(&fileinfo,0,sizeof fileinfo);
	strcpy(fileinfo.filename,argv[3]);
	fileinfo.filesize=atoi(argv[4]);
	if(tcpclient.send(&fileinfo,sizeof(fileinfo))==false) {
		perror("send");return -1;
	}
	cout<<"已发送文件信息的结构体"<<fileinfo.filename<<" 大小:"<<fileinfo.filesize<<endl;

	string buffer;
	if(tcpclient.recv(buffer,2)==false) {
		perror("recv()");return -1;
	}
	if(buffer!="ok") {
		cout<<"服务端没有回复ok"<<endl;return -1;
	}

	if(tcpclient.sendfile(fileinfo.filename,fileinfo.filesize)==false) {
		perror("sendfile()");return -1;
	}
	if(tcpclient.recv(buffer,2)==false) {
		perror("recv()");return -1;
	}
	if(buffer!="ok") {
		cout<<"文件内容，服务端没有回复ok"<<endl;return -1;
	}
    return 0;
}