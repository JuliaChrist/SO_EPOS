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

    unsigned int get_grouped_size(void){
        return(grouped_size());
    }

    void * alloc(unsigned int bytes) {
        db<Heaps>(TRC) << "Heap::alloc(this=" << this << ",bytes=" << bytes;

        if(!bytes)
            return 0;

        //kout << "\n---------------------------------------------------------" << endl;
        //kout << "ALLOC " << bytes << "+sizeof(int) BYTES" << endl;

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

        //kout << "END ALLOC -> " << addr << endl;
        //kout << "---------------------------------------------------------" << endl;

        *addr++ = bytes;

        db<Heaps>(TRC) << ") => " << reinterpret_cast<void *>(addr) << endl;

  

        return addr;
    }

    void free(void * ptr, unsigned int bytes) {
        db<Heaps>(TRC) << "Heap::free(this=" << this << ",ptr=" << ptr << ",bytes=" << bytes << ")" << endl;
        //kout << "\n---------------------------------------------------------" << endl;
        //kout << "FREE " << ptr << endl;

        if(ptr && (bytes >= sizeof(Element))) {
            Element * e = new (ptr) Element(reinterpret_cast<char *>(ptr), bytes);
            Element * m1, * m2;
            insert_merging(e, &m1, &m2);
        }

        //kout << "END FREE" << endl;
        //kout << "---------------------------------------------------------" << endl;  
    }

    void free(void * ptr) {
        int * addr = reinterpret_cast<int *>(ptr);
        unsigned int bytes = *--addr;
        free(addr, bytes);
    }

private:
    void out_of_memory();
};

class Heap_Application: private Grouping_List<char>
{
public:
    using Grouping_List<char>::empty;
    using Grouping_List<char>::size;

    /*função usada nos testes de desempenho. Necessária pois a classe herda Grouping_List de forma privada*/
    unsigned int get_grouped_size(void){
        return(grouped_size());
    }

    Heap_Application() {
        db<Init, Heaps>(TRC) << "Heap_Application() => " << this << endl;
    }

    Heap_Application(void * addr, unsigned int bytes) {
        db<Init, Heaps>(TRC) << "Heap_Application(addr=" << addr << ",bytes=" << bytes << ") => " << this << endl;

        free(addr, bytes);
    }

    void * alloc(unsigned int bytes) {
        db<Heaps>(TRC) << "Heap_buddy::alloc(this=" << this << ",bytes=" << bytes;
              
        if(!bytes)
            return 0;

        kout << "\n---------------------------------------------------------" << endl;
        kout << "ALLOC " << bytes << "+sizeof(int) BYTES" << endl;

        if(!Traits<CPU>::unaligned_memory_access)
            while((bytes % sizeof(void *)))
                ++bytes;

        bytes += sizeof(int);         // add room for size
        if(bytes < sizeof(Element))
            bytes = sizeof(Element);

        Element * e = search_decrementing_buddy(bytes);

        if(!e) {
            out_of_memory();
            return 0;
        }

        int * addr = reinterpret_cast<int *>(e->object() + e->size());

        kout << "END ALLOC -> " << addr << endl;
        kout << "---------------------------------------------------------" << endl; 
        *addr++ = bytes;

        db<Heaps>(TRC) << ") => " << reinterpret_cast<void *>(addr) << endl;
 
        return addr;
        
    }

    void free(void * ptr, unsigned int bytes) {
        db<Heaps>(TRC) << "Heap_buddy::free(this=" << this << ",ptr=" << ptr << ",bytes=" << bytes << ")" << endl;
        kout << "\n---------------------------------------------------------" << endl;
        kout << "FREE " << ptr << endl;

        while(!potencia_de_dois(bytes)){
            bytes++;
        }
        if(ptr && (bytes >= sizeof(Element))) {
            Element * e = new (ptr) Element(reinterpret_cast<char *>(ptr), bytes);
            Element * m1, * m2;
            insert_merging_buddy(e, &m1, &m2);
        }

        kout << "END FREE" << endl;
        kout << "---------------------------------------------------------" << endl;  
    }

    void free(void * ptr) {
        int * addr = reinterpret_cast<int *>(ptr);
        unsigned int bytes = *--addr;
        while(!potencia_de_dois(bytes)){
            bytes++;
        }
        free(addr, bytes);
    }

private:
    void out_of_memory();
};


__END_UTIL

#endif