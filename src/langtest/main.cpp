#include <stdio.h>

extern int stacktrace();
extern void c_array();
struct Cbstruc{
    void (*cb)();
};


void out_call(Cbstruc* cbs)
{
    cbs->cb();
}

class CTest{
public:
    CTest(){}
    virtual ~CTest() {}
    
    void run(){
        Cbstruc cbs;
        cbs.cb = m_cb;
        out_call(&cbs);
    }
    
    static void m_cb(){
        printf("this is mcb...\n");
    }
};

int main()
{
    // stacktrace();
    // c_array();
    CTest t;
    t.run();
    return 0; 
}