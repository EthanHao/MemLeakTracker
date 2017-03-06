/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MemHearder.h
 * Author: ethan
 *
 * Created on March 6, 2017, 10:05 AM
 */

#ifndef MEMHEARDER_H
#define MEMHEARDER_H

#include "MemoryLeak.h"




/////////////////////////////////////

class HeapStamp {
public:

    HeapStamp(const char* nsFile, int nLine) : msPath(nsFile), mLine(nLine) {
    }
public:
    const char * const msPath;
    const int mLine;
};

template <class T> inline T *operator+(const HeapStamp &stamp, T *p) {
    // HeapStamp(p, stamp, typeid(T).name());
    std::cout << "Allocated a block of memory of " << typeid (T).name() <<
            " Address " << p << " Size " << sizeof (T) << " File " << stamp.msPath << " Line " << stamp.mLine << std::endl;

    MemoryLeaking::MemoryLinkList::SetFileAndLineOfNode(p, stamp.msPath, stamp.mLine);
    return p;
}


void *operator new(size_t size) throw (std::bad_alloc&){
    //std::cout << "new" << std::endl;
    void *p = MemoryLeaking::MemoryLinkList::Allocte(size);
    if (p == nullptr) throw std::bad_alloc();
    return p;
}

/* ---------------------------------------- operator delete */

void operator delete(void *p) {
    MemoryLeaking::MemoryLinkList::free(p);
}

/* ---------------------------------------- operator new[] */

void *operator new[](size_t size) throw (std::bad_alloc&) {
    //std::cout << "new[]" << std::endl;
    void *p = MemoryLeaking::MemoryLinkList::Allocte(size);
    if (p == nullptr) throw std::bad_alloc();
    return p;
}

/* ---------------------------------------- operator delete[] */

void operator delete[](void *p) {
    MemoryLeaking::MemoryLinkList::free(p);
}



#define new HeapStamp(__FILE__, __LINE__) + new

#endif /* MEMHEARDER_H */

