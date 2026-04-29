#include<iostream>
using namespace std;
int main(int argc,char *argv[],char *envp[]) {
    cout<<"一共有"<<argc<<"个参数\n";
    for(int i=0; i<argc; i++) {
        cout<<"第"<<i<<"个参数是"<<argv[i]<<'\n';
    }
    for(int i=0; envp[i]!=0; i++) {
        cout<<envp[i]<<'\n';
    }
    return 0;
}