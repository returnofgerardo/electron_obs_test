#include <napi.h>

using namespace Napi;

Object Init(Env env, Object exports)
{
    return exports;
}

NODE_API_MODULE(mini-obs, Init)
