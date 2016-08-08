#ifndef SOURCEKIT_WORKER_H
#define SOURCEKIT_WORKER_H

#include <nan.h>
#include "sourcekitd/sourcekitd.h"

namespace SourceKit {

    class Worker : public Nan::AsyncWorker {
    public:
        Worker(Nan::Callback *callback, sourcekitd_object_t request);
        ~Worker();
        void Execute();
        void HandleOKCallback();
    private:
        sourcekitd_object_t Request;
        char* buffer;
    };

}

#endif