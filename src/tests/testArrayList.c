/*
 * DISCLAIMER: TESTS ARE AI GENERATED
 */

#include "../../include/utils/arraylist.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>

typedef struct TestStruct {
    int id;
    double value;
} TestStruct;

static double nowSeconds(void) {
    return (double)clock() / (double)CLOCKS_PER_SEC;
}

static void printTiming(const char* label, double seconds) {
    printf("%s: %.6f seconds\n", label, seconds);
}

static void testInit(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);

    assert(list != nullptr);
    assert(list->length == 0);
    assert(list->capacity == 1);
    assert(list->elementSize == sizeof(int));
    assert(list->elements != nullptr);

    arrayListDestroy(list);
}

static void testInitWithLargerCapacity(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 8);

    assert(list != nullptr);
    assert(list->length == 0);
    assert(list->capacity == 8);
    assert(list->elementSize == sizeof(int));
    assert(list->elements != nullptr);

    arrayListDestroy(list);
}

static void testGetOnEmptyListReturnsnullptr(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);

    assert(list != nullptr);
    assert(arrayListGet(list, 0) == nullptr);
    assert(arrayListGet(list, -1) == nullptr);

    arrayListDestroy(list);
}

static void testAddSingleInt(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    int value = 42;
    arrayListAdd(list, &value);

    assert(list->length == 1);
    assert(list->capacity >= 1);
    assert(list->elements != nullptr);

    int* storedValue = (int*)arrayListGet(list, 0);
    assert(storedValue != nullptr);
    assert(*storedValue == 42);

    arrayListDestroy(list);
}

static void testAddMultipleInts(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    unsigned int previousCapacity = list->capacity;

    for (int i = 0; i < 10; i++) {
        arrayListAdd(list, &i);
        assert(list->length == i + 1);
        assert(list->capacity >= (unsigned int)list->length);

        if (list->capacity != previousCapacity) {
            assert(list->capacity > previousCapacity);
            previousCapacity = list->capacity;
        }
    }

    assert(list->length == 10);

    for (int i = 0; i < 10; i++) {
        int* storedValue = (int*)arrayListGet(list, i);
        assert(storedValue != nullptr);
        assert(*storedValue == i);
    }

    arrayListDestroy(list);
}

static void testCapacityGrowsWhenNeeded(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 2);
    assert(list != nullptr);
    assert(list->capacity == 2);

    int a = 1;
    int b = 2;
    int c = 3;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);

    assert(list->length == 2);
    assert(list->capacity >= 2);

    unsigned int capacityBeforeGrowth = list->capacity;

    arrayListAdd(list, &c);

    assert(list->length == 3);
    assert(list->capacity >= 3);
    assert(list->capacity >= capacityBeforeGrowth);

    if (capacityBeforeGrowth == 2) {
        assert(list->capacity > 2);
    }

    assert(*(int*)arrayListGet(list, 0) == 1);
    assert(*(int*)arrayListGet(list, 1) == 2);
    assert(*(int*)arrayListGet(list, 2) == 3);

    arrayListDestroy(list);
}

static void testAddCopiesValue(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    int value = 5;
    arrayListAdd(list, &value);

    value = 999;

    int* storedValue = (int*)arrayListGet(list, 0);
    assert(storedValue != nullptr);
    assert(*storedValue == 5);

    arrayListDestroy(list);
}

static void testGetBadIndexesReturnnullptr(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    int a = 10;
    int b = 20;
    arrayListAdd(list, &a);
    arrayListAdd(list, &b);

    assert(arrayListGet(list, -1) == nullptr);
    assert(arrayListGet(list, 2) == nullptr);
    assert(arrayListGet(list, 999) == nullptr);

    arrayListDestroy(list);
}

static void testRemoveFirstElement(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    int a = 11;
    int b = 22;
    int c = 33;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);

    unsigned int capacityBeforeRemove = list->capacity;

    arrayListRemoveAt(list, 0);

    assert(list->length == 2);
    assert(list->capacity <= capacityBeforeRemove || list->capacity == capacityBeforeRemove);

    int* first = (int*)arrayListGet(list, 0);
    int* second = (int*)arrayListGet(list, 1);

    assert(first != nullptr);
    assert(second != nullptr);
    assert(*first == 22);
    assert(*second == 33);

    arrayListDestroy(list);
}

static void testRemoveMiddleElement(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    int a = 11;
    int b = 22;
    int c = 33;
    int d = 44;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);
    arrayListAdd(list, &d);

    unsigned int capacityBeforeRemove = list->capacity;

    arrayListRemoveAt(list, 2);

    assert(list->length == 3);
    assert(list->capacity <= capacityBeforeRemove || list->capacity == capacityBeforeRemove);

    int* first = (int*)arrayListGet(list, 0);
    int* second = (int*)arrayListGet(list, 1);
    int* third = (int*)arrayListGet(list, 2);

    assert(first != nullptr);
    assert(second != nullptr);
    assert(third != nullptr);

    assert(*first == 11);
    assert(*second == 22);
    assert(*third == 44);

    arrayListDestroy(list);
}

static void testRemoveLastElement(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    int a = 11;
    int b = 22;
    int c = 33;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);

    unsigned int capacityBeforeRemove = list->capacity;

    arrayListRemoveAt(list, 2);

    assert(list->length == 2);
    assert(list->capacity <= capacityBeforeRemove || list->capacity == capacityBeforeRemove);

    int* first = (int*)arrayListGet(list, 0);
    int* second = (int*)arrayListGet(list, 1);

    assert(first != nullptr);
    assert(second != nullptr);
    assert(*first == 11);
    assert(*second == 22);

    arrayListDestroy(list);
}

