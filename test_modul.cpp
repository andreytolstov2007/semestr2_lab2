#include <iostream>
#include "MutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"
#include "LinkedList.h"
#include "DynamicArray.h"
#include "Errors.h"

static int tests_passed = 0;
static int tests_failed = 0;

void Register_test(void (*test_func)(), const std::string &test_name) {
    try {
        test_func();
        tests_passed ++;
        std::cout << "[PASS] " << test_name << std::endl;
    }
    catch (const Errors &err) {
        tests_failed ++;
        std::cout << "[FAIL] " << test_name << " " << err.Message() << std::endl;
    }
}

#define ASSERT_EQ(x, y) \
    if ((x) != (y)) { \
        throw Errors("ASSERT_EQ failed"); \
    } \

#define ASSERT_THROWS(expr, exc_type) \
    { \
        bool caught = false; \
        try { \
            expr; \
        } \
        catch (const exc_type&) { \
            caught = true; \
        } \
        catch (const Errors&) {} \
        if (caught == false) { \
            throw Errors("ASSERT_THROWS failed"); \
        } \
    } \

bool is_even(int x) {
    if (x % 2 == 0) {
        return true;
    }
    return false;
}

int square_int(int x) {
    return x * x;
}

int sum_int(int acc, int x) {
    return acc + x;
}

void Test_DynamicArray_default_constructor() {
    DynamicArray<int> arr;
    ASSERT_EQ(arr.Get_length(), 64);
}

void Test_DynamicArray_size_constructor() {
    DynamicArray<int> arr(5);
    ASSERT_EQ(arr.Get_length(), 5);
}

void Test_DynamicArray_array_constructor() {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr(items, 3);
    ASSERT_EQ(arr.Get_length(), 3);
    ASSERT_EQ(arr.Get_arr_index(0), 1);
    ASSERT_EQ(arr.Get_arr_index(2), 3);
}

void Test_DynamicArray_copy_constructor() {
    int items[] = {4, 5, 6};
    DynamicArray<int> arr1(items, 3);
    DynamicArray<int> arr2(arr1);
    ASSERT_EQ(arr2.Get_length(), 3);
    ASSERT_EQ(arr2.Get_arr_index(1), 5);
}

void Test_DynamicArray_get_set() {
    DynamicArray<int> arr(3);
    arr.Set(0, 10);
    arr.Set(1, 20);
    arr.Set(2, 30);
    ASSERT_EQ(arr.Get_arr_index(0), 10);
    ASSERT_EQ(arr.Get_arr_index(1), 20);
    ASSERT_EQ(arr.Get_arr_index(2), 30);
    ASSERT_THROWS(arr.Get_arr_index(3), IndexOutOfRange);
    ASSERT_THROWS(arr.Set(3, 40), IndexOutOfRange);
}

void Test_DynamicArray_negative_index() {
    DynamicArray<int> arr(3);
    ASSERT_THROWS(arr.Get_arr_index(-1), IndexOutOfRange);
    ASSERT_THROWS(arr.Set(-1, 10), IndexOutOfRange);
}

void Test_DynamicArray_resize() {
    DynamicArray<int> arr(2);
    arr.Set(0, 1);
    arr.Set(1, 2);
    arr.Resize(4);
    ASSERT_EQ(arr.Get_length(), 4);
    ASSERT_EQ(arr.Get_arr_index(0), 1);
    ASSERT_EQ(arr.Get_arr_index(1), 2);
    arr.Resize(1);
    ASSERT_EQ(arr.Get_length(), 1);
    ASSERT_EQ(arr.Get_arr_index(0), 1);
}

void Test_DynamicArray_resize_negative() {
    DynamicArray<int> arr(3);
    ASSERT_THROWS(arr.Resize(-1), IndexOutOfRange);
}

void Test_LinkedList_default_constructor() {
    LinkedList<int> list;
    ASSERT_EQ(list.Get_length(), 0);
}

