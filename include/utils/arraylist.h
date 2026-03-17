#ifndef RTS_ARRAYLIST_H
#define RTS_ARRAYLIST_H

#include <stddef.h>

// Dynamic array that stores copies of provided elements in contiguous memory
//
// METHODS -
// ArrayListInit
// arrayListFree
// arrayListAppend
// arrayListPop
// arrayListClear
//
// GETTERS -
// ArrayListGetLength
// ArrayListGetCapacity
// ArrayListGetElementSize
// ArrayListAt
typedef struct ArrayList ArrayList;


// Returns an initialized ArrayList
// Returns nullptr on error
//
// elementSize - size_t of one element
// capacity    - initial size of array
ArrayList* ArrayListInit(size_t elementSize, unsigned int capacity);

// Frees the provided ArrayList and its internal buffer
void arrayListFree(ArrayList* arrayList);

// Adds an element to an ArrayList
//
// arrayList - ArrayList to perform the operation on
// element   - element to add to arrayList
void arrayListAppend(ArrayList* arrayList, const void* element);

// Removes the last appended element of arrayList
void arrayListPop(ArrayList* arrayList);

// Removes the element of arrayList at index
void arrayListRemoveAt(ArrayList* arrayList, unsigned int index);

// Re-initializes the arrayList internal buffer to capacity
//
// arrayList - ArrayList to perform the operation on
// capacity  - The new capacity to assign to the ArrayList
void arrayListClear(ArrayList* arrayList, unsigned int capacity);

// Returns the current length of arrayList
// Returns -1 on error
int ArrayListGetLength(const ArrayList* arrayList);

// Returns the capacity of arrayList
// Returns 0 on error
unsigned int ArrayListGetCapacity(const ArrayList* arrayList);

// Returns the elementSize of arrayList
// Returns 0 on error
size_t ArrayListGetElementSize(const ArrayList* arrayList);

// Returns a pointer to the element stored in the list at index
// Returns a nullptr if the index or arrayList is invalid
//
// arrayList - The ArrayList to perform the operation on
// index     - The index to return a pointer to
void* ArrayListAt(const ArrayList* arrayList, int index);

#endif