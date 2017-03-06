/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MemoryLeak.h
 * Author: ethan
 *
 * Created on March 6, 2017, 9:24 AM
 */

#ifndef MEMORYLEAK_H
#define MEMORYLEAK_H
#include <memory>
#include <exception>

namespace MemoryLeaking {

    const int HeadID = 0x77ab55aa;
    struct MemoryNode {
        int _headID;
        MemoryNode * _pre;
        MemoryNode * _next;
        char * _fileName;
        size_t _lineNumber;
        size_t _size;
        
        MemoryNode() :_headID(HeadID), _pre(nullptr), _next(nullptr), _fileName(nullptr), _lineNumber(0),_size(0) {

        }

        void SetFileNameAndLine(const char* nsName, int nLine) {
           
            _fileName = const_cast<char*> (nsName);
            _lineNumber = nLine;
           
        }
        bool IsValid(){
            return _headID == HeadID;
        }
    };

    class MemoryLinkList {
        static MemoryNode * mHead;

    public:
        static void * Allocte(size_t size) {
            void * p = ::malloc(sizeof (MemoryNode) + size);
            if (p == nullptr)
                return nullptr;
          
            //Insert this node to the head of list;
            MemoryNode * lp = reinterpret_cast<MemoryNode*> (p);
            lp->_headID = HeadID;
            lp->_pre = nullptr;
            lp->_next = mHead;
            if(mHead)
                mHead->_pre = lp;
            
            lp->_size = size;
            mHead = lp;

            return (char*)p + sizeof(MemoryNode);

        }

        static void free(void * p) {
            //Remove from the list
            if (p == nullptr)
                return;
           
            MemoryNode * lp = reinterpret_cast<MemoryNode*> ((char*)p - sizeof(MemoryNode));
            if(!lp->IsValid())
                return;
           
            MemoryNode * lpPre = lp->_pre;
            if (lpPre == nullptr) {
                if (lp->_next) {
                    lp->_next->_pre = nullptr;
                    mHead = lp->_next;
                } else {
                    mHead = nullptr;
                }
            } else {
                if (lp->_next) {
                    lp->_next->_pre = lpPre;
                    lpPre->_next = lp->_next;
                } else {
                    lpPre->_next = nullptr;
                }
            }
            lp = nullptr;
            free(lp);
        }

        static void SetFileAndLineOfNode(void * p, const char* nsName,  int nLine  ) {
            if (p == nullptr)
                return;
            MemoryNode * lp = reinterpret_cast<MemoryNode*> ((char*)p - sizeof(MemoryNode));
            if(!lp->IsValid())
                return;
            lp->SetFileNameAndLine(nsName, nLine);
        }
        
        static void Dump(){
             MemoryNode * p = mHead;
            for(;p != nullptr; p = p->_next){
                 void * pReal = (void*)((char*)p + sizeof(MemoryNode));
                 std::cout << "memory leaking at " <<
                " Address " << pReal  << " Size " << p->_size << " File " << (p->_fileName == nullptr ? "" : p->_fileName) << " Line " << p->_lineNumber << std::endl;
 
            }
        }
    };
    MemoryNode * MemoryLinkList::mHead = nullptr;




}
#endif /* MEMORYLEAK_H */