void Test_LinkedList_array_constructor() {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);
    ASSERT_EQ(list.Get_length(), 3);
    ASSERT_EQ(list.Get_first(), 1);
    ASSERT_EQ(list.Get_last(), 3);
}

void Test_LinkedList_copy_constructor() {
    int items[] = {4, 5, 6};
    LinkedList<int> list1(items, 3);
    LinkedList<int> list2(list1);
    ASSERT_EQ(list2.Get_length(), 3);
    ASSERT_EQ(list2.Get_list_index(1), 5);
}

void Test_LinkedList_append() {
    LinkedList<int> list;
    list.Append(10);
    list.Append(20);
    ASSERT_EQ(list.Get_length(), 2);
    ASSERT_EQ(list.Get_last(), 20);
}

void Test_LinkedList_prepend() {
    LinkedList<int> list;
    list.Append(30);
    list.Prepend(10);
    list.Prepend(5);
    ASSERT_EQ(list.Get_length(), 3);
    ASSERT_EQ(list.Get_first(), 5);
}

void Test_LinkedList_insert_at() {
    LinkedList<int> list;
    list.Append(1);
    list.Append(3);
    list.Insert_at(2, 1);
    ASSERT_EQ(list.Get_length(), 3);
    ASSERT_EQ(list.Get_list_index(1), 2);
    ASSERT_THROWS(list.Insert_at(99, 10), IndexOutOfRange);
}

void Test_LinkedList_insert_at_negative() {
    LinkedList<int> list;
    list.Append(1);
    ASSERT_THROWS(list.Insert_at(99, -1), IndexOutOfRange);
}

void Test_LinkedList_get_list_index_negative() {
    LinkedList<int> list;
    list.Append(1);
    ASSERT_THROWS(list.Get_list_index(-1), IndexOutOfRange);
}

void Test_LinkedList_get_sub_list() {
    int items[] = {10, 20, 30, 40, 50};
    LinkedList<int> list(items, 5);
    LinkedList<int> sub = list.Get_sub_list(1, 3);
    ASSERT_EQ(sub.Get_length(), 3);
    ASSERT_EQ(sub.Get_first(), 20);
    ASSERT_EQ(sub.Get_last(), 40);
}

void Test_LinkedList_get_sub_list_negative() {
    int items[] = {10, 20, 30};
    LinkedList<int> list(items, 3);
    ASSERT_THROWS(list.Get_sub_list(-1, 1), IndexOutOfRange);
}

void Test_LinkedList_concat() {
    int items1[] = {1, 2};
    int items2[] = {3, 4};
    LinkedList<int> list1(items1, 2);
    LinkedList<int> list2(items2, 2);
    LinkedList<int> result = list1.Concat(list2);
    ASSERT_EQ(result.Get_length(), 4);
    ASSERT_EQ(result.Get_first(), 1);
    ASSERT_EQ(result.Get_last(), 4);
}

void Test_MutableArraySequence_default_constructor() {
    MutableArraySequence<int> seq;
    ASSERT_EQ(seq.Get_length(), 64);
}

void Test_MutableArraySequence_array_constructor() {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> seq(items, 3);
    ASSERT_EQ(seq.Get_length(), 3);
    ASSERT_EQ(seq.Get_first(), 1);
    ASSERT_EQ(seq.Get_last(), 3);
}

void Test_MutableArraySequence_append() {
    MutableArraySequence<int> seq(0);
    Sequence<int> *new_seq = seq.Append(10);
    ASSERT_EQ(new_seq, &seq);
    new_seq = new_seq -> Append(20);
    ASSERT_EQ(new_seq -> Get_length(), 2);
    ASSERT_EQ(new_seq -> Get_last(), 20);
}

void Test_MutableArraySequence_prepend() {
    MutableArraySequence<int> seq(0);
    Sequence<int> *new_seq = seq.Append(30);
    new_seq = new_seq -> Prepend(10);
    ASSERT_EQ(new_seq -> Get_first(), 10);
}