static void testRemoveOnlyElement(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    int value = 77;
    arrayListAdd(list, &value);

    arrayListRemoveAt(list, 0);

    assert(list->length == 0);
    assert(arrayListGet(list, 0) == nullptr);

    if (list->elements == nullptr) {
        assert(1);
    }

    arrayListDestroy(list);
}

static void testRemoveBadIndexesDoNothing(void) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    int a = 1;
    int b = 2;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);

    int originalLength = list->length;
    unsigned int originalCapacity = list->capacity;

    arrayListRemoveAt(list, -1);
    assert(list->length == originalLength);
    assert(list->capacity == originalCapacity);
    assert(*(int*)arrayListGet(list, 0) == 1);
    assert(*(int*)arrayListGet(list, 1) == 2);

    arrayListRemoveAt(list, 2);
    assert(list->length == originalLength);
    assert(list->capacity == originalCapacity);
    assert(*(int*)arrayListGet(list, 0) == 1);
    assert(*(int*)arrayListGet(list, 1) == 2);

    arrayListRemoveAt(list, 999);
    assert(list->length == originalLength);
    assert(list->capacity == originalCapacity);
    assert(*(int*)arrayListGet(list, 0) == 1);
    assert(*(int*)arrayListGet(list, 1) == 2);

    arrayListDestroy(list);
}

static void testAddStructs(void) {
    ArrayList* list = ArrayListInit(sizeof(TestStruct), 1);
    assert(list != nullptr);

    TestStruct a = {1, 1.5};
    TestStruct b = {2, 3.25};
    TestStruct c = {3, 9.75};

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);

    assert(list->length == 3);
    assert(list->capacity >= 3);

    TestStruct* first = (TestStruct*)arrayListGet(list, 0);
    TestStruct* second = (TestStruct*)arrayListGet(list, 1);
    TestStruct* third = (TestStruct*)arrayListGet(list, 2);

    assert(first != nullptr);
    assert(second != nullptr);
    assert(third != nullptr);

    assert(first->id == 1);
    assert(first->value == 1.5);

    assert(second->id == 2);
    assert(second->value == 3.25);

    assert(third->id == 3);
    assert(third->value == 9.75);

    arrayListDestroy(list);
}

static void testStructCopyIsIndependent(void) {
    ArrayList* list = ArrayListInit(sizeof(TestStruct), 1);
    assert(list != nullptr);

    TestStruct item = {10, 2.0};
    arrayListAdd(list, &item);

    item.id = 999;
    item.value = 123.456;

    TestStruct* stored = (TestStruct*)arrayListGet(list, 0);
    assert(stored != nullptr);
    assert(stored->id == 10);
    assert(stored->value == 2.0);

    arrayListDestroy(list);
}

static void testPerformanceAdd(size_t count) {
    ArrayList* list = ArrayListInit(sizeof(int), 1);
    assert(list != nullptr);

    double start = nowSeconds();

    for (size_t i = 0; i < count; i++) {
        int value = (int)i;
        arrayListAdd(list, &value);
    }

    double end = nowSeconds();

    assert(list->length == (int)count);
    assert(list->capacity >= count);

    printTiming("Performance - add", end - start);

    arrayListDestroy(list);
}

static void testPerformanceGet(size_t count) {
    ArrayList* list = ArrayListInit(sizeof(int), (unsigned int)count);
    assert(list != nullptr);

    for (size_t i = 0; i < count; i++) {
        int value = (int)i;
        arrayListAdd(list, &value);
    }

    volatile long long sum = 0;
    double start = nowSeconds();

    for (size_t i = 0; i < count; i++) {
        int* value = (int*)arrayListGet(list, (int)i);
        assert(value != nullptr);
        sum += *value;
    }

    double end = nowSeconds();

    printTiming("Performance - get", end - start);

    if (sum == -1) {
        printf("Impossible sum: %lld\n", sum);
    }

    arrayListDestroy(list);
}

static void testPerformanceRemoveFront(size_t count) {
    ArrayList* list = ArrayListInit(sizeof(int), (unsigned int)count);
    assert(list != nullptr);

    for (size_t i = 0; i < count; i++) {
        int value = (int)i;
        arrayListAdd(list, &value);
    }

    double start = nowSeconds();

    while (list->length > 0) {
        arrayListRemoveAt(list, 0);
    }

    double end = nowSeconds();

    assert(list->length == 0);
    printTiming("Performance - remove front", end - start);

    arrayListDestroy(list);
}

static void testPerformanceRemoveBack(size_t count) {
    ArrayList* list = ArrayListInit(sizeof(int), (unsigned int)count);
    assert(list != nullptr);

    for (size_t i = 0; i < count; i++) {
        int value = (int)i;
        arrayListAdd(list, &value);
    }

    double start = nowSeconds();

    while (list->length > 0) {
        arrayListRemoveAt(list, list->length - 1);
    }

    double end = nowSeconds();

    assert(list->length == 0);
    printTiming("Performance - remove back", end - start);

    arrayListDestroy(list);
}

int main(void) {
    testInit();
    testInitWithLargerCapacity();
    testGetOnEmptyListReturnsnullptr();
    testAddSingleInt();
    testAddMultipleInts();
    testCapacityGrowsWhenNeeded();
    testAddCopiesValue();
    testGetBadIndexesReturnnullptr();
    testRemoveFirstElement();
    testRemoveMiddleElement();
    testRemoveLastElement();
    testRemoveOnlyElement();
    testRemoveBadIndexesDoNothing();
    testAddStructs();
    testStructCopyIsIndependent();

    printf("All ArrayList correctness tests passed.\n\n");
    
    testPerformanceAdd(100000);
    testPerformanceGet(100000);
    testPerformanceRemoveBack(50000);
    testPerformanceRemoveFront(50000);

    return 0;
}