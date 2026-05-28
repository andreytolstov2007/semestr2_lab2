#include <iostream>
#include <limits>
#include <string>
#include "MutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"

void Run_tests();

template <typename T>
void Print_sequence(const Sequence<T> &seq, const std::string &name) {
    std::cout << name << " (length " << seq.Get_length() << "): [";
    for (long element_index = 0; element_index < seq.Get_length(); element_index ++) {
        if (element_index != 0) {
            std::cout << ", ";
        }
        std::cout << seq.Get(element_index);
    }
    std::cout << "]" << std::endl;
}

bool greater_than_five(int x) {
    if (x > 5) {
        return true;
    }
    return false;
}

int square(int x) {
    return x * x;
}

int sum_reducer(int acc, int x) {
    return acc + x;
}

int Read_int(const std::string &prompt) {
    int value;
    std::cout << prompt;
    std::cin >> value;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter an integer: ";
        std::cin >> value;
    }
    return value;
}

template <typename Seq>
Seq* Create_sequence_from_user() {
    int count = Read_int("How many elements? ");
    int *arr = new int[count];
    std::cout << "Enter " << count << " integers: ";
    for (int element_index = 0; element_index < count; element_index ++) {
        std::cin >> arr[element_index];
    }
    Seq *seq = new Seq(arr, count);
    delete []arr;
    return seq;
}

void Array_sequence_menu() {
    Sequence<int> *seq = nullptr;
    std::cout << "\n--- MutableArraySequence ---\n";
    int choice = 0;
    while (choice != 11) {
        std::cout << "\n1. Create new sequence\n";
        std::cout << "2. Append\n";
        std::cout << "3. Prepend\n";
        std::cout << "4. Insert at index\n";
        std::cout << "5. Get element at index\n";
        std::cout << "6. Print\n";
        std::cout << "7. Get subsequence\n";
        std::cout << "8. Concat with another sequence\n";
        std::cout << "9. Map (square)\n";
        std::cout << "10. Where (filter >5) and Reduce (sum)\n";
        std::cout << "11. Exit\n";
        choice = Read_int("Your choice: ");

        if (choice == 1) {
            if (seq != nullptr) {
                delete seq;
            }
            seq = Create_sequence_from_user<MutableArraySequence<int>>();
            Print_sequence(*seq, "Current sequence");
        }
        else if (choice == 2) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int value = Read_int("Value to append: ");
            Sequence<int> *new_seq = seq -> Append(value);
            if (new_seq != seq) {
                delete seq;
                seq = new_seq;
            }
            Print_sequence(*seq, "After append");
        }
        else if (choice == 3) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int value = Read_int("Value to prepend: ");
            Sequence<int> *new_seq = seq -> Prepend(value);
            if (new_seq != seq) {
                delete seq;
                seq = new_seq;
            }
            Print_sequence(*seq, "After prepend");
        }
        else if (choice == 4) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int index = Read_int("Index: ");
            int value = Read_int("Value: ");
            try {
                Sequence<int> *new_seq = seq -> Insert_at(value, index);
                if (new_seq != seq) {
                    delete seq;
                    seq = new_seq;
                }
                Print_sequence(*seq, "After insert");
            }
            catch (const Errors &err) {
                std::cout << "Error: " << err.Message() << std::endl;
            }
        }
        else if (choice == 5) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int index = Read_int("Index: ");
            try {
                std::cout << "Element at index " << index << " = " << seq -> Get(index) << std::endl;
            }
            catch (const Errors &err) {
                std::cout << "Error: " << err.Message() << std::endl;
            }
        }
        else if (choice == 6) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            Print_sequence(*seq, "Sequence");
        }
        else if (choice == 7) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int start = Read_int("Start index: ");
            int end = Read_int("End index: ");
            try {
                Sequence<int> *sub = seq -> Get_subsequence(start, end);
                Print_sequence(*sub, "Subsequence");
                delete sub;
            }
            catch (const Errors &err) {
                std::cout << "Error: " << err.Message() << std::endl;
            }
        }
        else if (choice == 8) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            std::cout << "Create another sequence to concatenate:\n";
            MutableArraySequence<int> *other = Create_sequence_from_user<MutableArraySequence<int>>();
            Sequence<int> *new_seq = seq -> Concat(other);
            if (new_seq != seq) {
                delete seq;
                seq = new_seq;
            }
            Print_sequence(*seq, "Concatenated sequence");
            delete other;
        }
        else if (choice == 9) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            Sequence<int> *mapped = seq -> Map(square);
            Print_sequence(*mapped, "Mapped (square)");
            delete mapped;
        }
        else if (choice == 10) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            Sequence<int> *filtered = seq -> Where(greater_than_five);
            Print_sequence(*filtered, "Filtered (elements >5)");
            delete filtered;
            int sum = seq -> Reduce(sum_reducer, 0);
            std::cout << "Sum of all elements: " << sum << std::endl;
        }
        else if (choice == 11) {
            std::cout << "Exiting array sequence menu.\n";
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }
    if (seq != nullptr) {
        delete seq;
    }
} //Array_sequence_menu

