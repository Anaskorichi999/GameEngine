#pragma ones
#include <stdlib.h>
#include <stdio.h>
typedef struct ArrayList
{
    void *Array;
    size_t arraysize;   
    size_t msize;
} ArrayList;

ArrayList NewArrayList(size_t arraysize,size_t msize)
{
    ArrayList arraylist;
    arraylist.Array = malloc(arraysize * msize);
    arraylist.arraysize = arraysize;
    arraylist.msize = msize;
    return arraylist;
}

void *AddToArray(void* m,ArrayList arraylist)
{
    arraylist.Array = realloc(arraylist.Array,(arraylist.arraysize+1) * arraylist.size);
    if(arraylist.Array == NULL)
        fprintf(stderr,"ERRER: CANT ALLOCET MEMERY :( ");
    return (arraylist.size*arraylist.arraysize)+&arraylist.Array;
}
void RemoveFromArray(void* m,ArrayList arraylist)
{
    arraylist.Array = realloc(arraylist.Array,(arraylist.arraysize-1) * arraylist.size);
}