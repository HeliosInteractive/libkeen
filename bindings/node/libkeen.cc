#include <nan.h>

#include "version.hh"
#include "client.hh"
#include "core.hh"

namespace libkeen
{

static void shutdown(void* /*ctx*/)
{
    Core core;
    core.Flush();
    core.Release();
}

void Init(v8::Local<v8::Object> exports)
{
    node::AtExit(shutdown);
    Version::Init(exports);
    Client::Init(exports);
    Core::Init(exports);
}

}

NODE_MODULE(libkeen, libkeen::Init)
