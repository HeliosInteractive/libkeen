#pragma once

#include <memory>
#include <string>

namespace dl
{

// platform independent dl helpers
void* open(const std::string& path);
void* sym(void* handle, const std::string& name);
void  close(void* handle);

// ref counted type of Loader, auto unloads.
using LoaderRef = std::shared_ptr< class Loader >;

// helper for loading modules
class Loader
{
public:
    Loader(const std::string& path);
    ~Loader();

    template<typename T>
    T                   Load(const std::string& signature);
    bool                valid() const;
    static LoaderRef    ref();

private:
    void*               mHandle;
};

// template impl
template<typename T>
T Loader::Load(const std::string& signature)
{
    if (!mHandle || signature.empty()) return nullptr;
    return static_cast<T>(sym(mHandle, signature));
}

}
