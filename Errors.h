#ifndef Errors_H
#define Errors_H

#include <string>

class Errors {
public:
    explicit Errors(const std::string &msg) {
        error_message = msg;
    }
    virtual ~Errors() {}
    virtual const std::string& Message() const {
        return error_message;
    }

protected:
    std::string error_message;
};

class IndexOutOfRange : public Errors {
public:
    IndexOutOfRange() : Errors("Index is out of range.") {}
};

class EmptyCollection : public Errors {
public:
    EmptyCollection() : Errors("Collection is empty.") {}
};

class NullPointerError : public Errors {
public:
    NullPointerError() : Errors("Null pointer is not allowed.") {}
};

class InvalidArgument : public Errors {
public:
    InvalidArgument() : Errors("Invalid argument.") {}
};

class IndexesTranslated : public Errors {
public:
    IndexesTranslated() : Errors("Start index is greater than end index.") {}
};
#endif //Errors_H