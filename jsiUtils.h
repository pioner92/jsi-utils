//
//  jsiUtils.h
//  intch_application
//
//  Created by Oleksandr Shumihin on 2/4/25.
//  Copyright © 2025 Facebook. All rights reserved.
//

#ifndef jsiUtils_h
#define jsiUtils_h

#pragma once

#include "jsi/jsi.h"

using namespace facebook;
using namespace std;

namespace jsiUtils {
template <typename T, typename V>
T jsiConvert(jsi::Runtime& rt, const V& arg);

template <>
inline std::string jsiConvert<std::string, jsi::String>(
    jsi::Runtime& rt,
    const jsi::String& arg) {
  return arg.utf8(rt);
}

template <>
inline jsi::String jsiConvert<jsi::String, std::string>(
    jsi::Runtime& rt,
    const std::string& arg) {
  return jsi::String::createFromUtf8(rt, arg);
}

#if defined(__APPLE__)
template <>
inline NSString* jsiConvert<NSString*, std::string>(jsi::Runtime& rt,
                                                    const std::string& arg) {
  return [NSString stringWithUTF8String:arg.c_str()];
}
#endif

inline jsi::Value createPromise(
    jsi::Runtime& rt,
    std::function<void(jsi::Runtime& rt,
                       jsi::Function resolve,
                       jsi::Function reject)> executor) {
  auto promiseCallbac = jsi::Function::createFromHostFunction(
      rt, jsi::PropNameID::forAscii(rt, "promise"), 2,
      [executor = std::move(executor)](jsi::Runtime& rt,
                                       const jsi::Value& thisVal,
                                       const jsi::Value* args, size_t count) {
        executor(rt, args[0].getObject(rt).getFunction(rt),
                 args[1].getObject(rt).getFunction(rt));
        return jsi::Value::undefined();
      });

  return rt.global()
      .getPropertyAsFunction(rt, "Promise")
      .callAsConstructor(rt, std::move(promiseCallbac), 1);
}

inline jsi::Function createFunction(
    jsi::Runtime& rt,
    std::function<jsi::Value(jsi::Runtime& rt, const jsi::Value* args)>
        callback) {
  return jsi::Function::createFromHostFunction(
      rt, jsi::PropNameID::forAscii(rt, "function"), 1,
      [callback = std::move(callback)](
          jsi::Runtime& rt, const jsi::Value& thisVal, const jsi::Value* args,
          size_t count) { return callback(rt, args); });
}

inline jsi::Function createFunction(
    jsi::Runtime& rt,
    std::function<jsi::Value(jsi::Runtime& rt,
                             const jsi::Value* args,
                             size_t argsCount)> callback) {
  return jsi::Function::createFromHostFunction(
      rt, jsi::PropNameID::forAscii(rt, "function"), 1,
      [callback = std::move(callback)](
          jsi::Runtime& rt, const jsi::Value& thisVal, const jsi::Value* args,
          size_t count) { return callback(rt, args, count); });
}

}  // namespace jsiUtils

#endif /* jsiUtils_h */
