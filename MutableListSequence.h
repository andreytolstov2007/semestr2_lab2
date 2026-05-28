#ifndef MutableListSequence_H
#define MutableListSequence_H

#include "ListSequence.h"

template <typename T>
class MutableListSequence : public ListSequence<T> {
public:
    MutableListSequence() : ListSequence<T>() {}
    MutableListSequence(T *items_arr, long count) : ListSequence<T>(items_arr, count) {}
    MutableListSequence(const MutableListSequence<T> &other) : ListSequence<T>(other) {}

    MutableListSequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }
};
#endif //MutableListSequence_H