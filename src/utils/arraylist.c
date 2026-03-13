#include "arraylist.h"

#include <stdlib.h>
#include <string.h>

void replaceElements(ArrayList* arrayList, void* newElements) {
    void* oldElements = arrayList->elements;
    arrayList->elements = newElements;
    arrayList->length++;
    free(oldElements);
}

ArrayList* ArrayListInit(const size_t elementSize) {
    const auto arrayList = (ArrayList*)malloc(sizeof(ArrayList));

    if (arrayList == nullptr) return nullptr;

    arrayList->length      = 0;
    arrayList->elementSize = elementSize;
    arrayList->elements    = nullptr;

    return arrayList;
}

void arrayListDestroy(ArrayList* arrayList) {
    if (arrayList == nullptr) return;

    if (arrayList->length > 0 && arrayList->elements != nullptr) {
        for (int i = 0; i < arrayList->length; i++) {
            free(&arrayList->elements[i]);
        }

        free(arrayList->elements);
    }

    free(arrayList);
}

void arrayListAdd(ArrayList* arrayList, const void* element) {
    if (arrayList == nullptr || sizeof(element) != arrayList->elementSize)
        return;

    void* newElements =
        malloc(sizeof(arrayList->elementSize) * (arrayList->length + 1));
    if (newElements == nullptr) return;

    memcpy(newElements, arrayList->elements, sizeof(arrayList->elementSize) * (arrayList->length));
    memcpy(newElements + arrayList->length, element, sizeof(element));

    replaceElements(arrayList, newElements);
}

void arrayListRemoveAt(ArrayList* arrayList, const int index) {
    if (arrayList == nullptr ||
        index > arrayList->length - 1 ||
        arrayList->elements == nullptr)
        return;

    free(&arrayList->elements[index]);

    void* newElements =
        malloc(sizeof(arrayList->elementSize) * (arrayList->length + 1));
    if (newElements == nullptr) return;

    int curIndex = 0;
    for (int i = 0; i < arrayList->length; i++) {
        if (i == index) continue;

        memcpy(newElements, &arrayList->elements[curIndex], sizeof(arrayList->elementSize));
        curIndex++;
    }

    replaceElements(arrayList, newElements);
}