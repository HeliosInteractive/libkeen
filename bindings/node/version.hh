#pragma once

#include <nan.h>
#include "dl.hh"

namespace libkeen
{

class Version final : public Nan::ObjectWrap
{
public:
    Version();
    ~Version();
    int             Major() const;
    int             Minor() const;
    int             Patch() const;

public:
    static void     Init(v8::Local<v8::Object> exports);
    static void     New(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     GetMajor(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     GetMinor(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     GetPatch(const Nan::FunctionCallbackInfo<v8::Value>& info);

private:
    bool            Valid() const;

private:
    static Nan::Persistent<v8::Function>
                    sConstructor;
    dl::LoaderRef   mHandle;
};

}
