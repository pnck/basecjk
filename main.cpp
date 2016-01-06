#include <iostream>
#include <cstdio>
#include "basecjk.h"

using namespace std;

int main ()
{
    locale::global(locale(""));
    ios::sync_with_stdio(false);
    char test_buff[] = {"\x61\x00\x00\x00\x00\x00\x00\x61\x00\x00\x00\x00\xff\xff\x00\xff\xff\xff\x0f\x00"};

    string bs = BaseCJK::bcjkencodeutf8(reinterpret_cast<BaseCJK::BYTE *>(test_buff), sizeof(test_buff) - 1);
    cout<<bs<<endl;
    wcout<<BaseCJK::bcjkencodewchar(reinterpret_cast<BaseCJK::BYTE *>(test_buff), sizeof(test_buff) - 1)<<endl;
    auto v = BaseCJK::bcjkdecodeutf8(bs);
    for(auto b:v)
    {
        printf(" %02x",b);
    }
    return 0;
}