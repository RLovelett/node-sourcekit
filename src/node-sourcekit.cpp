// hello.cc
#include <functional>
#include <nan.h> // includes v8 too

// Include SourceKit
#include "sourcekitd/sourcekitd.h"

#include "SourceKitWorker.h"

auto valueToCString = [](v8::Local<v8::Value> value) -> Nan::Utf8String* {
    Nan::Utf8String* temp = new Nan::Utf8String(value);
    return temp;
};

auto valueToInt64 = [](v8::Local<v8::Value> value) -> Nan::Maybe<int64_t> {
    return Nan::To<int64_t>(value);
};

auto valueToArray =
    [](v8::Local<v8::Value> value) -> Nan::MaybeLocal<v8::Array> {
    if (value->IsArray()) {
        return v8::Local<v8::Array>::Cast(value);
    } else {
        return Nan::MaybeLocal<v8::Array>();
    }
};

Nan::Maybe<Nan::Utf8String*>
map(Nan::MaybeLocal<v8::Value> box,
    std::function<Nan::Utf8String*(v8::Local<v8::Value>)> func) {
    if (box.IsEmpty() || box.ToLocalChecked()->IsUndefined()) {
        return Nan::Nothing<Nan::Utf8String*>();
    } else {
        return Nan::Just<Nan::Utf8String*>(func(box.ToLocalChecked()));
    }
}

Nan::Maybe<int64_t>
flatMap(Nan::MaybeLocal<v8::Value> box,
        std::function<Nan::Maybe<int64_t>(v8::Local<v8::Value>)> func) {
    if (box.IsEmpty() || box.ToLocalChecked()->IsUndefined()) {
        return Nan::Nothing<int64_t>();
    } else {
        return func(box.ToLocalChecked());
    }
}

Nan::MaybeLocal<v8::Array>
flatMap(Nan::MaybeLocal<v8::Value> box,
        std::function<Nan::MaybeLocal<v8::Array>(v8::Local<v8::Value>)> func) {
    if (box.IsEmpty()) {
        return Nan::MaybeLocal<v8::Array>();
    } else {
        return func(box.ToLocalChecked());
    }
}

namespace SourceKit {

    using namespace Nan;
    using namespace node;
    using namespace std;
    using namespace v8;

    static sourcekitd_uid_t KeyRequest =
        sourcekitd_uid_get_from_cstr("key.request");
    static sourcekitd_uid_t KeyCompilerArgs =
        sourcekitd_uid_get_from_cstr("key.compilerargs");
    static sourcekitd_uid_t KeyOffset =
        sourcekitd_uid_get_from_cstr("key.offset");
    static sourcekitd_uid_t KeySourceText =
        sourcekitd_uid_get_from_cstr("key.sourcetext");
    static sourcekitd_uid_t KeySourceFile =
        sourcekitd_uid_get_from_cstr("key.sourcefile");

    static sourcekitd_uid_t RequestCursorInfo =
        sourcekitd_uid_get_from_cstr("source.request.cursorinfo");
    static sourcekitd_uid_t RequestDocumentInfo =
        sourcekitd_uid_get_from_cstr("source.request.docinfo");

    NAN_METHOD(CursorInfo) {
        // This method never returns anything
        info.GetReturnValue().SetUndefined();

        sourcekitd_object_t Req =
            sourcekitd_request_dictionary_create(nullptr, nullptr, 0);
        sourcekitd_request_dictionary_set_uid(Req, KeyRequest,
                                              RequestCursorInfo);

        // TODO: Unsafe cast (e.g., ToLocalChecked())
        v8::Local<v8::Object> options =
            To<v8::Object>(info[0]).ToLocalChecked();

        // NOTE: This line could cause a runtime exception.
        // This is the same as a Swift IUO, e.g., v8::String!. Granted it is
        // unlikely to do so.
        v8::Local<v8::String> sourcefileKey =
            Nan::New<v8::String>("sourcefile").ToLocalChecked();
        Nan::Maybe<Nan::Utf8String*> sourceFile =
            map(Nan::Get(options, sourcefileKey), valueToCString);
        if (sourceFile.IsNothing()) {
            Nan::ThrowTypeError("Ensure the object has a property "
                                "\"sourcefile\" property and that property can "
                                "be coerced into a String.");
            return;
        } else {
            sourcekitd_request_dictionary_set_string(Req, KeySourceFile,
                                                     **sourceFile.FromJust());
        }
        // TODO: Pretty sure there is a memory leak here!

        // NOTE: This line could cause a runtime exception.
        // This is the same as a Swift IUO, e.g., v8::String!. Granted it is
        // unlikely to do so.
        v8::Local<v8::String> offsetKey =
            Nan::New<v8::String>("offset").ToLocalChecked();
        Nan::Maybe<int64_t> offset =
            flatMap(Nan::Get(options, offsetKey), valueToInt64);
        if (offset.IsNothing()) {
            Nan::ThrowTypeError("Ensure the object has a property \"offset\" "
                                "property and that property can be coerced "
                                "into a Number.");
            return;
        } else {
            sourcekitd_request_dictionary_set_int64(Req, KeyOffset,
                                                    offset.FromJust());
        }

        // NOTE: This line could cause a runtime exception.
        // This is the same as a Swift IUO, e.g., v8::String!. Granted it is
        // unlikely to do so.
        v8::Local<v8::String> compilerargsKey =
            Nan::New<v8::String>("compilerargs").ToLocalChecked();
        Nan::MaybeLocal<v8::Array> compilerargs =
            flatMap(Nan::Get(options, compilerargsKey), valueToArray);
        if (compilerargs.IsEmpty()) {
            Nan::ThrowTypeError("Ensure the object has a property "
                                "\"compilerargst\" property and that property "
                                "can be coerced into a Array of String "
                                "values.");
            return;
        } else {
            v8::Local<v8::Array> args = compilerargs.ToLocalChecked();
            sourcekitd_object_t Args =
                sourcekitd_request_array_create(nullptr, 0);
            for (uint32_t i = 0; i < args->Length(); i++) {
                v8::Local<v8::Value> arg = args->Get(i);
                sourcekitd_request_array_set_string(
                    Args, SOURCEKITD_ARRAY_APPEND, *Nan::Utf8String(arg));
            }
            sourcekitd_request_dictionary_set_value(Req, KeyCompilerArgs, Args);
            sourcekitd_request_release(Args);
        }

#ifdef DEBUG
        sourcekitd_request_description_dump(Req);
#endif

        // Get a reference to the 2nd argument, which should be a function
        Callback* callback = new Callback(info[1].As<Function>());

        auto work = new Worker(callback, Req);
        AsyncQueueWorker(work);
    }

