//
//  Auxiliary.hpp
//  NetBug
//
//  Created by maxwells Turring on 2021/6/18.
//

#ifndef Auxiliary_h
#define Auxiliary_h

namespace NetBug {

template<typename _target>
inline void swapMemory(_target * t1,_target * t2) {
    typedef union {
        _target t;
        char each_w[sizeof(_target)];
    } _temp;
    _temp *_t1 = reinterpret_cast<_temp *>(t1);
    _temp *_t2 = reinterpret_cast<_temp *>(t2);
    for(size_t s = 0 ; s < sizeof(_target) ; s ++) {
        (*_t1).each_w[s] ^= (*_t2).each_w[s];
        (*_t2).each_w[s] ^= (*_t1).each_w[s];
        (*_t1).each_w[s] ^= (*_t2).each_w[s];
    }
}

};

#endif /* Auxiliary_h */
