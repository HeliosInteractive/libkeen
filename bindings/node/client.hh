#pragma once

#include <nan.h>
#include "dl.hh"

namespace libkeen
{

class Client final : public Nan::ObjectWrap
{
public:
    Client();
    ~Client();
    void            SetProjectId(const char*);
    void            SetWriteKey(const char*);
    void            SendEvent(const char*, const char*);

public:
    static void     Init(v8::Local<v8::Object> exports);
    static void     New(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     InvokeSetProjectId(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     InvokeSetWriteKey(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     InvokeSendEvent(const Nan::FunctionCallbackInfo<v8::Value>& info);

private:
    bool            Valid() const;

private:
    static Nan::Persistent<v8::Function>
                    sConstructor;
    dl::LoaderRef   mHandle;
    void*           mClient;
};

}
