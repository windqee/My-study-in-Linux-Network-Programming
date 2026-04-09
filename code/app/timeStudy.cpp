#include<iostream>
#include<time.h>
using namespace std;

int main() {
    time_t now1(0);
    long now2;
    time(&now2);
    cout<<"now1="<<now1<<endl;
    cout<<"now2="<<now2<<endl;
    exit(1);
    // return 0;
}