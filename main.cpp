#include <iostream>
#include <cstdio>
#include <string.h>
#include "basecjk.h"

using namespace std;

int main (int argc,char *argv[])
{
    if(argc!=3) return 1;
    if(string(argv[1]) == "e")
    {
        cout<<BaseCJK::bcjkencodeutf8(reinterpret_cast<BaseCJK::BYTE *>(argv[2]),strlen(argv[2]))<<endl;
    }
    if(string(argv[1]) == "d")
    {
        auto v = BaseCJK::bcjkdecodeutf8(string(argv[2]));
        for (auto b:v)
        {
            printf("%c", b);
        }
        putchar('\n');
    }
    return 0;
}