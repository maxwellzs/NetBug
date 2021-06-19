//
//  Buffer.cpp
//  NetBug
//
//  Created by maxwells Turring on 2021/6/18.
//

#include "Buffer.hpp"

using namespace std;
using namespace NetBug;

BaseBuffer::BaseBuffer() : BaseBuffer(DEFAULT_BUFFER_SIZE) {
    // no given size, use default
}

BaseBuffer::BaseBuffer(const size_t size) :
buffer_start(malloc(size)) {
    // initialize values
    this->size = size;
}

BaseBuffer::~BaseBuffer() {
    free(const_cast<void *>(buffer_start));
}

void BaseBuffer::resizeIfNeed(const size_t inSize) {
    if(getLimit() <= inSize) {
        size_t finalSize = this->size;
        do {
            // multiply two times, until enough
            // memory space is prepared
            finalSize += 1024;
        } while(finalSize - index <= inSize);
        // realloc memory
        this->buffer_start = realloc(const_cast<void *>(this->buffer_start), finalSize);
        this->size = finalSize;
    }
}

void UnsafeBuffer::pushData(const void *src, const size_t src_size) {
    // check first
    resizeIfNeed(src_size);
    // write in buffer
    memcpy(((char *)buffer_start) + index, src, src_size);
    // refresh memory
    index += src_size;
}