void Test_MutableArraySequence_insert_at() {
    MutableArraySequence<int> seq(0);
    Sequence<int> *new_seq = seq.Append(1);
    new_seq = new_seq -> Append(3);
    new_seq = new_seq -> Insert_at(2, 1);
    ASSERT_EQ(new_seq -> Get(1), 2);
}

void Test_MutableArraySequence_insert_at_negative() {
    MutableArraySequence<int> seq(0);
    Sequence<int> *new_seq = seq.Append(1);
    ASSERT_THROWS(new_seq -> Insert_at(99, -1), IndexOutOfRange);
}

void Test_MutableArraySequence_get_negative() {
    MutableArraySequence<int> seq(0);
    Sequence<int> *new_seq = seq.Append(1);
    ASSERT_THROWS(new_seq -> Get(-1), IndexOutOfRange);
}

void Test_MutableArraySequence_get_subsequence() {
    int items[] = {10, 20, 30, 40, 50};
    MutableArraySequence<int> seq(items, 5);
    Sequence<int> *sub = seq.Get_subsequence(1, 3);
    ASSERT_EQ(sub -> Get_length(), 3);
    ASSERT_EQ(sub -> Get_first(), 20);
    delete sub;
}

void Test_MutableArraySequence_get_subsequence_negative() {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> seq(items, 3);
    ASSERT_THROWS(seq.Get_subsequence(-1, 1), IndexOutOfRange);
}

void Test_MutableArraySequence_concat() {
    int items1[] = {1, 2};
    int items2[] = {3, 4};
    MutableArraySequence<int> seq1(items1, 2);
    MutableArraySequence<int> seq2(items2, 2);
    Sequence<int> *result = seq1.Concat(&seq2);
    ASSERT_EQ(result -> Get_length(), 4);
    ASSERT_EQ(result, &seq1);
}

void Test_MutableArraySequence_map() {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> seq(items, 3);
    Sequence<int> *mapped = seq.Map(square_int);
    ASSERT_EQ(mapped -> Get(0), 1);
    ASSERT_EQ(mapped -> Get(1), 4);
    ASSERT_EQ(mapped -> Get(2), 9);
    delete mapped;
}

