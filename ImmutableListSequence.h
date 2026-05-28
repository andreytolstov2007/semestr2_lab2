#ifndef ImmutableListSequence_H
#define ImmutableListSequence_H

#include "ListSequence.h"

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(T *items_arr, long count) : ListSequence<T>(items_arr, count) {}
    ImmutableListSequence(const ImmutableListSequence<T> &other) : ListSequence<T>(other) {}

    ImmutableListSequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }

protected:
    ImmutableListSequence<T>* Instance() override {
        return this -> Clone();
    }
};
#endif //ImmutableListSequence_H
