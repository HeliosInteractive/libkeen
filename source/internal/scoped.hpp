#pragma once

namespace libkeen {
namespace internal {

template<typename T>
class Scoped
{
public:
    Scoped(T*& pointer);
    void onScopeEnd();
    ~Scoped();
private:
    T*& mPointer;
};

template<typename T>
Scoped<T>::Scoped(T*& pointer)
    : mPointer(pointer)
{}

template<typename T>
Scoped<T>::~Scoped()
{
    onScopeEnd();
    mPointer = nullptr;
}

}}
