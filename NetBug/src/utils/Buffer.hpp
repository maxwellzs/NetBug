//
//  Buffer.hpp
//  NetBug
//
//  Created by maxwells Turring on 2021/6/18.
//

#ifndef Buffer_hpp
#define Buffer_hpp

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace NetBug {

const size_t DEFAULT_BUFFER_SIZE = 1024;

class BaseBuffer {
protected:
    // were to stay the data
    const void * buffer_start;
    // buffer size
    size_t size;
    size_t index = 0;
    /**
     resize method
     */
    virtual void resizeIfNeed(const size_t inSize) final;
    /**
     get byte count available for current
     operation
     */
    virtual inline size_t getLimit() final {
        return this->size - this->index;
    }
public:
    BaseBuffer();
    BaseBuffer(const size_t size);
    virtual ~BaseBuffer();
    /**
     get the count of the data in all
     */
    virtual inline size_t getSize() final {
        return this->size;
    }
    /**
     reuse the buffer,
     change the write pos to 0
     */
    virtual inline void flip() final {
        this->index = 0;
    }
    /**
     return the pointer in the given type
     */
    virtual inline void * getPointer() final {
        return (const_cast<void *>(this->buffer_start));
    }
    /**
     return the writen bytes
     */
    virtual inline size_t getAvailable() final {
        return this->index;
    }
    /**
     base function for putting data
     */
    virtual void pushData(const void * src,const size_t src_size) = 0;
    /**
     wipe all data
     */
    virtual inline void clearData() final {
        memset(const_cast<void *>(this->buffer_start), 0x0, size);
    }
};

/**
 Buffer NOT synchronized
 */
class UnsafeBuffer : public BaseBuffer {
public:
    virtual void pushData(const void * src,const size_t src_size) override;
    UnsafeBuffer() : BaseBuffer() {
        this->size = DEFAULT_BUFFER_SIZE;
    }
    UnsafeBuffer(const size_t size) : BaseBuffer(size) {
        this->size = size;
    }
};

}

#endif /* Buffer_hpp */
