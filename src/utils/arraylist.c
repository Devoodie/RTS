#include "arraylist.h"

#include <stdlib.h>

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

