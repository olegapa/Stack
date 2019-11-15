#include "divine_stack.cpp"
#define DEFAULT 20

#define eps 0.000001

template <typename elem_t>
elem_t* Stack_t<elem_t>:: StackRealloc(int new_maxsize)
{
    Ok();

    elem_t* new_data = new elem_t(new_maxsize) + (elem_t) (new canary_t(2));

    assert(new_data != NULL);

    can1 = (canary_t*) new_data;
    *can1 = 0xbeda;

    new_data = (elem_t*) ((canary_t*)new_data + 1);

    int i = 0;
    while(i < size)
    {
        new_data[i] = data[i];
        i++;
    }

    while(i <= new_maxsize)
    {
        new_data[i] = POISON;
        i++;
    }

    can2 = (canary_t*) (new_data + new_maxsize + 1);
    *can2 = 0xdead;

    Ok();

    return new_data;
}

template <typename elem_t>
BOOL Stack_t <elem_t>:: StackInit ()
{


    data = new elem_t(DEFAULT+3);

    can1 = (canary_t*) data;

    *can1 = 0xbeda;

    data = (elem_t*) ((canary_t*)data + 1);

    size = 0;
    maxsize = DEFAULT - 1;

    can2 = (canary_t*) (data + maxsize + 1);
    *can2 = 0xdead;

    assert(data != NULL);

    FillStackPoison();


    hash = MurmurHash2();

    Ok();
//    printf("f");
    return TRUE;
}

template <typename elem_t>
BOOL Stack_t <elem_t>:: StackPush (elem_t element)
{
    Ok();

    if(maxsize == size)
        data = StackRealloc(maxsize*2);

    data[size] = element;
    size++;

    hash = MurmurHash2();

    Ok();

    return TRUE;
}

template <typename elem_t>
elem_t Stack_t<elem_t>:: StackPop ()
{
    Ok();

    elem_t value;

    size--;

    value = data[size];

    data[size] = POISON;

    hash = MurmurHash2();

    if(size <= maxsize/3)
        data =  StackRealloc(maxsize);

    Ok();

    return value;
}

template <typename elem_t>
BOOL Stack_t <elem_t>:: FillStackPoison()
{

    for(int i = size; i <= maxsize; i++)
        data[i] = POISON;

    return TRUE;

}

template <typename elem_t>
void Stack_t <elem_t>:: Print()
{
    for(int i = 0; i < size; i++)
    {
        printf(ELM, data[i]);

        printf("   ");
    }

}

template <typename elem_t>
unsigned int Stack_t <elem_t>:: MurmurHash2()
{

    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned int h = seed ^ size;

    const unsigned char * key = (const unsigned char *)data;
    unsigned int k;

    while (size >= 4)
    {
        k  = key[0];
        k |= key[1] << 8;
        k |= key[2] << 16;
        k |= key[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        key += 4;
        size -= 4;
    }

    switch (size)
    {
    case 3:
        h ^= key[2] << 16;

    case 2:
        h ^= key[1] << 8;

    case 1:
        h ^= key[0];
        h *= m;
    default:
        return ERROR;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

template <typename elem_t>
void Stack_t <elem_t>:: Dump(const char* reason)
{
    printf("Stuck_t stk  DUMP reason: %s\n{\n\tsize = %d\n\tmaxsize = %d\n\tdata:\n\t{\n\t\tcan1 = %x\n\t\t", reason, size, maxsize, *can1);
    for(int i = 0; i <= maxsize; i++)
    {
        if(data[i] != POISON)
            printf("*[%d] = " ELM"\n\t\t", i, data[i]);

        else
            printf("[%d] = " ELM" (POISON)\n\t\t", i, data[i]);
    }

    printf("can2 = %x\n", *can2);

    printf("\t}\n}\n");

}

template <typename elem_t>
double Stack_t <elem_t>:: Ok()
{
//    printf("DDD");
    if(abs(hash - MurmurHash2()) >= eps)
    {
        Dump("wrong hash");
        return ERROR;
    }

    if(*can1 != 0xbeda || *can2 != 0xdead)
    {
        printf("LOL");
        Dump("wrong canary");
        return ERROR;

    }

    if(size >= maxsize)
    {
        Dump("wrong size (size >= maxsize");
        return ERROR;

    }

    int i = 0;
    while(i < size)
    {
        if(data[i] == POISON)
        {
            Dump("stack have poison elemen");
            return ERROR;
        }

        i++;
    }

    while(i <= maxsize)
    {
        if(data[i] != POISON)
        {
            Dump("elemen out of stack");
            return ERROR;
        }

        i++;
    }
    return 0;

}
