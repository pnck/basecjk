//
// Created by pnck on 16-1-6.
//

#include <iostream>
#include "basecjk.h"
#include <cstring>
#include <codecvt>
#include <locale>

namespace BaseCJK
{

    string bcjkencodeutf8(const BYTE *buff, size_t len)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;//magic c++11
        return conv.to_bytes(bcjkencodewchar(buff, len));
    }

    vector<BYTE> bcjkdecodeutf8(const string &s)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;//magic c++11
        return bcjkdecodewchar(conv.from_bytes(s));
    }

    wstring bcjkencodewchar(const BYTE *buff, size_t len)
    {
#ifdef _USEASM
        asm ("push %r15;");
        asm volatile ("xor %r15,%r15");
#else
        register unsigned long long r = 0;
#endif
        wstring res;
        for (const BYTE *p = buff; p < buff + len; p += 7)
        {
            if (p <= buff + len - 7)
            {
#ifdef _USEASM
                asm volatile("movq (%0),%%r15;and %1,%%r15"::"r"(p), "r"(0x00ffffffffffffff));//load 7 bytes
#else
                r = *(unsigned long long *) p;
#endif
                for (int i = 0; i < 4; ++i)
                {
                    register wchar_t wch = 0;
#ifdef _USEASM
                    asm volatile("mov %%r15w,%%ax;and $0x3fff,%%eax;mov %%eax,%0":"=m"(wch)::"rax");//put 14 bits to ch
                    asm volatile("shr $14,%r15");
#else
                    wch = ((wchar_t *) &r)[0];
                    wch &= 0x3fff;
                    r >>= 14;
#endif
                    res.push_back(wch + 0x4e01);//base = 0x4e01 , 4e00 for continuous zero
                }
            }
            else
            {
                BYTE t[8];
                int bl = buff + len - p;//bytes len
                for (int i = 0; i < bl; ++i)
                {
                    t[i] = p[i];
                }
#ifdef _USEASM
                asm volatile("movq (%0),%%r15;"::"r"(t));// load remaining bytes
#else
                r = *(unsigned long long *) t;
#endif
                //1=0;2=null     3=n||3=0;4=n    5=n||5=0;6=n  7=n||7=0
                //00000000000000 00000000000000 00000000000000 00000000000000
                //11111111222222 22333333334444 44445555555566 66666677777777
                int wl = bl * 8 / 14 + 1;//wchar len
                for (int i = 0; i < wl; ++i)
                {
                    register wchar_t wch = 0;
#ifdef _USEASM
                    asm volatile("mov %%r15w,%%ax;and $0x3fff,%%eax;mov %%eax,%0":"=m"(wch)::"rax");//put 14 bits to t
                    asm volatile("shr $14,%r15");
#else
                    wch = ((wchar_t *) &r)[0];
                    wch &= 0x3fff;
                    r >>= 14;
#endif
                    if (wch == 0)//special process for zeros
                    {
                        int bi = i * 14 / 8 + 1;//bytes index
                        if (bi < bl)// X == 0
                        {
                            res.push_back(0x4e01);
                        }
                        else// X == NULL
                        {
                            res.push_back(0x4e00);
                        }
                    }
                    else res.push_back(wch + 0x4e01);//normal
                }
            }
        }
#ifdef _USEASM
        asm ("pop %r15;");
#endif
        return res;
    }

    vector<BYTE> bcjkdecodewchar(const wstring &s)
    {
#ifdef _USEASM
        asm ("push %r15;");
#else
        register unsigned long long r;
#endif
        const wchar_t *buff = s.c_str();
        size_t len = s.size();
        vector<BYTE> res;
        for (auto p = buff; p < buff + len; p += 4)
        {
#ifdef _USEASM
            asm volatile("xor %r15,%r15");
#else
            r = 0;
#endif
            BYTE t[8];
            if (p < buff + len - 4)//normally
            {
                for (int i = 0; i < 4; ++i)//load 4*14 bits to reg
                {
                    wchar_t wch = p[3 - i];
#ifdef _USEASM
                    asm volatile("shl $14,%%r15;movl %0,%%eax;subl $0x4e01,%%eax;or %%rax,%%r15"::"r"(wch):"rax", "r15");
                }
                asm volatile("movq %%r15,%0":"=m"(t)::"memory");//copy to temp
#else
                    r <<= 14;
                    wch -= 0x4e01;
                    r |= wch;
                }
                *(unsigned long long *) t = r;
#endif
                for (int i = 0; i < 7; ++i)
                {
                    res.push_back(t[i]);//append 7 bytes
                }
            }
            else
            {
                //1=0;2=null     3=n||3=0;4=n    5=n||5=0;6=n  7=n||7=0
                //00000000000000 00000000000000 00000000000000 00000000000000
                //11111111222222 22333333334444 44445555555566 66666677777777
                //00000000000000 00000000001111 11110000000000 000000******** // 00 00 00 ff 00 00
                int wl = buff + len - p;//remaining len
                bool flg_null_tail = false;
                for (int i = wl - 1; i >= 0; --i)
                {
                    wchar_t wch = 0;
                    if (p[i] == 0x4e00)
                    {
                        flg_null_tail = true;//tail == NULL, decrease 1byte
                    } else
                    {
                        wch = p[i] - 0x4e01;
                    }
#ifdef _USEASM
                    asm volatile("shl $14,%%r15;movl %0,%%eax;or %%rax,%%r15"::"r"(wch):"r15", "rax");
                }
                asm volatile("movq %%r15,%0":"=m"(t)::"memory");//copy to temp
#else
                    r <<= 14;
                    r |= wch;
                }
                *(unsigned long long *) t = r;
#endif
                int bl = wl * 14 / 8 - flg_null_tail;//bytes len
                if (bl == 0)bl = 1;//fix to 1 if calced bl == 0
                for (int i = 0; i < bl; ++i)
                {
                    res.push_back(t[i]);
                }
            }
        }
#ifdef _USEASM
        asm ("pop %r15");
#endif
        return res;
    }
}