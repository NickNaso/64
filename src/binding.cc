// Copyright 2017 Lovell Fuller.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdlib.h>
#include <napi.h>

#include "include/libbase64.h"

Napi::Value Encode(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info[0].IsBuffer()) {
    Napi::Buffer<char> buffer = info[0].As<Napi::Buffer<char>>();
    char const* in = buffer.Data();
    size_t inLen = buffer.Length();
    size_t const outAlloc = 4 + inLen * 4 / 3;
    char* out = static_cast<char*>(malloc(outAlloc));
    size_t outLen;
    base64_encode(in, inLen, out, &outLen, 0);
    return Napi::Buffer<char>::New(env, out, outLen);
  } else {
    throw Napi::TypeError::New(env, "Expected Buffer");
  }
}

Napi::Value Decode(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info[0].IsBuffer()) {
    Napi::Buffer<char> buffer = info[0].As<Napi::Buffer<char>>();
    char const* in = buffer.Data();
    size_t inLen = buffer.Length();
    size_t const outAlloc = 1 + inLen * 3 / 4;
    char* out = static_cast<char*>(malloc(outAlloc));
    size_t outLen;
    base64_decode(in, inLen, out, &outLen, 0);
    return Napi::Buffer<char>::New(env, out, outLen);
  } else {
    throw Napi::TypeError::New(env, "Expected Buffer");
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "encode"),
    Napi::Function::New(env, Encode));
  exports.Set(Napi::String::New(env, "decode"),
    Napi::Function::New(env, Decode));
  return exports;
}

NODE_API_MODULE(base64, Init)
