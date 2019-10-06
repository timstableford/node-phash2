#include <napi.h>

#ifndef __IMAGE_HASH_H__
#define __IMAGE_HASH_H__

#include <pHash.h>

#define MH_HASH_LENGTH (72)

namespace ImageHash {

#ifdef HAVE_IMAGE_HASH
class DCTImageHash : public Napi::AsyncWorker {
   public:
    DCTImageHash(Napi::Function &callback, std::string &path)
      : AsyncWorker(callback), path(path) {}
    ~DCTImageHash() {}

    void Execute() {
        int retVal = ph_dct_imagehash(this->path.c_str(), this->hash);
        if (retVal != 0) {
            SetError("Failed to calculate image hash");
        }
    }

    void OnOK() {
        Napi::HandleScope scope(Env());

        uint64_t *hash = new uint64_t;
        *hash = this->hash;
        Callback().Call({Env().Null(),
                         Napi::Buffer<uint64_t>::New(
                             Env(), hash, 1,
                             [](Napi::Env, uint64_t *data) { delete data; })});
    }

    static Napi::Value Hash(const Napi::CallbackInfo &info) {
        Napi::Env env = info.Env();
        if (info.Length() < 2 || !info[0].IsString() || !info[1].IsFunction()) {
            Napi::TypeError::New(env, "Expected string parameter and callback")
                .ThrowAsJavaScriptException();
        }
        std::string path = info[0].As<Napi::String>().Utf8Value();
        Napi::Function callback = info[1].As<Napi::Function>();

        DCTImageHash *worker = new DCTImageHash(callback, path);
        worker->Queue();
        return info.Env().Undefined();
    }

   private:
    std::string path;
    uint64_t hash;
};

// MH Hash
class MHImageHash : public Napi::AsyncWorker {
   public:
    MHImageHash(Napi::Function &callback, std::string &path, float alpha,
                float level)
      : AsyncWorker(callback), path(path), alpha(alpha), level(level) {}
    ~MHImageHash() {}

    void Execute() {
        int hashLength;
        this->hash = ph_mh_imagehash(this->path.c_str(), hashLength,
                                     this->alpha, this->level);
        if (this->hash == nullptr || hashLength != MH_HASH_LENGTH) {
            SetError("Failed to calculate image hash");
        }
    }

    void OnOK() {
        Napi::HandleScope scope(Env());

        Callback().Call(
            {Env().Null(), Napi::Buffer<uint8_t>::New(
                               Env(), this->hash, MH_HASH_LENGTH,
                               [](Napi::Env, uint8_t *data) { free(data); })});
    }

    static Napi::Value Hash(const Napi::CallbackInfo &info) {
        Napi::Env env = info.Env();
        if (info.Length() < 4 || !info[0].IsString() || !info[1].IsNumber() ||
            !info[2].IsNumber() || !info[3].IsFunction()) {
            Napi::TypeError::New(
                env,
                "Expected parameters: string(path), number(alpha), "
                "number(level), function(callback)")
                .ThrowAsJavaScriptException();
        }
        std::string path = info[0].As<Napi::String>().Utf8Value();
        float alpha = info[1].As<Napi::Number>();
        float level = info[2].As<Napi::Number>();
        Napi::Function callback = info[3].As<Napi::Function>();

        MHImageHash *worker = new MHImageHash(callback, path, alpha, level);
        worker->Queue();
        return info.Env().Undefined();
    }

   private:
    std::string path;
    float alpha;
    float level;

    uint8_t *hash;
};

#endif  // HAVE_IMAGE_HASH

// Hamming distance (original DCT)
Napi::Number HammingDistanceWrapped(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsBuffer() || !info[1].IsBuffer()) {
        Napi::TypeError::New(
            env, "Expected parameters: buffer(hash A), buffer(hash B)")
            .ThrowAsJavaScriptException();
    }
    Napi::Buffer<uint64_t> hashA = info[0].As<Napi::Buffer<uint64_t>>();
    Napi::Buffer<uint64_t> hashB = info[1].As<Napi::Buffer<uint64_t>>();

    // 1 as in 1 uint64_t
    if (hashA.Length() != 1 || hashB.Length() != 1) {
        Napi::TypeError::New(env, "Both buffers must have a length of 8 bytes")
            .ThrowAsJavaScriptException();
    }

    return Napi::Number::New(env,
                             ph_hamming_distance(*hashA.Data(), *hashB.Data()));
}

// Hamming distance (MH) (can be used for DCT)
Napi::Number HammingDistance2Wrapped(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsBuffer() || !info[1].IsBuffer()) {
        Napi::TypeError::New(
            env, "Expected parameters: buffer(hash A), buffer(hash B)")
            .ThrowAsJavaScriptException();
    }
    Napi::Buffer<uint8_t> hashA = info[0].As<Napi::Buffer<uint8_t>>();
    Napi::Buffer<uint8_t> hashB = info[1].As<Napi::Buffer<uint8_t>>();

    if (hashA.Length() == 0 || hashB.Length() == 0) {
        Napi::TypeError::New(env, "Both buffers must have data in")
            .ThrowAsJavaScriptException();
    }

    return Napi::Number::New(
        env, ph_hammingdistance2(hashA.Data(), hashA.Length(), hashB.Data(),
                                 hashB.Length()));
}

void Init(Napi::Env env, Napi::Object exports) {
#ifdef HAVE_IMAGE_HASH
    exports.Set("getDctImageHash",
                Napi::Function::New(env, DCTImageHash::Hash));
    exports.Set("getMhImageHash", Napi::Function::New(env, MHImageHash::Hash));
#endif  // HAVE_IMAGE_HASH
    exports.Set("getHammingDistance",
                Napi::Function::New(env, HammingDistanceWrapped));
    exports.Set("getHammingDistance2",
                Napi::Function::New(env, HammingDistance2Wrapped));
}

}  // namespace ImageHash

#endif  // __IMAGE_HASH_H__
