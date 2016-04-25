#include "dl.hh"

#ifdef _WIN32
#include <windows.h>

void* dl::open(const std::string& path)
{
    if (path.empty()) return nullptr;
    auto handle = ::LoadLibraryA(path.c_str());

    if (handle != NULL) return static_cast<void*>(handle);
    else return nullptr;
}

void* dl::sym(void* handle, const std::string& name)
{
    if (!handle || name.empty()) return nullptr;
    
    auto proc = GetProcAddress(static_cast<HMODULE>(handle), name.c_str());
    if (proc != NULL) return static_cast<void*>(proc);
    else return nullptr;
}

void dl::close(void* handle)
{
    if (!handle) return;
    ::FreeLibrary(static_cast<HMODULE>(handle));
}
#endif // _WIN32

namespace dl
{

Loader::Loader(const std::string& path) : mHandle(nullptr)
{
    if (path.empty()) return;
    mHandle = open(path);
}

bool Loader::valid() const
{
    return mHandle != nullptr;
}

LoaderRef Loader::ref()
{
    static LoaderRef instance{ new Loader("libkeen.dll") };
    return instance;
}

Loader::~Loader()
{
    if (mHandle) close(mHandle);
    mHandle = nullptr;
}

}
