// #include <stdio.h>

// extern int stacktrace();
// extern void c_array();
// struct Cbstruc{
//     void (*cb)();
// };


// void out_call(Cbstruc* cbs)
// {
//     cbs->cb();
// }

// class CTest{
// public:
//     CTest(){}
//     virtual ~CTest() {}
    
//     void run(){
//         Cbstruc cbs;
//         cbs.cb = m_cb;
//         out_call(&cbs);
//     }
    
//     static void m_cb(){
//         printf("this is mcb...\n");
//     }
// };

// int main()
// {
//     // stacktrace();
//     // c_array();
//     CTest t;
//     t.run();
//     return 0; 
// }


#include <memory>
#include <iostream>

using namespace std;

// class Test
// {
// public:
//     //析构函数
//     ~Test() { cout << "Test Destructor." << endl; }
//     //获取指向当前对象的指针
//     shared_ptr<Test> GetObject()
//     {
//         shared_ptr<Test> pTest(this);
//         return pTest;
//     }
// };

class Test2 : public enable_shared_from_this<Test2>        //改进1
{
public:
    //析构函数
    ~Test2() { cout << "Test Destructor." << endl; }
    //获取指向当前对象的指针
    shared_ptr<Test2> GetObject()
    {
        return shared_from_this();      //改进2
    }
};

int main(int argc, char *argv[])
{
    // {
    //     shared_ptr<Test> p( new Test( ));
    //     shared_ptr<Test> q = p->GetObject();
    // }
    
    {
        shared_ptr<Test2> p2( new Test2( ));
        // shared_ptr<Test2> q2 = p2->GetObject();
        shared_ptr<Test2> q2(p2);
    }
    
    
    return 0;
}