void List_sequence_menu() {
    Sequence<int> *seq = nullptr;
    std::cout << "\n--- MutableListSequence ---\n";
    int choice = 0;
    while (choice != 11) {
        std::cout << "\n1. Create new sequence\n";
        std::cout << "2. Append\n";
        std::cout << "3. Prepend\n";
        std::cout << "4. Insert at index\n";
        std::cout << "5. Get element at index\n";
        std::cout << "6. Print\n";
        std::cout << "7. Get subsequence\n";
        std::cout << "8. Concat with another sequence\n";
        std::cout << "9. Map (square)\n";
        std::cout << "10. Where (filter >5) and Reduce (sum)\n";
        std::cout << "11. Exit\n";
        choice = Read_int("Your choice: ");

        if (choice == 1) {
            if (seq != nullptr) {
                delete seq;
            }
            seq = Create_sequence_from_user<MutableListSequence<int>>();
            Print_sequence(*seq, "Current sequence");
        }
        else if (choice == 2) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int value = Read_int("Value to append: ");
            Sequence<int> *new_seq = seq -> Append(value);
            if (new_seq != seq) {
                delete seq;
                seq = new_seq;
            }
            Print_sequence(*seq, "After append");
        }
        else if (choice == 3) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int value = Read_int("Value to prepend: ");
            Sequence<int> *new_seq = seq -> Prepend(value);
            if (new_seq != seq) {
                delete seq;
                seq = new_seq;
            }
            Print_sequence(*seq, "After prepend");
        }
        else if (choice == 4) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int index = Read_int("Index: ");
            int value = Read_int("Value: ");
            try {
                Sequence<int> *new_seq = seq -> Insert_at(value, index);
                if (new_seq != seq) {
                    delete seq;
                    seq = new_seq;
                }
                Print_sequence(*seq, "After insert");
            }
            catch (const Errors &err) {
                std::cout << "Error: " << err.Message() << std::endl;
            }
        }
        else if (choice == 5) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int index = Read_int("Index: ");
            try {
                std::cout << "Element at index " << index << " = " << seq -> Get(index) << std::endl;
            }
            catch (const Errors &err) {
                std::cout << "Error: " << err.Message() << std::endl;
            }
        }
        else if (choice == 6) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            Print_sequence(*seq, "Sequence");
        }
        else if (choice == 7) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            int start = Read_int("Start index: ");
            int end = Read_int("End index: ");
            try {
                Sequence<int> *sub = seq -> Get_subsequence(start, end);
                Print_sequence(*sub, "Subsequence");
                delete sub;
            }
            catch (const Errors &err) {
                std::cout << "Error: " << err.Message() << std::endl;
            }
        }
        else if (choice == 8) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            std::cout << "Create another sequence to concatenate:\n";
            MutableListSequence<int> *other = Create_sequence_from_user<MutableListSequence<int>>();
            Sequence<int> *new_seq = seq -> Concat(other);
            if (new_seq != seq) {
                delete seq;
                seq = new_seq;
            }
            Print_sequence(*seq, "Concatenated sequence");
            delete other;
        }
        else if (choice == 9) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            Sequence<int> *mapped = seq -> Map(square);
            Print_sequence(*mapped, "Mapped (square)");
            delete mapped;
        }
        else if (choice == 10) {
            if (seq == nullptr) {
                std::cout << "No sequence. Create one first.\n";
                continue;
            }
            Sequence<int> *filtered = seq -> Where(greater_than_five);
            Print_sequence(*filtered, "Filtered (elements >5)");
            delete filtered;
            int sum = seq -> Reduce(sum_reducer, 0);
            std::cout << "Sum of all elements: " << sum << std::endl;
        }
        else if (choice == 11) {
            std::cout << "Exiting list sequence menu.\n";
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }
    if (seq != nullptr) {
        delete seq;
    }
} //List_sequence_menu

int main() {
    std::cout << "=== Interactive Sequence Tester ===\n";
    int type = 0;
    while (type != 4) {
        std::cout << "\nSelect sequence type:\n";
        std::cout << "1. MutableArraySequence\n";
        std::cout << "2. MutableListSequence\n";
        std::cout << "3. Run tests\n";
        std::cout << "4. Exit\n";
        type = Read_int("Choice: ");
        if (type == 1) {
            Array_sequence_menu();
        }
        else if (type == 2) {
            List_sequence_menu();
        }
        else if (type == 3) {
            Run_tests();
        }
        else if (type == 4) {
            std::cout << "Goodbye!\n";
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}