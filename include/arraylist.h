#ifndef RTS_ARRAYLIST_H
#define RTS_ARRAYLIST_H

#include <stddef.h>

// Dynamic array that stores copies of provided elements in contiguous memory
//
// Initialize with ArrayListInit
// Free with arrayListDestroy
//
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

// Frees the provided ArrayList and its elements
void arrayListDestroy(ArrayList* arrayList);

// Adds an element by copying elementSize bytes into the list
void arrayListAdd(ArrayList* arrayList, const void* element);

// Removes an element from an ArrayList if the index is valid
void arrayListRemoveAt(ArrayList* arrayList, int index);

// Returns a pointer to the element stored in the list at index
// Returns a nullptr if the index or arrayList is invalid
void* arrayListGet(const ArrayList* arrayList, int index);

#endif