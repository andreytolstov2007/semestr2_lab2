#ifndef Option_H
#define Option_H

#include "Errors.h"

template <typename T>
class Option {
private:
    bool has_value;
    T value;

public:
    Option() { has_value = false; }
    
    explicit Option(const T &val) {
        has_value = true;
        value = val;
    }
    
    bool Is_some() const {  return has_value; }

    bool Is_none() const { 
        if (has_value == true) {
            return false;
        }
        return true;
    }
    
    T Get() const {
        if (has_value == false) {
            throw EmptyCollection();
        }
        return value;
    }
    
    T Get_or_default(const T &defaultValue) const {
        if (has_value == true) {
            return value;
        } else {
            return defaultValue;
        }
    }
    
    template <typename ResultType>
    Option<ResultType> Map(ResultType (*function)(T)) const {
        if (has_value == true) {
            return Option<ResultType>(function(value));
        } else {
            return Option<ResultType>();
        }
    }
};
#endif //Option_H
