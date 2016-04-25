#include "core.hh"

typedef int  (*sig_libkeen_core_use_count)(void);
typedef void (*sig_libkeen_core_flush)(void);
typedef void (*sig_libkeen_core_release)(void);
typedef void (*sig_libkeen_core_post_cache)(int count);
typedef void (*sig_libkeen_core_clear_cache)(void);
typedef void (*sig_libkeen_core_enable_file_logging)(int on);
typedef void (*sig_libkeen_core_enable_console_logging)(int on);

#define LOAD_DYNAMIC_LINK(name) \
static auto name{ mHandle->Load<sig_##name>(#name) }

namespace libkeen
{

Nan::Persistent<v8::Function> Core::sConstructor;

void Core::Init(v8::Local<v8::Object> exports)
{
    Nan::HandleScope scope;
    std::string name{ "Core" };

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New(name).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "useCount", GetUseCount);
    Nan::SetPrototypeMethod(tpl, "flush", InvokeFlush);
    Nan::SetPrototypeMethod(tpl, "release", InvokeRelease);
    Nan::SetPrototypeMethod(tpl, "postCache", InvokePostCache);
    Nan::SetPrototypeMethod(tpl, "clearCache", InvokeClearCache);
    Nan::SetPrototypeMethod(tpl, "enableFileLogging", InvokeEnableFileLogging);
    Nan::SetPrototypeMethod(tpl, "enableConsoleLogging", InvokeEnableConsoleLogging);

    sConstructor.Reset(tpl->GetFunction());
    exports->Set(Nan::New(name).ToLocalChecked(), tpl->GetFunction());
}

void Core::New(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.IsConstructCall())
    {
        // Invoked as constructor: new Type(...)
        (new Core())->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else
    {
        // Invoked as plain function Type(...), turn into construct call.
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(sConstructor);
        info.GetReturnValue().Set(cons->NewInstance());
    }
}

int Core::UseCount() const
{
    if (!Valid()) return -1;
    LOAD_DYNAMIC_LINK(libkeen_core_use_count);

    if (libkeen_core_use_count)
        return libkeen_core_use_count();
    else return -1;
}

void Core::Flush()
{
    if (!Valid()) return;
    LOAD_DYNAMIC_LINK(libkeen_core_flush);

    if (libkeen_core_flush)
        return libkeen_core_flush();
}

void Core::Release()
{
    if (!Valid()) return;
    LOAD_DYNAMIC_LINK(libkeen_core_release);

    if (libkeen_core_release)
        return libkeen_core_release();
}

void Core::PostCache(int count)
{
    if (!Valid()) return;
    LOAD_DYNAMIC_LINK(libkeen_core_post_cache);

    if (libkeen_core_post_cache)
        return libkeen_core_post_cache(count);
}

void Core::ClearCache()
{
    if (!Valid()) return;
    LOAD_DYNAMIC_LINK(libkeen_core_clear_cache);

    if (libkeen_core_clear_cache)
        return libkeen_core_clear_cache();
}

void Core::EnableFileLogging(bool on)
{
    if (!Valid()) return;
    LOAD_DYNAMIC_LINK(libkeen_core_enable_file_logging);

    if (libkeen_core_enable_file_logging)
        return libkeen_core_enable_file_logging(on ? 1 : 0);
}

void Core::EnableConsoleLogging(bool on)
{
    if (!Valid()) return;
    LOAD_DYNAMIC_LINK(libkeen_core_enable_console_logging);

    if (libkeen_core_enable_console_logging)
        return libkeen_core_enable_console_logging(on ? 1 : 0);
}

void Core::GetUseCount(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Core>(info.Holder()))
        info.GetReturnValue().Set(Nan::New(obj->UseCount()));
}

void Core::InvokeFlush(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Core>(info.Holder()))
        obj->Flush();
}

void Core::InvokeRelease(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Core>(info.Holder()))
        obj->Release();
}

void Core::InvokePostCache(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Core>(info.Holder()))
    {
        double count = 0;
        if (info.Length() > 0 && !info[0]->IsUndefined())
            count = info[0]->NumberValue();
        obj->PostCache(static_cast<int>(count));
    }
}

void Core::InvokeClearCache(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Core>(info.Holder()))
        obj->ClearCache();
}

void Core::InvokeEnableFileLogging(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Core>(info.Holder()))
    {
        bool on = true;
        if (info.Length() > 0 && !info[0]->IsUndefined())
            on= info[0]->BooleanValue();
        obj->EnableFileLogging(on);
    }
}

void Core::InvokeEnableConsoleLogging(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Core>(info.Holder()))
    {
        bool on = true;
        if (info.Length() > 0 && !info[0]->IsUndefined())
            on = info[0]->BooleanValue();
        obj->EnableConsoleLogging(on);
    }
}

Core::Core()
    : mHandle(dl::Loader::ref())
{}

Core::~Core()
{}

bool Core::Valid() const
{
    return mHandle && mHandle->valid();
}

}
