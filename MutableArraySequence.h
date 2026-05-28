#ifndef MutableArraySequence_H
#define MutableArraySequence_H

#include "ArraySequence.h"

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(T *items_arr, long count) : ArraySequence<T>(items_arr, count) {}
    explicit MutableArraySequence(long size) : ArraySequence<T>(size) {}
    MutableArraySequence(const MutableArraySequence<T> &other) : ArraySequence<T>(other) {}

    MutableArraySequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
};
#endif //MutableArraySequence_H