void Test_MutableArraySequence_where() {
    int items[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(items, 4);
    Sequence<int> *filtered = seq.Where(is_even);
    ASSERT_EQ(filtered -> Get_length(), 2);
    delete filtered;
}

void Test_MutableArraySequence_reduce() {
    int items[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(items, 4);
    int sum = seq.Reduce(sum_int, 0);
    ASSERT_EQ(sum, 10);
}

void Test_MutableArraySequence_equality() {
    int items1[] = {1, 2, 3};
    int items2[] = {1, 2, 3};
    int items3[] = {1, 2, 4};
    MutableArraySequence<int> seq1(items1, 3);
    MutableArraySequence<int> seq2(items2, 3);
    MutableArraySequence<int> seq3(items3, 3);
    ASSERT_EQ(seq1 == seq2, true);
    ASSERT_EQ(seq1 == seq3, false);
}

void Test_MutableArraySequence_subscript() {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> seq(items, 3);
    ASSERT_EQ(seq[0], 10);
    ASSERT_EQ(seq[2], 30);
}

void Test_MutableArraySequence_subscript_out_of_range() {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> seq(items, 3);
    ASSERT_THROWS(seq[3], IndexOutOfRange);
}

void Test_MutableArraySequence_append_same_pointer() {
    MutableArraySequence<int> seq(0);
    Sequence<int> *result = seq.Append(5);
    ASSERT_EQ(result, &seq);
}

void Test_MutableArraySequence_prepend_same_pointer() {
    MutableArraySequence<int> seq(0);
    Sequence<int> *result = seq.Prepend(5);
    ASSERT_EQ(result, &seq);
}

void Test_MutableArraySequence_concat_same_pointer() {
    int items1[] = {1, 2};
    int items2[] = {3, 4};
    MutableArraySequence<int> seq1(items1, 2);
    MutableArraySequence<int> seq2(items2, 2);
    Sequence<int> *result = seq1.Concat(&seq2);
    ASSERT_EQ(result, &seq1);
}

void Test_MutableListSequence_default_constructor() {
    MutableListSequence<int> seq;
    ASSERT_EQ(seq.Get_length(), 0);
}

void Test_MutableListSequence_array_constructor() {
    int items[] = {1, 2, 3};
    MutableListSequence<int> seq(items, 3);
    ASSERT_EQ(seq.Get_length(), 3);
    ASSERT_EQ(seq.Get_first(), 1);
    ASSERT_EQ(seq.Get_last(), 3);
}

void Test_MutableListSequence_append() {
    MutableListSequence<int> seq;
    Sequence<int> *new_seq = seq.Append(10);
    ASSERT_EQ(new_seq, &seq);
    new_seq = new_seq -> Append(20);
    ASSERT_EQ(new_seq -> Get_length(), 2);
    ASSERT_EQ(new_seq -> Get_last(), 20);
}

void Test_MutableListSequence_prepend() {
    MutableListSequence<int> seq;
    Sequence<int> *new_seq = seq.Append(30);
    new_seq = new_seq -> Prepend(10);
    ASSERT_EQ(new_seq -> Get_first(), 10);
}

void Test_MutableListSequence_insert_at() {
    MutableListSequence<int> seq;
    Sequence<int> *new_seq = seq.Append(1);
    new_seq = new_seq -> Append(3);
    new_seq = new_seq -> Insert_at(2, 1);
    ASSERT_EQ(new_seq -> Get(1), 2);
}

void Test_MutableListSequence_insert_at_negative() {
    MutableListSequence<int> seq;
    Sequence<int> *new_seq = seq.Append(1);
    ASSERT_THROWS(new_seq -> Insert_at(99, -1), IndexOutOfRange);
}

void Test_MutableListSequence_get_negative() {
    MutableListSequence<int> seq;
    Sequence<int> *new_seq = seq.Append(1);
    ASSERT_THROWS(new_seq -> Get(-1), IndexOutOfRange);
}

void Test_MutableListSequence_get_subsequence() {
    int items[] = {10, 20, 30, 40, 50};
    MutableListSequence<int> seq(items, 5);
    Sequence<int> *sub = seq.Get_subsequence(1, 3);
    ASSERT_EQ(sub -> Get_length(), 3);
    ASSERT_EQ(sub -> Get_first(), 20);
    delete sub;
}

void Test_MutableListSequence_get_subsequence_negative() {
    int items[] = {10, 20, 30};
    MutableListSequence<int> seq(items, 3);
    ASSERT_THROWS(seq.Get_subsequence(-1, 1), IndexOutOfRange);
}

void Test_MutableListSequence_concat() {
    int items1[] = {1, 2};
    int items2[] = {3, 4};
    MutableListSequence<int> seq1(items1, 2);
    MutableListSequence<int> seq2(items2, 2);
    Sequence<int> *result = seq1.Concat(&seq2);
    ASSERT_EQ(result -> Get_length(), 4);
    ASSERT_EQ(result, &seq1);
}

void Test_MutableListSequence_map() {
    int items[] = {1, 2, 3};
    MutableListSequence<int> seq(items, 3);
    Sequence<int> *mapped = seq.Map(square_int);
    ASSERT_EQ(mapped -> Get(0), 1);
    ASSERT_EQ(mapped -> Get(1), 4);
    ASSERT_EQ(mapped -> Get(2), 9);
    delete mapped;
}

void Test_MutableListSequence_where() {
    int items[] = {1, 2, 3, 4};
    MutableListSequence<int> seq(items, 4);
    Sequence<int> *filtered = seq.Where(is_even);
    ASSERT_EQ(filtered -> Get_length(), 2);
    delete filtered;
}

void Test_MutableListSequence_reduce() {
    int items[] = {1, 2, 3, 4};
    MutableListSequence<int> seq(items, 4);
    int sum = seq.Reduce(sum_int, 0);
    ASSERT_EQ(sum, 10);
}

void Test_MutableListSequence_equality() {
    int items1[] = {1, 2, 3};
    int items2[] = {1, 2, 3};
    int items3[] = {1, 2, 4};
    MutableListSequence<int> seq1(items1, 3);
    MutableListSequence<int> seq2(items2, 3);
    MutableListSequence<int> seq3(items3, 3);
    ASSERT_EQ(seq1 == seq2, true);
    ASSERT_EQ(seq1 == seq3, false);
}

void Test_MutableListSequence_subscript() {
    int items[] = {10, 20, 30};
    MutableListSequence<int> seq(items, 3);
    ASSERT_EQ(seq[0], 10);
    ASSERT_EQ(seq[2], 30);
}

void Test_MutableListSequence_subscript_out_of_range() {
    int items[] = {10, 20, 30};
    MutableListSequence<int> seq(items, 3);
    ASSERT_THROWS(seq[3], IndexOutOfRange);
}

void Test_MutableListSequence_append_same_pointer() {
    MutableListSequence<int> seq;
    Sequence<int> *result = seq.Append(5);
    ASSERT_EQ(result, &seq);
}

void Test_MutableListSequence_prepend_same_pointer() {
    MutableListSequence<int> seq;
    Sequence<int> *result = seq.Prepend(5);
    ASSERT_EQ(result, &seq);
}

void Test_MutableListSequence_concat_same_pointer() {
    int items1[] = {1, 2};
    int items2[] = {3, 4};
    MutableListSequence<int> seq1(items1, 2);
    MutableListSequence<int> seq2(items2, 2);
    Sequence<int> *result = seq1.Concat(&seq2);
    ASSERT_EQ(result, &seq1);
}

void Test_ImmutableArraySequence_append_new_pointer() {
    int items[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(items, 3);
    Sequence<int> *result = seq.Append(4);
    ASSERT_EQ(result != &seq, true);
    ASSERT_EQ(result -> Get_length(), 4);
    ASSERT_EQ(result -> Get_last(), 4);
    ASSERT_EQ(seq.Get_length(), 3);
    delete result;
}

void Test_ImmutableListSequence_append_new_pointer() {
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> seq(items, 3);
    Sequence<int> *result = seq.Append(4);
    ASSERT_EQ(result != &seq, true);
    ASSERT_EQ(result -> Get_length(), 4);
    ASSERT_EQ(result -> Get_last(), 4);
    ASSERT_EQ(seq.Get_length(), 3);
    delete result;
}

void Test_ImmutableArraySequence_prepend_new_pointer() {
    int items[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(items, 3);
    Sequence<int> *result = seq.Prepend(0);
    ASSERT_EQ(result != &seq, true);
    ASSERT_EQ(result -> Get_length(), 4);
    ASSERT_EQ(result -> Get_first(), 0);
    ASSERT_EQ(seq.Get_length(), 3);
    delete result;
}

void Test_ImmutableListSequence_prepend_new_pointer() {
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> seq(items, 3);
    Sequence<int> *result = seq.Prepend(0);
    ASSERT_EQ(result != &seq, true);
    ASSERT_EQ(result -> Get_length(), 4);
    ASSERT_EQ(result -> Get_first(), 0);
    ASSERT_EQ(seq.Get_length(), 3);
    delete result;
}

void Test_ImmutableArraySequence_concat_new_pointer() {
    int items1[] = {1, 2};
    int items2[] = {3, 4};
    ImmutableArraySequence<int> seq1(items1, 2);
    ImmutableArraySequence<int> seq2(items2, 2);
    Sequence<int> *result = seq1.Concat(&seq2);
    ASSERT_EQ(result != &seq1, true);
    ASSERT_EQ(result -> Get_length(), 4);
    ASSERT_EQ(seq1.Get_length(), 2);
    delete result;
}

void Test_ImmutableListSequence_concat_new_pointer() {
    int items1[] = {1, 2};
    int items2[] = {3, 4};
    ImmutableListSequence<int> seq1(items1, 2);
    ImmutableListSequence<int> seq2(items2, 2);
    Sequence<int> *result = seq1.Concat(&seq2);
    ASSERT_EQ(result != &seq1, true);
    ASSERT_EQ(result -> Get_length(), 4);
    ASSERT_EQ(seq1.Get_length(), 2);
    delete result;
}

void Test_ImmutableArraySequence_map() {
    int items[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(items, 3);
    Sequence<int> *mapped = seq.Map(square_int);
    ASSERT_EQ(mapped -> Get(0), 1);
    ASSERT_EQ(mapped -> Get(1), 4);
    ASSERT_EQ(mapped -> Get(2), 9);
    delete mapped;
}

void Test_ImmutableListSequence_map() {
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> seq(items, 3);
    Sequence<int> *mapped = seq.Map(square_int);
    ASSERT_EQ(mapped -> Get(0), 1);
    ASSERT_EQ(mapped -> Get(1), 4);
        ASSERT_EQ(mapped -> Get(2), 9);
    delete mapped;
}

void Run_tests() {
    Register_test(Test_DynamicArray_default_constructor, "DynamicArray_default_constructor");
    Register_test(Test_DynamicArray_size_constructor, "DynamicArray_size_constructor");
    Register_test(Test_DynamicArray_array_constructor, "DynamicArray_array_constructor");
    Register_test(Test_DynamicArray_copy_constructor, "DynamicArray_copy_constructor");
    Register_test(Test_DynamicArray_get_set, "DynamicArray_get_set");
    Register_test(Test_DynamicArray_negative_index, "DynamicArray_negative_index");
    Register_test(Test_DynamicArray_resize, "DynamicArray_resize");
    Register_test(Test_DynamicArray_resize_negative, "DynamicArray_resize_negative");

    Register_test(Test_LinkedList_default_constructor, "LinkedList_default_constructor");
    Register_test(Test_LinkedList_array_constructor, "LinkedList_array_constructor");
    Register_test(Test_LinkedList_copy_constructor, "LinkedList_copy_constructor");
    Register_test(Test_LinkedList_append, "LinkedList_append");
    Register_test(Test_LinkedList_prepend, "LinkedList_prepend");
    Register_test(Test_LinkedList_insert_at, "LinkedList_insert_at");
    Register_test(Test_LinkedList_insert_at_negative, "LinkedList_insert_at_negative");
    Register_test(Test_LinkedList_get_list_index_negative, "LinkedList_get_list_index_negative");
    Register_test(Test_LinkedList_get_sub_list, "LinkedList_get_sub_list");
    Register_test(Test_LinkedList_get_sub_list_negative, "LinkedList_get_sub_list_negative");
    Register_test(Test_LinkedList_concat, "LinkedList_concat");

    Register_test(Test_MutableArraySequence_default_constructor, "MutableArraySequence_default_constructor");
    Register_test(Test_MutableArraySequence_array_constructor, "MutableArraySequence_array_constructor");
    Register_test(Test_MutableArraySequence_append, "MutableArraySequence_append");
    Register_test(Test_MutableArraySequence_prepend, "MutableArraySequence_prepend");
    Register_test(Test_MutableArraySequence_insert_at, "MutableArraySequence_insert_at");
    Register_test(Test_MutableArraySequence_insert_at_negative, "MutableArraySequence_insert_at_negative");
    Register_test(Test_MutableArraySequence_get_negative, "MutableArraySequence_get_negative");
    Register_test(Test_MutableArraySequence_get_subsequence, "MutableArraySequence_get_subsequence");
    Register_test(Test_MutableArraySequence_get_subsequence_negative, "MutableArraySequence_get_subsequence_negative");
    Register_test(Test_MutableArraySequence_concat, "MutableArraySequence_concat");
    Register_test(Test_MutableArraySequence_map, "MutableArraySequence_map");
    Register_test(Test_MutableArraySequence_where, "MutableArraySequence_where");
    Register_test(Test_MutableArraySequence_reduce, "MutableArraySequence_reduce");
    Register_test(Test_MutableArraySequence_equality, "MutableArraySequence_equality");
    Register_test(Test_MutableArraySequence_subscript, "MutableArraySequence_subscript");
    Register_test(Test_MutableArraySequence_subscript_out_of_range, "MutableArraySequence_subscript_out_of_range");
    Register_test(Test_MutableArraySequence_append_same_pointer, "MutableArraySequence_append_same_pointer");
    Register_test(Test_MutableArraySequence_prepend_same_pointer, "MutableArraySequence_prepend_same_pointer");
    Register_test(Test_MutableArraySequence_concat_same_pointer, "MutableArraySequence_concat_same_pointer");

    Register_test(Test_MutableListSequence_default_constructor, "MutableListSequence_default_constructor");
    Register_test(Test_MutableListSequence_array_constructor, "MutableListSequence_array_constructor");
    Register_test(Test_MutableListSequence_append, "MutableListSequence_append");
    Register_test(Test_MutableListSequence_prepend, "MutableListSequence_prepend");
    Register_test(Test_MutableListSequence_insert_at, "MutableListSequence_insert_at");
    Register_test(Test_MutableListSequence_insert_at_negative, "MutableListSequence_insert_at_negative");
    Register_test(Test_MutableListSequence_get_negative, "MutableListSequence_get_negative");
    Register_test(Test_MutableListSequence_get_subsequence, "MutableListSequence_get_subsequence");
    Register_test(Test_MutableListSequence_get_subsequence_negative, "MutableListSequence_get_subsequence_negative");
    Register_test(Test_MutableListSequence_concat, "MutableListSequence_concat");
    Register_test(Test_MutableListSequence_map, "MutableListSequence_map");
    Register_test(Test_MutableListSequence_where, "MutableListSequence_where");
    Register_test(Test_MutableListSequence_reduce, "MutableListSequence_reduce");
    Register_test(Test_MutableListSequence_equality, "MutableListSequence_equality");
    Register_test(Test_MutableListSequence_subscript, "MutableListSequence_subscript");
    Register_test(Test_MutableListSequence_subscript_out_of_range, "MutableListSequence_subscript_out_of_range");
    Register_test(Test_MutableListSequence_append_same_pointer, "MutableListSequence_append_same_pointer");
    Register_test(Test_MutableListSequence_prepend_same_pointer, "MutableListSequence_prepend_same_pointer");
    Register_test(Test_MutableListSequence_concat_same_pointer, "MutableListSequence_concat_same_pointer");

    Register_test(Test_ImmutableArraySequence_append_new_pointer, "ImmutableArraySequence_append_new_pointer");
    Register_test(Test_ImmutableListSequence_append_new_pointer, "ImmutableListSequence_append_new_pointer");
    Register_test(Test_ImmutableArraySequence_prepend_new_pointer, "ImmutableArraySequence_prepend_new_pointer");
    Register_test(Test_ImmutableListSequence_prepend_new_pointer, "ImmutableListSequence_prepend_new_pointer");
    Register_test(Test_ImmutableArraySequence_concat_new_pointer, "ImmutableArraySequence_concat_new_pointer");
    Register_test(Test_ImmutableListSequence_concat_new_pointer, "ImmutableListSequence_concat_new_pointer");
    Register_test(Test_ImmutableArraySequence_map, "ImmutableArraySequence_map");
    Register_test(Test_ImmutableListSequence_map, "ImmutableListSequence_map");

    std::cout << "\n=== Test Results ===" << std::endl;
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;

    if (tests_failed == 0) {
        std::cout << "All tests passed!" << std::endl;
    }
} //Run_tests()