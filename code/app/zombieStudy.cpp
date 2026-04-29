#include<sys/types.h>
#include<sys/wait.h>
#include<iostream>
#include<unistd.h>
using namespace std;
void study1() {
    if(fork()>0) {
        sleep(20);
        cout<<"父进程将退出"<<endl;
        return ;
    }
    else {
        for(int i=0; i<100; i++) {
            cout<<"子进程运行中"<<endl;
            sleep(1);
        }
    }
}
void study2() {
    for(int i=0; i<100; i++) {
        cout<<"服务运行中"<<endl;
        sleep(1);
    }
}
int study3() {
    signal(SIGCHLD, SIG_IGN);

    // 2. 创建子进程
    pid_t pid = fork();

    if (pid < 0) {
        // 错误处理：fork 失败
        perror("fork failed");
        return 1;
    } 
    else if (pid == 0) {
        // 3. 这里是子进程逻辑
        cout << "[子进程] 我诞生了，PID: " << getpid() << "。我即将退出..." << endl;
        // 子进程直接退出，由于父进程设置了 SIG_IGN，它不会变成僵尸
        return 0; 
    } 
    else {
        // 4. 这里是父进程逻辑
        cout << "[父进程] 已启动子进程，PID: " << pid << endl;
        
        while (true) {
            cout << "[父进程] 运行中，我不需要去 wait 子进程..." << endl;
            sleep(1);
        }
    }
}
void study4() {
    if(fork()>0) {
        int sts;
        pid_t pid=wait(&sts);
        cout<<"已终止的子进程编号是:"<<pid<<endl;
        if(WIFEXITED(sts))
            cout<<"子进程是正常退出的"<<endl;
        else cout<<"子进程是异常退出的，终止它的信号是:"<<WTERMSIG(sts)<<endl;
    } else {
        // sleep(30);
        int *p=0;*p=10;
        exit(0);
    }
}
int main() {
    // study1();
    // study2();
    // study3();
    study4();
    return 0;
}