    NAN_METHOD(DocumentInformation) {
        // This method never returns anything
        info.GetReturnValue().SetUndefined();

        sourcekitd_object_t Req =
            sourcekitd_request_dictionary_create(nullptr, nullptr, 0);
        sourcekitd_request_dictionary_set_uid(Req, KeyRequest,
                                              RequestDocumentInfo);

        // TODO: Unsafe cast (e.g., ToLocalChecked())
        v8::Local<v8::Object> options =
            To<v8::Object>(info[0]).ToLocalChecked();

        // NOTE: This line could cause a runtime exception.
        // This is the same as a Swift IUO, e.g., v8::String!. Granted it is
        // unlikely to do so.
        v8::Local<v8::String> sourcefileKey =
            Nan::New<v8::String>("sourcefile").ToLocalChecked();
        Nan::Maybe<Nan::Utf8String*> sourceFile =
            map(Nan::Get(options, sourcefileKey), valueToCString);
        if (sourceFile.IsNothing()) {
            Nan::ThrowTypeError("Ensure the object has a property "
                                "\"sourcefile\" property and that property can "
                                "be coerced into a String.");
            return;
        } else {
            sourcekitd_request_dictionary_set_string(Req, KeySourceFile,
                                                     **sourceFile.FromJust());
        }
        // TODO: Pretty sure there is a memory leak here!

        // NOTE: This line could cause a runtime exception.
        // This is the same as a Swift IUO, e.g., v8::String!. Granted it is
        // unlikely to do so.
        v8::Local<v8::String> compilerargsKey =
            Nan::New<v8::String>("compilerargs").ToLocalChecked();
        Nan::MaybeLocal<v8::Array> compilerargs =
            flatMap(Nan::Get(options, compilerargsKey), valueToArray);
        if (compilerargs.IsEmpty()) {
            Nan::ThrowTypeError("Ensure the object has a property "
                                "\"compilerargst\" property and that property "
                                "can be coerced into a Array of String "
                                "values.");
            return;
        } else {
            v8::Local<v8::Array> args = compilerargs.ToLocalChecked();
            sourcekitd_object_t Args =
                sourcekitd_request_array_create(nullptr, 0);
            for (uint32_t i = 0; i < args->Length(); i++) {
                v8::Local<v8::Value> arg = args->Get(i);
                sourcekitd_request_array_set_string(
                    Args, SOURCEKITD_ARRAY_APPEND, *Nan::Utf8String(arg));
            }
            sourcekitd_request_dictionary_set_value(Req, KeyCompilerArgs, Args);
            sourcekitd_request_release(Args);
        }

#ifdef DEBUG
        sourcekitd_request_description_dump(Req);
#endif

        // Get a reference to the 2nd argument, which should be a function
        Callback* callback = new Callback(info[1].As<Function>());

        auto work = new Worker(callback, Req);
        AsyncQueueWorker(work);
    }

    NAN_MODULE_INIT(Init) {
        // Initializes structures needed across the SourceKit API
        sourcekitd_initialize();
        // Deallocates structures needed across the SourceKit API
        // NOTE: `AtExit` accepts a pointer to a function that accepts a single
        // `void*`
        // `sourcekitd_shutdown` does not have this signature. Thus a C++
        // closure
        // was used to wrap the function.
        // I don't know enough about C++ optimization to know if there is a
        // "better" way
        // but it gets us moving down the road.
        AtExit([](void*) { sourcekitd_shutdown(); });

        // Register the methods
        Nan::Set(
            target, New<String>("cursorinfo").ToLocalChecked(),
            GetFunction(New<FunctionTemplate>(CursorInfo)).ToLocalChecked());
        Nan::Set(target, New<String>("documentinfo").ToLocalChecked(),
                 GetFunction(New<FunctionTemplate>(DocumentInformation))
                     .ToLocalChecked());
    }

    NODE_MODULE(addon, Init)

} // namespace node_sourcekit
