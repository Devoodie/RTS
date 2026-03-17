#include "../../include/utils/arraylist.h"

#include <stdint.h>
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

static void addElement(ArrayList* arrayList, const void* element) {
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
    if (elementSize == 0 || capacity == 0) return nullptr;
    if (capacity > SIZE_MAX / elementSize) return nullptr;

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
    free(arrayList->elements);
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
    if (arrayList->capacity > SIZE_MAX / 2 / arrayList->elementSize) return;

    const size_t newSize =
        (size_t)arrayList->capacity * 2 * arrayList->elementSize;

    void* newElements = realloc(
            arrayList->elements,
            newSize
            );

    if (newElements == nullptr) return;

    arrayList->elements = newElements;
    arrayList->capacity *= 2;

    addElement(arrayList, element);
}

void arrayListPop(ArrayList* arrayList) {
    if (arrayList == nullptr ||
        arrayList->length == 0)
        return;

    arrayList->length--;
}

void arrayListRemoveAt(ArrayList* arrayList, const unsigned int index)
{
    if (!arrayList || !arrayList->elements || index >= arrayList->length)
        return;

    const auto elements = (char*)arrayList->elements;

    memmove(
        elements + index * arrayList->elementSize,
        elements + (index + 1) * arrayList->elementSize,
        (arrayList->length - index - 1) * arrayList->elementSize
    );

    arrayList->length--;
}

void arrayListClear(ArrayList* arrayList, const unsigned int capacity) {
    if (arrayList == nullptr || capacity == 0) return;
    if (capacity > SIZE_MAX / arrayList->elementSize) return;

    void* newElements = calloc(capacity, arrayList->elementSize);

    if (newElements == nullptr) return;

    free(arrayList->elements);

    arrayList->length      = 0;
    arrayList->capacity    = capacity;
    arrayList->elements    = newElements;
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
        index >= arrayList->length ||
        index < 0) return nullptr;

    return (char*)arrayList->elements + index * arrayList->elementSize;
}