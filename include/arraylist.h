#ifndef RTS_ARRAYLIST_H
#define RTS_ARRAYLIST_H

#include <stddef.h>

// Dynamic array, initialize with ArrayListInit
// Stores copies of elements in contiguous heap memory
// Frees internal elements upon destruction by arrayListDestroy
// length        - amount of elements in array
// elementSize   - size_t of one element
// elements      - pointer to an array of stored elements
typedef struct ArrayList {
    int    length;
    size_t elementSize;
    void*  elements;
} ArrayList;

// Returns an initialized ArrayList
// Returns nullptr on error
ArrayList* ArrayListInit(size_t elementSize);

// Safely frees the provided ArrayList and its elements
void arrayListDestroy(ArrayList* arrayList);

// Adds an element to an ArrayList by copy
void arrayListAdd(ArrayList* arrayList, const void* element);

// Removes an element from an arraylist, if it is present at the provided index
void arrayListRemoveAt(ArrayList* arrayList, int index);

// Returns a pointer to the element at index
// Returns a nullptr on error
void* arrayListGet(const ArrayList* arrayList, int index);

#endif