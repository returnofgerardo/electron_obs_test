#include <napi.h>
#include <obs.h>

using namespace Napi;

static Number GetNAPIVersion(const CallbackInfo& info)
{
    return Number::New(info.Env(), NAPI_VERSION);
}

Object Init(Env env, Object exports)
{
    obs_startup("en-US", nullptr, nullptr);
    exports.Set(String::New(env, "getNAPIVersion"), Function::New(env, GetNAPIVersion));
    return exports;
}

NODE_API_MODULE(mini-obs, Init)
