#ifndef ListSequence_H
#define ListSequence_H

#include "Sequence.h"
#include "LinkedList.h"

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> *items;
    
    explicit ListSequence(LinkedList<T> *list) { items = list; }

    virtual ListSequence<T>* Instance() { return this; }

public:
    ListSequence() { items = new LinkedList<T>(); }
    
    ListSequence(T *items_arr, long count) { items = new LinkedList<T>(items_arr, count); }
    
    ListSequence(const ListSequence<T> &other) { items = new LinkedList<T>(*other.items); }
    
    ~ListSequence() override { delete items; }
    
    T Get_first() const override {
        if (Get_length() == 0) { throw EmptyCollection(); }
        return items -> Get_first();
    }
    
    T Get_last() const override {
        if (Get_length() == 0) { throw EmptyCollection(); }
        return items -> Get_last();
    }
    
    T Get(long index) const override {
        if ((index < 0) || (index >= Get_length())) { throw IndexOutOfRange(); }
        return items -> Get_list_index(index);
    }
    
    long Get_length() const override { return items -> Get_length(); }
    
    Sequence<T>* Get_subsequence(long start_index, long end_index) const override {
        if ((start_index < 0) || (end_index >= Get_length())) { throw IndexOutOfRange(); }
        if (start_index > end_index) { throw IndexesTranslated(); }

        ListSequence<T> *result = new ListSequence<T>();
        Enumerator<T> enumerator = items -> Get_enumerator();
        long length = Get_length();
        long skip_count = length - 1 - end_index;
        long elements_count = end_index - start_index + 1;
        for (long skip_counter = 0; skip_counter < skip_count; skip_counter ++) {
            enumerator.Move_next();
        }
        
        for (long counter = 0; counter < elements_count; counter ++) {
            enumerator.Move_next();
            result -> Append(enumerator.Get_current());
        }
        
        return result;
    }
    
    ListSequence<T>* Clone() const override { return new ListSequence<T>(*this); }
    
    Sequence<T>* Append(T item) override {
        ListSequence<T> *result = Instance();
        result -> items -> Append(item);
        return result;
    }
    
    Sequence<T>* Prepend(T item) override {
        ListSequence<T> *result = Instance();
        result -> items -> Prepend(item);
        return result;
    }
    
    Sequence<T>* Insert_at(T item, long index) override {
        if ((index < 0) || (index > Get_length())) { throw IndexOutOfRange(); }

        ListSequence<T> *result = Instance();
        result -> items -> Insert_at(item, index);
        return result;
    }
    
    Sequence<T>* Concat(Sequence<T> *other) override {
        if (other == nullptr) { throw NullPointerError(); }

        ListSequence<T> *result = Instance();
        LinkedList<T> *new_list = new LinkedList<T>();

        Enumerator<T> enumerator = result -> items -> Get_enumerator();
        while (enumerator.Move_next()) {
            new_list -> Prepend(enumerator.Get_current());
        }

        Enumerator<T> enumerator2 = other -> Get_enumerator();
        while (enumerator2.Move_next()) {
            new_list -> Prepend(enumerator2.Get_current());
        }
        delete result -> items;
        result -> items = new_list;
        return result;
    }

    Sequence<T>* Map(T (*function)(T)) const override {
        if (function == nullptr) { throw NullPointerError(); }

        ListSequence<T> *result = new ListSequence<T>();
        Enumerator<T> enumerator = items -> Get_enumerator();
        while (enumerator.Move_next()) {
            T value = function(enumerator.Get_current());
            result -> Prepend(value);
        }
        return result;
    }
    
    Sequence<T>* Where(bool (*predicate)(T)) const override {
        if (predicate == nullptr) { throw NullPointerError(); }

        ListSequence<T> *result = new ListSequence<T>();
        Enumerator<T> enumerator = items -> Get_enumerator();
        while (enumerator.Move_next()) {
            T value = enumerator.Get_current();
            if (predicate(value)) {
                result -> Prepend(value);
            }
        }
        return result;
    }
    
    T Reduce(T (*function)(T, T), T init) const override {
        if (function == nullptr) { throw NullPointerError(); }
        
        T result = init;
        Enumerator<T> enumerator = items -> Get_enumerator();
        while (enumerator.Move_next()) {
            result = function(result, enumerator.Get_current());
        }
        return result;
    }
    
    Option<T> Try_get_first(bool (*predicate)(T)) const override {
        if (predicate == nullptr) {
            if (Get_length() > 0) {
                return Option<T>(Get_first());
            }
            return Option<T>();
        }
        
        Enumerator<T> enumerator = items -> Get_enumerator();
        Option<T> result;
        while (enumerator.Move_next()) {
            T value = enumerator.Get_current();
            if (predicate(value)) {
                result = Option<T>(value);
            }
        }
        return result;
    }
    
    Option<T> Try_get_last(bool (*predicate)(T)) const override {
        if (predicate == nullptr) {
            if (Get_length() > 0) {
                return Option<T>(Get_last());
            }
            return Option<T>();
        }
        
        Enumerator<T> enumerator = items -> Get_enumerator();
        while (enumerator.Move_next()) {
            T value = enumerator.Get_current();
            if (predicate(value)) {
                return Option<T>(value);
            }
        }
        return Option<T>();
    }
    
    Sequence<T>* Slice(long start, long count, Sequence<T> *insert_seq = nullptr) override {
        ListSequence<T> *result = Instance();
        long length = result -> Get_length();
        if ((start < 0) || (count < 0) || (start >= length) || (start + count > length)) {
            throw IndexOutOfRange();
        }

        LinkedList<T> *new_list = new LinkedList<T>();
        Enumerator<T> enumerator = result -> items -> Get_enumerator();
        long tail_count = length - start - count;
        for (long counter = 0; counter < tail_count; counter ++) {
            enumerator.Move_next();
            new_list -> Prepend(enumerator.Get_current());
        }
        for (long skip_counter = 0; skip_counter < count; skip_counter ++) {
            enumerator.Move_next();
        }
        
        if (insert_seq != nullptr) {
            Enumerator<T> insert_enum = insert_seq -> Get_enumerator();
            while (insert_enum.Move_next()) {
                new_list -> Prepend(insert_enum.Get_current());
            }
        }
        for (long counter = 0; counter < start; counter ++) {
            enumerator.Move_next();
            new_list -> Prepend(enumerator.Get_current());
        }
        
        delete result -> items;
        result -> items = new_list;
        return result;
    }
    
    Sequence<T>* Zip(Sequence<T> *other) const override {
        if (other == nullptr) { throw NullPointerError(); }

        ListSequence<T> *result = new ListSequence<T>();
        Enumerator<T> this_enumerator = items -> Get_enumerator();
        Enumerator<T> other_enumerator = other -> Get_enumerator();
        while (this_enumerator.Move_next() && other_enumerator.Move_next()) {
            result -> Prepend(other_enumerator.Get_current());
            result -> Prepend(this_enumerator.Get_current());
        }
        return result;
    }
    
    Sequence<T>* Flat_map(Sequence<T>* (*function)(T)) const override {
        if (function == nullptr) { throw NullPointerError(); }
        
        ListSequence<T> *result = new ListSequence<T>();
        Enumerator<T> enumerator = items -> Get_enumerator();
        while (enumerator.Move_next()) {
            Sequence<T> *mapped = function(enumerator.Get_current());
            if (mapped != nullptr) {
                Enumerator<T> mapped_enumerator = mapped -> Get_enumerator();
                while (mapped_enumerator.Move_next()) {
                    result -> Append(mapped_enumerator.Get_current());
                }
                delete mapped;
            }
        }
        return result;
    }
    
    Enumerator<T> Get_enumerator() override {
        class ListSequenceEnumerator : public IEnumerator<T> {
        private:
            const ListSequence<T> *seq;
            long position;
            long length;
            
        public:
            ListSequenceEnumerator(ListSequence<T> *s) {
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
            
            void Reset() override { position = -1; }
        };
        return Enumerator<T>(new ListSequenceEnumerator(this));
    }

    bool operator==(const Sequence<T> &other) const override {
        if (Get_length() != other.Get_length()) { return false; }
        
        long length = Get_length();

        Enumerator<T> this_enum = items -> Get_enumerator();
        bool equal = true;
        for (long compare_index = 0; compare_index < length; compare_index ++) {
            this_enum.Move_next();
            T this_value = this_enum.Get_current();
            if (this_value != other.Get(length - 1 - compare_index)) {
                equal = false;
                break;
            }
        }
        return equal;
    }
};
#endif //ListSequence_H
