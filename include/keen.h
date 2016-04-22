#pragma once

//! @cond
// Compiler specific shared library symbol visibility
#if defined(_WIN32) && defined(LIBKEEN_BUILD_DLL)
 /* We are building libkeen as a DLL */
 #define LIBKEEN_API __declspec(dllexport)
#elif defined(_WIN32)
 /* We are calling libkeen as a DLL */
 #define LIBKEEN_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(LIBKEEN_BUILD_DLL)
 /* We are building libkeen as a shared / dynamic library */
 #define LIBKEEN_API __attribute__((visibility("default")))
#else
 /* We are building or calling libkeen as a static library */
 #define LIBKEEN_API
#endif

#define LIBKEEN_BOOL_FALSE 0
#define LIBKEEN_BOOL_TRUE  1

//! @endcond

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

LIBKEEN_API int     libkeen_version_major(void);
LIBKEEN_API int     libkeen_version_minor(void);
LIBKEEN_API int     libkeen_version_patch(void);
LIBKEEN_API void*   libkeen_client_keenio_make(void);
LIBKEEN_API void    libkeen_client_keenio_free(void* instance);
LIBKEEN_API void    libkeen_client_keenio_set_project_id(void* instance, const char* id);
LIBKEEN_API void    libkeen_client_keenio_set_write_key(void* instance, const char* key);
LIBKEEN_API void    libkeen_client_send_event(void* instance, const char* collection, const char* json);
LIBKEEN_API int     libkeen_core_use_count(void);
LIBKEEN_API void    libkeen_core_flush(void);
LIBKEEN_API void    libkeen_core_release(void);
LIBKEEN_API void    libkeen_core_post_cache(int count);
LIBKEEN_API void    libkeen_core_clear_cache(void);
LIBKEEN_API void    libkeen_core_enable_file_logging(int on);
LIBKEEN_API void    libkeen_core_enable_console_logging(int on);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
