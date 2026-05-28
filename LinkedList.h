#ifndef LinkedList_H
#define LinkedList_H

#include "Errors.h"
#include "IEnumerator.h"

template <typename T>
class LinkedList {
public:
    LinkedList() {
        top = nullptr;
        low = nullptr;
        list_length = 0;
    }

    LinkedList(T *items, long length) : LinkedList() {
        if (items == nullptr) { throw NullPointerError(); }
        if (length < 0) { throw IndexOutOfRange(); }

        for (long items_index = 0; items_index < length; items_index ++) {
            Append(items[items_index]);
        }
    }

    LinkedList(const LinkedList<T> &other) : LinkedList() {
        Enumerator<T> enumerator = other.Get_enumerator();
        while (enumerator.Move_next()) {
            Prepend(enumerator.Get_current());
        }
    }

    ~LinkedList() {
        Node *current_element;
        for (long counter = 0; counter < list_length; counter ++) {
            current_element = top;
            top = current_element -> next;
            delete current_element;
        }
    }
    
    void Append(T item) {
        Node *new_node = new Node();
        new_node -> data = item;
        new_node -> next = top;
        top = new_node;
        if (list_length == 0) { low = new_node; }

        list_length ++;
    }
    
    void Prepend(T item) {
        Node *new_node = new Node();
        new_node -> data = item;
        new_node -> next = nullptr;
        if (list_length == 0) {
            top = new_node;
        }
        else {
            low -> next = new_node;
        }
        low = new_node;
        list_length ++;
    }

    T Pop() {
        if (list_length == 0) { throw EmptyCollection(); }

        if (list_length == 1) { low = nullptr; }

        Node *to_delete = top;
        top = top -> next;
        T value = to_delete -> data;
        list_length --;
        delete to_delete;
        return value;
    } 

    T Get_first() const {
        if (list_length == 0) { throw EmptyCollection(); }
        return low -> data;
    }

    T Get_last() const {
        if (list_length == 0) { throw EmptyCollection(); }
        return top -> data;
    }

    long Get_length() const { return list_length; }

    T Get_list_index(long list_index) const {
        if ((list_index < 0) || (list_index >= list_length)) { throw IndexOutOfRange(); }

        Node *current_element = top;
        for (long counter = 0; counter < (list_length - 1 - list_index); counter ++) {
            current_element = current_element -> next;
        }
        return current_element -> data;
    }

    LinkedList<T> Get_sub_list(long start_index, long end_index) {
        if ((start_index < 0) || (end_index >= list_length)) { throw IndexOutOfRange(); }
        if (start_index > end_index) { throw IndexesTranslated(); }
        
        LinkedList<T> sub_list;
        Node *current_element = top;
        for (long counter = 0; counter < (list_length - 1 - end_index); counter ++) {
            current_element = current_element -> next;
        }
        for (long counter = start_index; counter <= end_index; counter ++) {
            sub_list.Prepend(current_element -> data);
            current_element = current_element -> next;
        }
        return sub_list;
    }

    void Insert_at(T item, long list_index) {
        if ((list_index < 0) || (list_index > list_length)) { throw IndexOutOfRange(); }

        if (list_index == 0) {
            Prepend(item);
        }
        else if (list_index == list_length) {
            Append(item);
        }
        else {
            Node *current_element = top;
            for (long counter = 0; counter < (list_length - 1 - list_index); counter ++) {
                current_element = current_element -> next;
            }

            Node *new_node = new Node();
            new_node -> data = item;
            new_node -> next = current_element -> next;
            current_element -> next = new_node;
            list_length ++;
        }
    }

    LinkedList<T> Concat(const LinkedList<T> &other_list) const {
        LinkedList<T> result_list;
        Enumerator<T> enumerator = other_list.Get_enumerator();
        while (enumerator.Move_next()) {
            result_list.Prepend(enumerator.Get_current());
        }

        Enumerator<T> enumerator2 = this -> Get_enumerator();
        while (enumerator2.Move_next()) {
            result_list.Prepend(enumerator2.Get_current());
        }
        return result_list;
    }

    LinkedList<T>& operator=(const LinkedList<T> &other) {
        if (this != &other) {
            Node *current_element;
            for (long counter = 0; counter < list_length; counter ++) {
                current_element = top;
                top = top -> next;
                delete current_element;
            }
            low = nullptr;
            list_length = 0;

            Enumerator<T> enumerator = other.Get_enumerator();
            while (enumerator.Move_next()) {
                Prepend(enumerator.Get_current());
            }
        }
        return *this;
    }

    Enumerator<T> Get_enumerator() const {
        class LinkedListEnumerator : public IEnumerator<T> {
        private:
            const LinkedList<T> *list;
            Node *current_node;
            long position;
            long length;
            
        public:
            LinkedListEnumerator(const LinkedList<T> *list2) {
                list = list2;
                current_node = nullptr;
                position = -1;
                length = list2 -> list_length;
            }
            
            bool Move_next() override {
                if ((position + 1) < length) {
                    position ++;
                    if (position == 0) {
                        current_node = list -> top;
                    }
                    else {
                        current_node = current_node -> next;
                    }
                    return true;
                }
                return false;
            }
            
            T Get_current() override { return current_node -> data; }
            
            void Reset() override {
                position = -1;
                current_node = nullptr;
            }
        };
        return Enumerator<T>(new LinkedListEnumerator(this));
    }

private:
    struct Node {
        T data;
        Node *next;
    };

    Node *top;
    Node *low;
    long list_length;
};
#endif //LinkedList_H
