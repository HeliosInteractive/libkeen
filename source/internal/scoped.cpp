#pragma once

#include "scoped.hpp"
#include "curl/curl.h"
#include "sqlite3.h"

namespace libkeen {
namespace internal {

template<>
void Scoped<CURL>::onScopeEnd()
{
    if (!mPointer) return;
    curl_easy_cleanup(mPointer);
}

template<>
void Scoped<curl_slist>::onScopeEnd()
{
    if (!mPointer) return;
    curl_slist_free_all(mPointer);
}

template<>
void Scoped<sqlite3_stmt>::onScopeEnd()
{
    if (!mPointer) return;
    sqlite3_finalize(mPointer);
}

}}
