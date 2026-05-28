#ifndef ImmutableArraySequence_H
#define ImmutableArraySequence_H

#include "ArraySequence.h"

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(T *items_arr, long count) : ArraySequence<T>(items_arr, count) {}
    explicit ImmutableArraySequence(long size) : ArraySequence<T>(size) {}
    ImmutableArraySequence(const ImmutableArraySequence<T> &other) : ArraySequence<T>(other) {}

    ImmutableArraySequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }

protected:
    ImmutableArraySequence<T>* Instance() override {
        return this -> Clone();
    }
};
#endif //ImmutableArraySequence_H