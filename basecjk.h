//
// Created by pnck on 16-1-6.
//
#include <string>
#include <vector>
//#include <unicode/ustream.h>

#ifndef BASECJK_BASECJK_H
#define BASECJK_BASECJK_H

using namespace std;

namespace BaseCJK
{
    typedef struct _BYTE
    {
        unsigned char c;

        operator char() { return (char) c; }
        operator unsigned char() { return c; }
        _BYTE() : c(0) {}
        _BYTE(char ch) : c((unsigned char) ch) {}
        _BYTE(unsigned char ch) : c(ch) {}
        _BYTE(int ch) : c(ch) {}
    }BYTE;

    string bcjkencodeutf8(const BYTE *buff, size_t len);
    vector<BYTE> bcjkdecodeutf8(const string &s);
    wstring bcjkencodewchar(const BYTE *buff, size_t len);
    vector<BYTE> bcjkdecodewchar(const wstring &s);
}

#endif //BASECJK_BASECJK_H
