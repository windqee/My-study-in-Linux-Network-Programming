#include<iostream>
#include<cstring>
#include<cerrno>
#include<sys/stat.h>
using namespace std;

int main() {
    for(int i=0; i<150; i++) {
        cout<<i<<":"<<strerror(i)<<endl;
    }
    int iret=mkdir("/tmp/aaa/bb/cc/dd",0755);
    cout<<"iret="<<iret<<endl;
    cout<<errno<<":"<<strerror(errno)<<endl;
    perror("调用mkdir()失败");
    return 0;
}