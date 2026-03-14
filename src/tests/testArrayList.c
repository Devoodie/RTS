#include "arraylist.h"

#include <assert.h>
#include <stdio.h>

typedef struct TestStruct {
    int id;
    double value;
} TestStruct;

static void testInit(void) {
    ArrayList* list = ArrayListInit(sizeof(int));

    assert(list != nullptr);
    assert(list->length == 0);
    assert(list->elementSize == sizeof(int));
    assert(list->elements == nullptr);

    arrayListDestroy(list);
}

static void testGetOnEmptyListReturnsNull(void) {
    ArrayList* list = ArrayListInit(sizeof(int));

    assert(list != nullptr);
    assert(arrayListGet(list, 0) == nullptr);
    assert(arrayListGet(list, -1) == nullptr);

    arrayListDestroy(list);
}

static void testAddSingleInt(void) {
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != nullptr);

    int value = 42;
    arrayListAdd(list, &value);

    assert(list->length == 1);

    int* storedValue = (int*)arrayListGet(list, 0);
    assert(storedValue != nullptr);
    assert(*storedValue == 42);

    arrayListDestroy(list);
}

static void testAddMultipleInts(void) {
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != nullptr);

    for (int i = 0; i < 10; i++) {
        arrayListAdd(list, &i);
    }

    assert(list->length == 10);

    for (int i = 0; i < 10; i++) {
        int* storedValue = (int*)arrayListGet(list, i);
        assert(storedValue != nullptr);
        assert(*storedValue == i);
    }

    arrayListDestroy(list);
}

static void testAddCopiesValue(void) {
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != nullptr);

    int value = 5;
    arrayListAdd(list, &value);

    value = 999;

    int* storedValue = (int*)arrayListGet(list, 0);
    assert(storedValue != nullptr);
    assert(*storedValue == 5);

    arrayListDestroy(list);
}

static void testGetBadIndexesReturnNull(void) {
    ArrayList* list = ArrayListInit(sizeof(int));
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
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != nullptr);

    int a = 11;
    int b = 22;
    int c = 33;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);

    arrayListRemoveAt(list, 0);

    assert(list->length == 2);

    int* first = (int*)arrayListGet(list, 0);
    int* second = (int*)arrayListGet(list, 1);

    assert(first != nullptr);
    assert(second != nullptr);
    assert(*first == 22);
    assert(*second == 33);

    arrayListDestroy(list);
}

static void testRemoveMiddleElement(void) {
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != nullptr);

    int a = 11;
    int b = 22;
    int c = 33;
    int d = 44;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);
    arrayListAdd(list, &d);

    arrayListRemoveAt(list, 2);

    assert(list->length == 3);

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
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != nullptr);

    int a = 11;
    int b = 22;
    int c = 33;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);

    arrayListRemoveAt(list, 2);

    assert(list->length == 2);

    int* first = (int*)arrayListGet(list, 0);
    int* second = (int*)arrayListGet(list, 1);

    assert(first != nullptr);
    assert(second != nullptr);
    assert(*first == 11);
    assert(*second == 22);

    arrayListDestroy(list);
}

static void testRemoveOnlyElement(void) {
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != nullptr);

    int value = 77;
    arrayListAdd(list, &value);

    arrayListRemoveAt(list, 0);

    assert(list->length == 0);
    assert(list->elements == nullptr);
    assert(arrayListGet(list, 0) == nullptr);

    arrayListDestroy(list);
}

static void testRemoveBadIndexesDoNothing(void) {
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != nullptr);

    int a = 1;
    int b = 2;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);

    arrayListRemoveAt(list, -1);
    assert(list->length == 2);
    assert(*(int*)arrayListGet(list, 0) == 1);
    assert(*(int*)arrayListGet(list, 1) == 2);

    arrayListRemoveAt(list, 2);
    assert(list->length == 2);
    assert(*(int*)arrayListGet(list, 0) == 1);
    assert(*(int*)arrayListGet(list, 1) == 2);

    arrayListRemoveAt(list, 999);
    assert(list->length == 2);
    assert(*(int*)arrayListGet(list, 0) == 1);
    assert(*(int*)arrayListGet(list, 1) == 2);

    arrayListDestroy(list);
}

static void testAddStructs(void) {
    ArrayList* list = ArrayListInit(sizeof(TestStruct));
    assert(list != nullptr);

    TestStruct a = {1, 1.5};
    TestStruct b = {2, 3.25};
    TestStruct c = {3, 9.75};

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);

    assert(list->length == 3);

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
    ArrayList* list = ArrayListInit(sizeof(TestStruct));
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

int main(void) {
    testInit();
    testGetOnEmptyListReturnsNull();
    testAddSingleInt();
    testAddMultipleInts();
    testAddCopiesValue();
    testGetBadIndexesReturnNull();
    testRemoveFirstElement();
    testRemoveMiddleElement();
    testRemoveLastElement();
    testRemoveOnlyElement();
    testRemoveBadIndexesDoNothing();
    testAddStructs();
    testStructCopyIsIndependent();

    printf("All ArrayList tests passed.\n");
    return 0;
}