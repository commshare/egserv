#include <stdio.h>

void a(int len)
{
    while(len < 100){
        len = len * 2;
    }
    char* buf = new char[len];
    len += 5;
    delete buf;
    buf = new char[len];
    int i = 0;
    while(i++ < len-1){
        buf[i] = 'i';
    }
    buf[len-1] = 0;
    printf("--%s--%d--\n", buf, len);
}

void c_array()
{
    a(5);
}
