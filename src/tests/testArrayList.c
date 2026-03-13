#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "arraylist.h"

typedef struct TestStruct
{
    int id;
    double value;
} TestStruct;

static void testInit(void)
{
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != NULL);
    assert(list->length == 0);
    assert(list->elementSize == sizeof(int));
    assert(list->elements == NULL);

    arrayListDestroy(list);
}

static void testAddSingleInt(void)
{
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != NULL);

    int value = 42;
    arrayListAdd(list, &value);

    assert(list->length == 1);
    assert(list->elements != NULL);

    int* elements = (int*)list->elements;
    assert(elements[0] == 42);

    arrayListDestroy(list);
}

static void testAddMultipleInts(void)
{
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != NULL);

    for (int i = 0; i < 10; i++)
    {
        arrayListAdd(list, &i);
    }

    assert(list->length == 10);

    int* elements = (int*)list->elements;
    for (int i = 0; i < 10; i++)
    {
        assert(elements[i] == i);
    }

    arrayListDestroy(list);
}

static void testAddCopiesValue(void)
{
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != NULL);

    int value = 5;
    arrayListAdd(list, &value);

    value = 999;

    int* elements = (int*)list->elements;
    assert(list->length == 1);
    assert(elements[0] == 5);

    arrayListDestroy(list);
}

static void testAddStructs(void)
{
    ArrayList* list = ArrayListInit(sizeof(TestStruct));
    assert(list != NULL);

    TestStruct a = { .id = 1, .value = 1.5 };
    TestStruct b = { .id = 2, .value = 3.25 };
    TestStruct c = { .id = 3, .value = 9.75 };

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);

    assert(list->length == 3);

    TestStruct* elements = (TestStruct*)list->elements;

    assert(elements[0].id == 1);
    assert(elements[0].value == 1.5);

    assert(elements[1].id == 2);
    assert(elements[1].value == 3.25);

    assert(elements[2].id == 3);
    assert(elements[2].value == 9.75);

    arrayListDestroy(list);
}

static void testStructCopyIsIndependent(void)
{
    ArrayList* list = ArrayListInit(sizeof(TestStruct));
    assert(list != NULL);

    TestStruct item = { .id = 10, .value = 2.0 };
    arrayListAdd(list, &item);

    item.id = 999;
    item.value = 123.456;

    TestStruct* elements = (TestStruct*)list->elements;
    assert(list->length == 1);
    assert(elements[0].id == 10);
    assert(elements[0].value == 2.0);

    arrayListDestroy(list);
}

static void testContiguousStorage(void)
{
    ArrayList* list = ArrayListInit(sizeof(int));
    assert(list != NULL);

    int a = 11;
    int b = 22;
    int c = 33;

    arrayListAdd(list, &a);
    arrayListAdd(list, &b);
    arrayListAdd(list, &c);

    assert(list->length == 3);

    char* base = (char*)list->elements;
    int first = *(int*)(base + (0 * sizeof(int)));
    int second = *(int*)(base + (1 * sizeof(int)));
    int third = *(int*)(base + (2 * sizeof(int)));

    assert(first == 11);
    assert(second == 22);
    assert(third == 33);

    arrayListDestroy(list);
}

int main(void)
{
    testInit();
    testAddSingleInt();
    testAddMultipleInts();
    testAddCopiesValue();
    testAddStructs();
    testStructCopyIsIndependent();
    testContiguousStorage();

    printf("All ArrayList tests passed.\n");
    return 0;
}