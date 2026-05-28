#ifndef Sequence_H
#define Sequence_H

#include "Option.h"
#include "IEnumerator.h"

template <typename T>
class Sequence : public IEnumerable<T> {
public:
    virtual ~Sequence() {}

    virtual T Get_first() const = 0;
    virtual T Get_last() const = 0;
    virtual T Get(long index) const = 0;
    virtual Sequence<T>* Get_subsequence(long start_index, long end_index) const = 0;
    virtual long Get_length() const = 0;
    
    virtual Sequence<T>* Append(T item) = 0;
    virtual Sequence<T>* Prepend(T item) = 0;
    virtual Sequence<T>* Insert_at(T item, long index) = 0;
    virtual Sequence<T>* Concat(Sequence<T> *other) = 0;
    
    virtual Sequence<T>* Map(T (*function)(T)) const = 0;
    virtual Sequence<T>* Where(bool (*predicate)(T)) const = 0;
    virtual T Reduce(T (*function)(T, T), T init) const = 0;
    
    virtual Option<T> Try_get_first(bool (*predicate)(T)) const = 0;
    virtual Option<T> Try_get_last(bool (*predicate)(T)) const = 0;
    
    virtual Sequence<T>* Slice(long start, long count, Sequence<T> *insert_seq = nullptr) = 0;
    virtual Sequence<T>* Zip(Sequence<T> *other) const = 0;
    virtual Sequence<T>* Flat_map(Sequence<T>* (*function)(T)) const = 0;
    
    virtual Sequence<T>* Clone() const = 0;

    T operator[](long index) const {
        return Get(index);
    }

    virtual bool operator==(const Sequence<T> &other) const {
        if (Get_length() != other.Get_length()) {
            return false;
        }
        for (long sequence_index = 0; sequence_index < Get_length(); sequence_index ++) {
            if (Get(sequence_index) != other.Get(sequence_index)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Sequence<T> &other) const {
        if (*this == other) {
            return false;
        }
        return true;
    }
};
#endif //Sequence_H