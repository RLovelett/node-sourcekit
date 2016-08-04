#include "SourceKitWorker.h"

namespace SourceKit {

    Worker::Worker(Nan::Callback *callback, sourcekitd_object_t request)
        : AsyncWorker(callback), Request(request), buffer(nullptr) {};

    Worker::~Worker() {
        sourcekitd_request_release(Request);
        free(buffer);
    };

    void Worker::Execute() {
        sourcekitd_response_t Resp = sourcekitd_send_request_sync(Request);
#ifdef DEBUG
        sourcekitd_response_description_dump(Resp);
#endif
        if (sourcekitd_response_is_error(Resp)) {
            // TODO: Figure out what to do
            sourcekitd_response_description_dump(Resp);
        } else {
            sourcekitd_variant_t Info = sourcekitd_response_get_value(Resp);
            buffer = sourcekitd_variant_json_description_copy(Info);
        }

        sourcekitd_response_dispose(Resp);
    };

    void Worker::HandleOKCallback() {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();

        auto string = Nan::New<v8::String>(buffer).ToLocalChecked();
        v8::MaybeLocal<v8::Value> json = v8::JSON::Parse(isolate, string);

        if (json.IsEmpty()) {
            // TODO: Figure out what to do
        } else {
            auto thing = json.ToLocalChecked();
            callback->Call(1, &thing);
        }
    };

}