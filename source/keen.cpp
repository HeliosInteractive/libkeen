#include "keen.h"

#include "keen/version.hpp"
#include "keen/client.hpp"
#include "internal/core.hpp"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

LIBKEEN_API int libkeen_version_major(void) {
    return libkeen::Version::getMajor();
}

LIBKEEN_API int libkeen_version_minor(void) {
    return libkeen::Version::getMinor();
}

LIBKEEN_API int libkeen_version_patch(void) {
    return libkeen::Version::getPatch();
}

LIBKEEN_API void* libkeen_client_keenio_make(void) {
    return static_cast<void*>(new libkeen::ClientKeenIO);
}

LIBKEEN_API void libkeen_client_keenio_free(void* instance) {
    if (!instance) return;
    if (auto ptr = reinterpret_cast<libkeen::ClientKeenIO*>(instance))
        delete ptr;
}

LIBKEEN_API void libkeen_client_keenio_set_project_id(void* instance, const char* id) {
    if (!id) return;
    if (auto ptr = reinterpret_cast<libkeen::ClientKeenIO*>(instance))
        ptr->setProjectId(id);
}

LIBKEEN_API void libkeen_client_keenio_set_write_key(void* instance, const char* key) {
    if (!key) return;
    if (auto ptr = reinterpret_cast<libkeen::ClientKeenIO*>(instance))
        ptr->setWriteKey(key);
}

LIBKEEN_API void libkeen_client_send_event(void* instance, const char* collection, const char* json) {
    if (!collection || !json) return;
    if (auto ptr = reinterpret_cast<libkeen::Client*>(instance))
        ptr->sendEvent(collection, json);
}

LIBKEEN_API int libkeen_core_use_count(void) {
    return static_cast<int>(libkeen::internal::Core::instance()->useCount());
}

LIBKEEN_API void libkeen_core_flush(void) {
    libkeen::internal::Core::instance()->flush();
}

LIBKEEN_API void libkeen_core_release(void) {
    libkeen::internal::Core::instance()->release();
}

LIBKEEN_API void libkeen_core_post_cache(int count) {
    libkeen::internal::Core::instance()->postCache(static_cast<unsigned>(count));
}

LIBKEEN_API void libkeen_core_clear_cache(void) {
    libkeen::internal::Core::instance()->clearCache();
}

LIBKEEN_API void libkeen_core_enable_file_logging(int on) {
    libkeen::internal::Core::instance()->enableLogToFile(on == LIBKEEN_BOOL_TRUE);
}

LIBKEEN_API void libkeen_core_enable_console_logging(int on) {
    libkeen::internal::Core::instance()->enableLogToConsole(on == LIBKEEN_BOOL_TRUE);
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
