#ifndef DynamicArray_H
#define DynamicArray_H

#include "Errors.h"

template <typename T>
class DynamicArray {
public:
    DynamicArray() {
        array_length = 64;
        arr = new T[array_length]();
    }
    
    explicit DynamicArray(long length) {
        if (length < 0) {
            throw IndexOutOfRange();
        }
        arr = new T[length]();
        array_length = length;
    }

    DynamicArray(T *items, long length) {
        if (items == nullptr) {
            throw NullPointerError();
        }
        if (length < 0) {
            throw IndexOutOfRange();
        }

        arr = new T[length]();
        array_length = length;
        for (long arr_index = 0; arr_index < length; arr_index ++) {
            arr[arr_index] = items[arr_index];
        }
    }

    DynamicArray(const DynamicArray<T> &other) {
        this -> arr = new T[other.Get_length()]();
        this -> array_length = other.Get_length();
        for (long arr_index = 0; arr_index < other.Get_length(); arr_index ++) {
            this -> arr[arr_index] = other.Get_arr_index(arr_index);
        }
    }

    ~DynamicArray() {
        delete []arr;
    }

    T Get_arr_index(long arr_index) const {
        if ((arr_index < 0) || (arr_index >= array_length)) {
            throw IndexOutOfRange();
        }
        return arr[arr_index];
    }

    long Get_length() const {
        return array_length;
    }

    void Set(long arr_index, T value) {
        if ((arr_index < 0) || (arr_index >= array_length)) {
            throw IndexOutOfRange();
        }
        arr[arr_index] = value;
    }
    
    void Resize(long new_length) {
        if (new_length < 0) {
            throw IndexOutOfRange();
        }

        T *new_arr = new T[new_length]();
        if (new_length <= array_length) {
            for (long new_arr_index = 0; new_arr_index < new_length; new_arr_index ++) {
                new_arr[new_arr_index] = arr[new_arr_index];
            }
        }
        else {
            for (long new_arr_index = 0; new_arr_index < array_length; new_arr_index ++) {
                new_arr[new_arr_index] = arr[new_arr_index];
            }            
        }
        delete []arr;
        array_length = new_length;
        arr = new_arr;
    }
    DynamicArray<T>& operator=(const DynamicArray<T> &other) {
        if (this != &other) {
            delete []arr;
            array_length = other.Get_length();
            
            arr = new T[array_length]();
            for (long arr_index = 0; arr_index < array_length; arr_index ++) {
                arr[arr_index] = other.Get_arr_index(arr_index);
            }
        }
        return *this;
    }

private:
    T *arr;
    long array_length;
};
#endif //DynamicArray_H