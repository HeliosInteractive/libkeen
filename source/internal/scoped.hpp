#pragma once

namespace libkeen {
namespace internal {

/*!
 * @class Scoped
 * @brief Utility RAII class to scope-bound
 *        C-style pointers.
 * @note  Users must specialize pointer types
 *        in scoped.cpp to customize freeing
 *        behavior.
 */
template<typename T>
class Scoped
{
public:
    Scoped(T*& pointer);

    //! template T is left non-implemented.
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
