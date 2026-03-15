#include "arraylist.h"

#include <stdlib.h>
#include <string.h>

void replaceElements(ArrayList* arrayList, void* newElements, int newLength) {
    void* oldElements = arrayList->elements;
    arrayList->elements = newElements;
    arrayList->length = newLength;
    free(oldElements);
}

ArrayList* ArrayListInit(const size_t elementSize, const unsigned int capacity) {
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

void arrayListDestroy(ArrayList* arrayList) {
    if (arrayList == nullptr) return;

    if (arrayList->elements != nullptr) {
        free(arrayList->elements);
    }

    free(arrayList);
}

void arrayListAdd(ArrayList* arrayList, const void* element) {
    if (arrayList == nullptr ||
        element == nullptr)
        return;

    void* newElements =
        malloc(arrayList->elementSize * (arrayList->length + 1));
    if (newElements == nullptr) return;

    memcpy(newElements,
        arrayList->elements,
        arrayList->elementSize * (arrayList->length));

    memcpy((char*)newElements + arrayList->elementSize * arrayList->length,
        element,
        arrayList->elementSize);

    replaceElements(arrayList, newElements, arrayList->length + 1);
}

void arrayListRemoveAt(ArrayList* arrayList, const int index) {
    if (arrayList == nullptr ||
        index > arrayList->length - 1 ||
        index < 0 ||
        arrayList->elements == nullptr)
        return;

    void* newElements = nullptr;

    if (arrayList->length - 1 > 0) {
        newElements = malloc(arrayList->elementSize * (arrayList->length - 1));
        if (newElements == nullptr) return;
    }

    int curIndex = 0;
    for (int i = 0; i < arrayList->length; i++) {
        if (i == index) continue;

        memcpy((char*)newElements + arrayList->elementSize * curIndex,
            arrayListGet(arrayList, i),
            arrayList->elementSize);

        curIndex++;
    }

    replaceElements(arrayList, newElements, arrayList->length - 1);
}

void* arrayListGet(const ArrayList* arrayList, const int index) {
    if (arrayList == nullptr ||
        arrayList->elements == nullptr ||
        index > arrayList->length - 1 ||
        index < 0) return nullptr;

    return (char*)arrayList->elements + index * arrayList->elementSize;
}