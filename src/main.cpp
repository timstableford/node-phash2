#include <napi.h>
#include <pHash.h>

#include "image-hash.h"

Napi::String AboutWrapped(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, ph_about());
}

Napi::Boolean IsImageSupported(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
#ifdef HAVE_IMAGE_HASH
    return Napi::Boolean::New(env, true);
#else
    return Napi::Boolean::New(env, false);
#endif  // HAVE_IMAGE_HASH
}

Napi::Boolean IsVideoSupported(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
#ifdef HAVE_VIDEO_HASH
    return Napi::Boolean::New(env, true);
#else
    return Napi::Boolean::New(env, false);
#endif  // HAVE_VIDEO_HASH
}

Napi::Boolean IsAudioSupported(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
#ifdef HAVE_AUDIO_HASH
    return Napi::Boolean::New(env, true);
#else
    return Napi::Boolean::New(env, false);
#endif  // HAVE_AUDIO_HASH
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set("about", Napi::Function::New(env, AboutWrapped));
    exports.Set("isImageSupported", Napi::Function::New(env, IsImageSupported));
    exports.Set("isVideoSupported", Napi::Function::New(env, IsVideoSupported));
    exports.Set("isAudioSupported", Napi::Function::New(env, IsAudioSupported));

    ImageHash::Init(env, exports);

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)
