#include "../../include/utils/arraylist.h"

#include <stdlib.h>
#include <string.h>

typedef struct ArrayList {
    // Number of elements current in the ArrayList
    int          length;
    // Number of elements that can be stored in the current buffer
    unsigned int capacity;
    // size_t of one element
    size_t       elementSize;
    // Pointer to the current buffer
    void*        elements;
} ArrayList;

void addElement(ArrayList* arrayList, const void* element) {
    memcpy(
        (char*)arrayList->elements +
        arrayList->elementSize *
        arrayList->length,

            element,

            arrayList->elementSize
            );

    arrayList->length++;
}

ArrayList* ArrayListInit(const size_t elementSize,
    const unsigned int capacity)
{
    if (elementSize <= 0 || capacity <= 0) return nullptr;

    ArrayList* arrayList = malloc(sizeof(ArrayList));

    if (arrayList == nullptr) return nullptr;

    arrayList->length      = 0;
    arrayList->capacity    = capacity;
    arrayList->elementSize = elementSize;
    arrayList->elements    = calloc(capacity, elementSize);

    if (arrayList->elements == nullptr) {
        free(arrayList);
        return nullptr;
    }

    return arrayList;
}

void arrayListFree(ArrayList* arrayList) {
    if (arrayList == nullptr) return;

    if (arrayList->elements != nullptr) {
        free(arrayList->elements);
    }

    free(arrayList);
}

void arrayListAppend(ArrayList* arrayList, const void* element) {
    if (arrayList == nullptr ||
        element == nullptr)
        return;

    // Capacity not reached, no realloc required
    if (arrayList->capacity >= arrayList->length + 1) {
        addElement(arrayList, element);
        return;
    }

    // Double the capacity before adding new elements
    void* newElements = realloc(
            arrayList->elements,
            arrayList->capacity * 2 * arrayList->elementSize
            );

    if (newElements == nullptr) return;

    arrayList->elements = newElements;
    arrayList->capacity *= 2;

    addElement(arrayList, element);
}

void arrayListPop(ArrayList* arrayList) {
    if (arrayList == nullptr ||
        arrayList->elements == nullptr ||
        arrayList->length == 0)
        return;

    arrayList->length--;
}

void arrayListClear(ArrayList* arrayList, const unsigned int capacity) {
    if (arrayList == nullptr) return;

    if (arrayList->elements != nullptr) {
        free(arrayList->elements);
    }

    arrayList->length      = 0;
    arrayList->capacity    = capacity;
    arrayList->elements    = calloc(capacity, arrayList->elementSize);
}

int ArrayListGetLength(const ArrayList* arrayList) {
    if (arrayList == nullptr) return -1;
    return arrayList->length;
}

unsigned int ArrayListGetCapacity(const ArrayList* arrayList) {
    if (arrayList == nullptr) return 0;
    return arrayList->capacity;
}

size_t ArrayListGetElementSize(const ArrayList* arrayList) {
    if (arrayList == nullptr) return 0;
    return arrayList->elementSize;
}

void* ArrayListAt(const ArrayList* arrayList, const int index) {
    if (arrayList == nullptr ||
        arrayList->elements == nullptr ||
        index > arrayList->length - 1 ||
        index < 0) return nullptr;

    return (char*)arrayList->elements + index * arrayList->elementSize;
}