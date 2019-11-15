#include "StackFunk.cpp"

int main()
{
    Stack_t <int> stk;

    stk.StackInit();

    printf("lol");
    stk.Dump("test INIT");
    stk.Print();
    stk.Dump("test PRINT1");
    stk.StackPush(4);
    stk.Dump("test PUSH1");
    stk.StackPush(6);
    stk.Dump("test PUSH2");
    stk.Print();
    stk.Dump("test PRINT2");
    stk.StackPop();
    stk.Dump("test POP");
    stk.Print();
    stk.Dump("test PRINT3");



}
