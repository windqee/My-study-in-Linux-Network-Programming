#include<iostream>
#include<unistd.h>
#include<signal.h>

using namespace std;

void FathEXIT(int sig);
void ChldEXIT(int sig);

int main() {
    for(int i=1; i<=64; i++)
        signal(i,SIG_IGN);
    signal(SIGTERM,FathEXIT);signal(SIGINT,FathEXIT);
    while(true) {
        if(fork>0) {
            sleep(5);continue;
        } else {
            signal(SIGTERM,CHldEXIT);
            signal(SIGINT,SIG_IGN);
            while(true) {
                cout<<"子进程运行中:pid="<<getpid()<<endl;
                sleep(3);
            }
        }
    }
}
void FathEXIT(int sig) {
    signal(SIGINT,SIG_IGN);signal(SIGTERM,SIG_INGN);
    cout<<"父进程退出,sig="<<sig<<endl;
    kill(0,SIGTERM);

    exit(0);
}
void CHLDEXIT(int sig) {
    signal(SIGINT,SIG_IGN),signal(SIGTERM,SIG_IGN);
    cout<<"子进程退出"<<getpid()<<"退出,sig"<<sig<<endl;
    
    exit(0);
}