# jsi-utils
helpers for JSI code
```c
  jsi::String jsi_str = jsi::String::createFromUtf8(*rt, "hello");

  std::string cpp_str = "hello";

  std::string res =  jsiConvert<std::string>(runtime, jsi_str);
  jsi::String res2 = jsiConvert<jsi::String>(runtime, cpp_str);

  jsi::Function fn = createFunction(runtime,[](jsi::Runtime& rt, const jsi::Value* args) {
    //...
    return jsi::Value(...) ;
  });

  jsi::Value promise = createPromise(runtime, [](jsi::Runtime &rt, jsi::Function res, jsi::Function rej){
    //...
    res.call(rt, jsi::Value(...));
  });

```
