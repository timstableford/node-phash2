#include <napi.h>

#ifndef __VIDEO_HASH_H__
#define __VIDEO_HASH_H__

#include <pHash.h>

namespace VideoHash {

#ifdef HAVE_VIDEO_HASH

class DCTVideoHash : public Napi::AsyncWorker {
   public:
    DCTVideoHash(Napi::Function &callback, std::string &path)
      : AsyncWorker(callback), path(path) {}
    ~DCTVideoHash() {}

    void Execute() {
        this->hash = ph_dct_videohash(this->path.c_str(), this->hashLength);
        if (this->hash == nullptr || this->hashLength == 0) {
            if (this->hash != nullptr) {
                free(this->hash);
            }
            SetError("Failed to calculate image hash");
        }
    }

    void OnOK() {
        Napi::HandleScope scope(Env());

        Callback().Call(
            {Env().Null(), Napi::Buffer<uint64_t>::New(
                               Env(), this->hash, this->hashLength,
                               [](Napi::Env, uint64_t *data) { free(data); })});
    }

    static Napi::Value Hash(const Napi::CallbackInfo &info) {
        Napi::Env env = info.Env();
        if (info.Length() < 2 || !info[0].IsString() || !info[1].IsFunction()) {
            Napi::TypeError::New(
                env, "Expected parameters: string(path), function(callback)")
                .ThrowAsJavaScriptException();
        }
        std::string path = info[0].As<Napi::String>().Utf8Value();
        Napi::Function callback = info[1].As<Napi::Function>();

        DCTVideoHash *worker = new DCTVideoHash(callback, path);
        worker->Queue();
        return info.Env().Undefined();
    }

   private:
    std::string path;

    int hashLength;
    uint64_t *hash;
};

class DCTVideoDistance : public Napi::AsyncWorker {
   public:
    DCTVideoDistance(Napi::Function &callback, uint64_t *hashA, int hashALength,
                     uint64_t *hashB, int hashBLength, int threshold)
      : AsyncWorker(callback),
        hashA(hashA),
        hashALength(hashALength),
        hashB(hashB),
        hashBLength(hashBLength),
        threshold(threshold) {}
    ~DCTVideoDistance() {}

    void Execute() {
        this->distance =
            ph_dct_videohash_dist(this->hashA, this->hashALength, this->hashB,
                                  this->hashBLength, this->threshold);
    }

    void OnOK() {
        Napi::HandleScope scope(Env());

        Callback().Call(
            {Env().Null(), Napi::Number::New(Env(), this->distance)});
    }

    static Napi::Value Distance(const Napi::CallbackInfo &info) {
        Napi::Env env = info.Env();
        if (info.Length() < 4 || !info[0].IsBuffer() || !info[1].IsBuffer() ||
            !info[2].IsNumber() || !info[3].IsFunction()) {
            Napi::TypeError::New(
                env,
                "Expected parameters: buffer(hash A), buffer(hash B), "
                "number(threshold), function(callback)")
                .ThrowAsJavaScriptException();
        }
        Napi::Buffer<uint64_t> hashA = info[0].As<Napi::Buffer<uint64_t>>();
        Napi::Buffer<uint64_t> hashB = info[1].As<Napi::Buffer<uint64_t>>();
        Napi::Number threshold = info[2].As<Napi::Number>();
        Napi::Function callback = info[3].As<Napi::Function>();

        DCTVideoDistance *worker =
            new DCTVideoDistance(callback, hashA.Data(), hashA.Length(),
                                 hashB.Data(), hashB.Length(), threshold);
        worker->Queue();
        return info.Env().Undefined();
    }

   private:
    std::string path;
    uint64_t *hashA;
    int hashALength;
    uint64_t *hashB;
    int hashBLength;
    int threshold;

    double distance;
};

#endif  // HAVE_VIDEO_HASH

void Init(Napi::Env env, Napi::Object exports) {
#ifdef HAVE_VIDEO_HASH
    exports.Set("getDctVideoHash",
                Napi::Function::New(env, DCTVideoHash::Hash));
    exports.Set("getVideoDistance",
                Napi::Function::New(env, DCTVideoDistance::Distance));
#endif  // HAVE_VIDEO_HASH
}

}  // namespace VideoHash

#endif  // __VIDEO_HASH_H__
