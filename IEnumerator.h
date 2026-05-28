#ifndef IEnumerator_H
#define IEnumerator_H

template <typename T>
class IEnumerator {
public:
    virtual ~IEnumerator() {}
    virtual bool Move_next() = 0;
    virtual T Get_current() = 0;
    virtual void Reset() = 0;
};

template <typename T>
class Enumerator : public IEnumerator<T> {
public:
    explicit Enumerator(IEnumerator<T> *point) : wrapped(point) {}
    virtual ~Enumerator() { delete wrapped; }
    bool Move_next() override { return wrapped -> Move_next(); }
    T Get_current() override { return wrapped -> Get_current(); }
    void Reset() override { wrapped -> Reset(); }
private:
    IEnumerator<T> *wrapped;
};

template <typename T>
class IEnumerable {
public:
    virtual ~IEnumerable() {}
    virtual Enumerator<T> Get_enumerator() = 0;
};
#endif //IEnumerator_H