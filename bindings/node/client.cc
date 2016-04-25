#include "client.hh"
#include <iostream>

typedef void* (*sig_libkeen_client_keenio_make)(void);
typedef void  (*sig_libkeen_client_keenio_free)(void* instance);
typedef void  (*sig_libkeen_client_keenio_set_project_id)(void* instance, const char* id);
typedef void  (*sig_libkeen_client_keenio_set_write_key)(void* instance, const char* key);
typedef void  (*sig_libkeen_client_send_event)(void* instance, const char* collection, const char* json);

#define LOAD_DYNAMIC_LINK(name) \
static auto name{ mHandle->Load<sig_##name>(#name) }

namespace libkeen
{

Nan::Persistent<v8::Function> Client::sConstructor;

void Client::Init(v8::Local<v8::Object> exports)
{
    Nan::HandleScope scope;
    std::string name{ "Client" };

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New(name).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(2);

    Nan::SetPrototypeMethod(tpl, "setProjectId", InvokeSetProjectId);
    Nan::SetPrototypeMethod(tpl, "setWriteKey", InvokeSetWriteKey);
    Nan::SetPrototypeMethod(tpl, "sendEvent", InvokeSendEvent);

    sConstructor.Reset(tpl->GetFunction());
    exports->Set(Nan::New(name).ToLocalChecked(), tpl->GetFunction());
}

void Client::New(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.IsConstructCall())
    {
        // Invoked as constructor: new Type(...)
        (new Client())->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else
    {
        // Invoked as plain function Type(...), turn into construct call.
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(sConstructor);
        info.GetReturnValue().Set(cons->NewInstance());
    }
}

void Client::InvokeSetProjectId(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Client>(info.Holder()))
    {
        if (info.Length() > 0 && !info[0]->IsUndefined() && info[0]->IsString())
            obj->SetProjectId(*v8::String::Utf8Value(info[0]));
    }
}

void Client::InvokeSetWriteKey(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Client>(info.Holder()))
    {
        if (info.Length() > 0 && !info[0]->IsUndefined() && info[0]->IsString())
            obj->SetWriteKey(*v8::String::Utf8Value(info[0]));
    }
}

void Client::InvokeSendEvent(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    if (auto obj = ObjectWrap::Unwrap<Client>(info.Holder()))
    {
        if (info.Length() > 1 &&
            !info[0]->IsUndefined() && info[0]->IsString() &&
            !info[1]->IsUndefined() && info[1]->IsString())
            obj->SendEvent(*v8::String::Utf8Value(info[0]), *v8::String::Utf8Value(info[1]));
    }
}

bool Client::Valid() const
{
    return mHandle && mHandle->valid() && mClient;
}

Client::Client()
    : mHandle(dl::Loader::ref())
    , mClient(nullptr)
{
    if (!(mHandle && mHandle->valid())) return;

    LOAD_DYNAMIC_LINK(libkeen_client_keenio_make);

    if (libkeen_client_keenio_make)
        mClient = libkeen_client_keenio_make();
}

Client::~Client()
{
    if (!Valid()) return;

    LOAD_DYNAMIC_LINK(libkeen_client_keenio_free);

    if (libkeen_client_keenio_free)
        libkeen_client_keenio_free(mClient);
}

void Client::SetProjectId(const char* id)
{
    if (!Valid()) return;

    LOAD_DYNAMIC_LINK(libkeen_client_keenio_set_project_id);

    if (libkeen_client_keenio_set_project_id)
        libkeen_client_keenio_set_project_id(mClient, id);
}

void Client::SetWriteKey(const char* key)
{
    if (!Valid()) return;

    LOAD_DYNAMIC_LINK(libkeen_client_keenio_set_write_key);

    if (libkeen_client_keenio_set_write_key)
        libkeen_client_keenio_set_write_key(mClient, key);
}

void Client::SendEvent(const char* collection, const char* event)
{
    if (!Valid()) return;

    LOAD_DYNAMIC_LINK(libkeen_client_send_event);

    if (libkeen_client_send_event)
        libkeen_client_send_event(mClient, collection, event);
}

}
