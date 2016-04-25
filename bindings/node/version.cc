#include "version.hh"

typedef int (*sig_libkeen_version_major)(void);
typedef int (*sig_libkeen_version_minor)(void);
typedef int (*sig_libkeen_version_patch)(void);

#define LOAD_DYNAMIC_LINK(name) \
static auto name{ mHandle->Load<sig_##name>(#name) }

namespace libkeen
{

Nan::Persistent<v8::Function> Version::sConstructor;

Version::Version()
    : mHandle(dl::Loader::ref())
{}

Version::~Version()
{}

bool Version::Valid() const
{
    return mHandle && mHandle->valid();
}

int Version::Major() const
{
    if (!Valid()) return -1;
    LOAD_DYNAMIC_LINK(libkeen_version_major);

    if (libkeen_version_major)
        return libkeen_version_major();
    else return -1;
}

int Version::Minor() const
{
    if (!Valid()) return -1;
    LOAD_DYNAMIC_LINK(libkeen_version_minor);

    if (libkeen_version_minor)
        return libkeen_version_minor();
    else return -1;
}

int Version::Patch() const
{
    if (!Valid()) return -1;
    LOAD_DYNAMIC_LINK(libkeen_version_patch);

    if (libkeen_version_patch)
        return libkeen_version_patch();
    else return -1;
}

void Version::Init(v8::Local<v8::Object> exports)
{
    Nan::HandleScope scope;
    std::string name{ "Version" };

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New(name).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "major", GetMajor);
    Nan::SetPrototypeMethod(tpl, "minor", GetMinor);
    Nan::SetPrototypeMethod(tpl, "patch", GetPatch);

    sConstructor.Reset(tpl->GetFunction());
    exports->Set(Nan::New(name).ToLocalChecked(), tpl->GetFunction());
}

void Version::New(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.IsConstructCall())
    {
        // Invoked as constructor: new Type(...)
        (new Version())->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else
    {
        // Invoked as plain function Type(...), turn into construct call.
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(sConstructor);
        info.GetReturnValue().Set(cons->NewInstance());
    }
}

void Version::GetMajor(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Version>(info.Holder()))
        info.GetReturnValue().Set(Nan::New(obj->Major()));
}

void Version::GetMinor(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Version>(info.Holder()))
        info.GetReturnValue().Set(Nan::New(obj->Minor()));
}

void Version::GetPatch(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Version>(info.Holder()))
        info.GetReturnValue().Set(Nan::New(obj->Patch()));
}

}
