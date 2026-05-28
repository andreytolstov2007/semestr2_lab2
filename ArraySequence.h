#ifndef ArraySequence_H
#define ArraySequence_H

#include "Sequence.h"
#include "DynamicArray.h"

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> *items;
    
    ArraySequence(DynamicArray<T> *arr, bool mutable_flag) {
        items = arr;
    }

    virtual ArraySequence<T>* Instance() {
        return this;
    }
    
public:
    ArraySequence() {
        items = new DynamicArray<T>();
    }
    
    ArraySequence(T *items_arr, long count) {
        items = new DynamicArray<T>(items_arr, count);
    }
    
    explicit ArraySequence(long size) {
        items = new DynamicArray<T>(size);
    }
    
    ArraySequence(const ArraySequence<T> &other) {
        items = new DynamicArray<T>(*other.items);
    }
    
    ~ArraySequence() override {
        delete items;
    }
    
    T Get_first() const override {
        if (Get_length() == 0) {
            throw EmptyCollection();
        }
        return items -> Get_arr_index(0);
    }
    
    T Get_last() const override {
        if (Get_length() == 0) {
            throw EmptyCollection();
        }
        return items -> Get_arr_index(Get_length() - 1);
    }
    
    T Get(long index) const override {
        return items -> Get_arr_index(index);
    }
    
    long Get_length() const override {
        return items -> Get_length();
    }
    
    Sequence<T>* Get_subsequence(long start_index, long end_index) const override {
        if ((start_index < 0) || (end_index >= Get_length())) {
            throw IndexOutOfRange();
        }
        if (start_index > end_index) {
            throw IndexesTranslated();
        }
        
        long new_size = end_index - start_index + 1;
        
        T *new_items = new T[new_size];
        for (long new_items_index = 0; new_items_index < new_size; new_items_index ++) {
            new_items[new_items_index] = items -> Get_arr_index(start_index + new_items_index);
        }

        ArraySequence<T> *result = new ArraySequence<T>(new_items, new_size);
        delete []new_items;
        return result;
    }
    
    virtual ArraySequence<T>* Clone() const override {
        return new ArraySequence<T>(*this);
    }
    
    Sequence<T>* Append(T item) override {
        ArraySequence<T> *result = Instance();
        long new_size = result -> Get_length() + 1;
        result -> items -> Resize(new_size);
        result -> items -> Set(new_size - 1, item);
        return result;
    }
    
    Sequence<T>* Prepend(T item) override {
        ArraySequence<T> *result = Instance();
        long new_size = result -> Get_length() + 1;
        result -> items -> Resize(new_size);
        for (long items_index = (new_size - 1); items_index > 0; items_index --) {
            result -> items -> Set(items_index, result -> items -> Get_arr_index(items_index - 1));
        }
        result -> items -> Set(0, item);
        return result;
    }
    
    Sequence<T>* Insert_at(T item, long index) override {
        if ((index < 0) || (index > Get_length())) {
            throw IndexOutOfRange();
        }
        
        ArraySequence<T> *result = Instance();
        long new_size = result -> Get_length() + 1;
        result -> items -> Resize(new_size);
        for (long items_index = (new_size - 1); items_index > index; items_index --) {
            result -> items -> Set(items_index, result -> items -> Get_arr_index(items_index - 1));
        }
        result -> items -> Set(index, item);
        return result;
    }
    
    Sequence<T>* Concat(Sequence<T> *other) override {
        if (other == nullptr) {
            throw NullPointerError();
        }

        ArraySequence<T> *result = Instance();
        long new_size = result -> Get_length() + other -> Get_length();

        DynamicArray<T> *new_arr = new DynamicArray<T>(new_size);
        for (long this_items_index = 0; this_items_index < result -> Get_length(); this_items_index ++) {
            new_arr -> Set(this_items_index, result -> Get(this_items_index));
        }
        for (long other_items_index = 0; other_items_index < other -> Get_length(); other_items_index ++) {
            new_arr -> Set(result -> Get_length() + other_items_index, other -> Get(other_items_index));
        }
        delete result -> items;
        result -> items = new_arr;
        return result;
    }
    
    Sequence<T>* Map(T (*function)(T)) const override {
        if (function == nullptr) {
            throw NullPointerError();
        }

        T *new_items = new T[Get_length()];
        for (long new_items_index = 0; new_items_index < Get_length(); new_items_index ++) {
            new_items[new_items_index] = function(Get(new_items_index));
        }

        Sequence<T> *result = new ArraySequence<T>(new_items, Get_length());
        delete []new_items;
        return result;
    }
    
    Sequence<T>* Where(bool (*predicate)(T)) const override {
        if (predicate == nullptr) {
            throw NullPointerError();
        }

        long size = Get_length();

        T *filtered = new T[size];
        long filtered_size = 0;
        for (long items_index = 0; items_index < size; items_index ++) {
            T val = Get(items_index);
            if (predicate(val)) {
                filtered[filtered_size] = val;
                filtered_size ++;
            }
        }

        T *result_items = new T[filtered_size];
        for (long filtered_index = 0; filtered_index < filtered_size; filtered_index ++) {
            result_items[filtered_index] = filtered[filtered_index];
        }

        Sequence<T> *result = new ArraySequence<T>(result_items, filtered_size);
        delete []filtered;
        delete []result_items;
        return result;
    }
    
    T Reduce(T (*function)(T, T), T init) const override {
        if (function == nullptr) {
            throw NullPointerError();
        }

        T result = init;
        for (long items_index = 0; items_index < Get_length(); items_index ++) {
            result = function(result, Get(items_index));
        }
        return result;
    }

    Option<T> Try_get_first(bool (*predicate)(T)) const override {
        if (predicate == nullptr) {
            if (Get_length() > 0) {
                return Option<T>(Get(0));
            }
            return Option<T>();
        }
        
        for (long items_index = 0; items_index < Get_length(); items_index ++) {
            T val = Get(items_index);
            if (predicate(val)) {
                return Option<T>(val);
            }
        }
        return Option<T>();
        }
        
        Option<T> Try_get_last(bool (*predicate)(T)) const override {
        if (predicate == nullptr) {
            if (Get_length() > 0) {
                return Option<T>(Get(Get_length() - 1));
            }
            return Option<T>();
        }
        
        for (long items_index = (Get_length() - 1); items_index >= 0; items_index --) {
            T val = Get(items_index);
            if (predicate(val)) {
                return Option<T>(val);
            }
        }
        return Option<T>();
    }
    
    Sequence<T>* Slice(long start, long count, Sequence<T> *insert_seq = nullptr) override {
        ArraySequence<T> *result = Instance();
        long length = result -> Get_length();
        long actual_start;
        if (start >= 0) {
            actual_start = start;
        }
        else {
            actual_start = length + start;
        }
        if ((actual_start < 0) || (actual_start >= length) || (count < 0) || (actual_start + count > length)) {
            throw IndexOutOfRange();
        }

        long insert_size = 0;
        if (insert_seq != nullptr) {
            insert_size = insert_seq -> Get_length();
        }

        DynamicArray<T> *new_arr = new DynamicArray<T>(length - count + insert_size);
        long index_new = 0;
        for (long actual_start_index = 0; actual_start_index < actual_start; actual_start_index ++) {
            new_arr -> Set(index_new, result -> Get(actual_start_index));
            index_new ++;
        }
        if (insert_seq != nullptr) {
            for (long insert_seq_index = 0; insert_seq_index < insert_size; insert_seq_index ++) {
                new_arr -> Set(index_new, insert_seq -> Get(insert_seq_index));
                index_new ++;
            }
        }
        for (long index_items = (actual_start + count); index_items < length; index_items ++) {
            new_arr -> Set(index_new, result -> Get(index_items));
            index_new ++;
        }
        delete result -> items;
        result -> items = new_arr;
        return result;
    }
    
    Sequence<T>* Zip(Sequence<T> *other) const override {
        if (other == nullptr) {
            throw NullPointerError();
        }

        long min_size;
        if (Get_length() < other -> Get_length()) {
            min_size = Get_length();
        } else {
            min_size = other -> Get_length();
        }

        ArraySequence<T> *result = new ArraySequence<T>();
        for (long result_index = 0; result_index < min_size; result_index ++) {
            result -> Append(Get(result_index));
            result -> Append(other -> Get(result_index));
        }
        return result;
    }
    
    Sequence<T>* Flat_map(Sequence<T>* (*function)(T)) const override {
        if (function == nullptr) {
            throw NullPointerError();
        }

        ArraySequence<T> *result = new ArraySequence<T>();
        for (long items_index = 0; items_index < Get_length(); items_index ++) {
            Sequence<T> *mapped = function(Get(items_index));
            if (mapped != nullptr) {
                for (long mapped_index = 0; mapped_index < mapped -> Get_length(); mapped_index ++) {
                    result -> Append(mapped -> Get(mapped_index));
                }
                delete mapped;
            }
        }
        return result;
    }
    
    Enumerator<T> Get_enumerator() override {
        class ArrayEnumerator : public IEnumerator<T> {
        private:
            ArraySequence<T> *seq;
            long position;
            long length;
            
        public:
            ArrayEnumerator(ArraySequence<T> *s) {
                seq = s;
                position = -1;
                length = s -> Get_length();
            }
            
            bool Move_next() override {
                if ((position + 1) < length) {
                    position ++;
                    return true;
                }
                return false;
            }
            
            T Get_current() override { return seq -> Get(position); }
            
            void Reset() override {
                position = -1;
            }
        };
        return Enumerator<T>(new ArrayEnumerator(this));
    }
};
#endif //ArraySequence_H 