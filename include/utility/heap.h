// EPOS Heap Utility Declarations

#ifndef __heap_h
#define __heap_h

#include <utility/debug.h>
#include <utility/list.h>
#include <utility/spin.h>

__BEGIN_UTIL

// Heap
class Heap: private Grouping_List<char>
{
public:
    using Grouping_List<char>::empty;
    using Grouping_List<char>::size;

    Heap() {
        db<Init, Heaps>(TRC) << "Heap() => " << this << endl;
    }

    Heap(void * addr, unsigned int bytes) {
        db<Init, Heaps>(TRC) << "Heap(addr=" << addr << ",bytes=" << bytes << ") => " << this << endl;

        free(addr, bytes);
    }

    void * alloc(unsigned int bytes) {
        db<Heaps>(TRC) << "Heap::alloc(this=" << this << ",bytes=" << bytes;

        if(!bytes)
            return 0;

        if(!Traits<CPU>::unaligned_memory_access)
            while((bytes % sizeof(void *)))
                ++bytes;

        bytes += sizeof(int);         // add room for size
        if(bytes < sizeof(Element))
            bytes = sizeof(Element);

        Element * e = search_decrementing(bytes);
        if(!e) {
            out_of_memory();
            return 0;
        }

        int * addr = reinterpret_cast<int *>(e->object() + e->size());

        *addr++ = bytes;

        db<Heaps>(TRC) << ") => " << reinterpret_cast<void *>(addr) << endl;

        return addr;
    }

    void free(void * ptr, unsigned int bytes) {
        db<Heaps>(TRC) << "Heap::free(this=" << this << ",ptr=" << ptr << ",bytes=" << bytes << ")" << endl;

        if(ptr && (bytes >= sizeof(Element))) {
            Element * e = new (ptr) Element(reinterpret_cast<char *>(ptr), bytes);
            Element * m1, * m2;
            insert_merging(e, &m1, &m2);
        }
    }

    void free(void * ptr) {
        int * addr = reinterpret_cast<int *>(ptr);
        unsigned int bytes = *--addr;
        free(addr, bytes);
    }

private:
    void out_of_memory();
};






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Heap_buddy: private Grouping_List<char>
{
public:
    using Grouping_List<char>::empty;
    using Grouping_List<char>::size;

    Heap_buddy() {
        db<Init, Heaps>(TRC) << "Heap_buddy() => " << this << endl;
    }

    Heap_buddy(void * addr, unsigned int bytes) {
        db<Init, Heaps>(TRC) << "Heap_buddy(addr=" << addr << ",bytes=" << bytes << ") => " << this << endl;

        free(addr, bytes);
    }

    bool potencia_de_dois(unsigned int n){
    while(n%2 == 0){
        n = n/2;
    }
    if(n == 1){
        return true;
    }
    return false;
    }

    void * alloc(unsigned int bytes) {
        db<Heaps>(TRC) << "Heap_buddy::alloc(this=" << this << ",bytes=" << bytes;

        if(!bytes)
            return 0;

/*        if(!Traits<CPU>::unaligned_memory_access)
            while((bytes % sizeof(void *)))
                ++bytes;

        bytes += sizeof(int);         // add room for size
        if(bytes < sizeof(Element))
            bytes = sizeof(Element);
*/


        /*alteração*/
        unsigned int menor_bloco = sizeof(Element);
        while(!potencia_de_dois(menor_bloco)){
            menor_bloco++;
        }

        bytes += sizeof(int);

        while(!potencia_de_dois(bytes)){
            bytes++;
        }

        if(bytes < menor_bloco){
            bytes = menor_bloco;
        }
        /*fim alteração*/

        Element * e = search_decrementing_buddy(bytes);
        if(!e) {
            out_of_memory();
            return 0;
        }

        int * addr = reinterpret_cast<int *>(e->object() + e->size());

        *addr++ = bytes;

        db<Heaps>(TRC) << ") => " << reinterpret_cast<void *>(addr) << endl;

        return addr;
    }

    void free(void * ptr, unsigned int bytes) {
        db<Heaps>(TRC) << "Heap_buddy::free(this=" << this << ",ptr=" << ptr << ",bytes=" << bytes << ")" << endl;

        if(ptr && (bytes >= sizeof(Element))) {
            Element * e = new (ptr) Element(reinterpret_cast<char *>(ptr), bytes);
            Element * m1, * m2;
            insert_merging(e, &m1, &m2);
        }
    }

    void free(void * ptr) {
        int * addr = reinterpret_cast<int *>(ptr);
        unsigned int bytes = *--addr;
        free(addr, bytes);
    }

private:
    void out_of_memory();
};


__END_UTIL

#endif
