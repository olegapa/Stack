#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define ERROR 2394239
#define ELM "%d"

typedef unsigned int canary_t;

enum BOOL
{
    FALSE,
    TRUE,
};

template <typename elem_t>
class Stack_t{

private:
    int size;
    int maxsize;

    elem_t* data;

    double hash;

    canary_t* can1;
    canary_t* can2;

    const elem_t POISON = -666;
    const double error = ERROR;

public:
    Stack_t ();
    BOOL FillStackPoison();

    BOOL StackPush (elem_t element);
    elem_t StackPop ();

    void Print();

    BOOL StackDistr();

    elem_t* StackRealloc(int new_maxsize);

    unsigned int MurmurHash2();

    double Ok();
    void Dump(const char* reason);

    BOOL StackDestr();

};

