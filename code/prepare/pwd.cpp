#include<iostream>
#include<unistd.h>
#include<dirent.h>
using namespace std;

int main(int argc,char *argv[]) {
    char path1[256];
    getcwd(path1,256);
    cout<<"path1="<<path1<<endl;

    char *path2=get_current_dir_name();
    cout<<"path2="<<path2<<endl;
    free(path2);

    if(argc!=2) {cout<<"Using ./demo 目录名\n";return -1;}
    DIR *dir;
    if((dir=opendir(argv[1]))==0)return -1;

    struct dirent *stdinfo=nullptr;
    while(true) {
        if((stdinfo=readdir(dir))==0)break;
        cout<<"文件名:"<<stdinfo->d_name<<",文件类型="<<(int)stdinfo->d_type<<endl;
    }
    closedir(dir);
    return 0;
}