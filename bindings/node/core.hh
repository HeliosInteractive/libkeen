#pragma once

#include <nan.h>
#include "dl.hh"

namespace libkeen
{

class Core final : public Nan::ObjectWrap
{
public:
    Core();
    ~Core();
    int             UseCount() const;
    void            Flush();
    void            Release();
    void            PostCache(int count);
    void            ClearCache();
    void            EnableFileLogging(bool on);
    void            EnableConsoleLogging(bool on);

public:
    static void     Init(v8::Local<v8::Object> exports);
    static void     New(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     GetUseCount(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     InvokeFlush(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     InvokeRelease(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     InvokePostCache(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     InvokeClearCache(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     InvokeEnableFileLogging(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void     InvokeEnableConsoleLogging(const Nan::FunctionCallbackInfo<v8::Value>& info);

private:
    bool            Valid() const;

private:
    static Nan::Persistent<v8::Function>
                    sConstructor;
    dl::LoaderRef   mHandle;
};

}
