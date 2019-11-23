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
class Heap_buddy: private Grouping_List_heap<char>
{
public:
    using Grouping_List_heap<char>::empty;
    using Grouping_List_heap<char>::size;

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
        kout << "\n-----ALOCANDO----- " << bytes << endl;
        if(!bytes)
            return 0;

        if(!Traits<CPU>::unaligned_memory_access)
            while((bytes % sizeof(void *)))
                ++bytes;

        bytes += sizeof(int);         // add room for size
        if(bytes < sizeof(Element))
            bytes = sizeof(Element);



        /*alteração
        unsigned int menor_bloco = sizeof(Element);
        while(!potencia_de_dois(menor_bloco)){
            define o limite inferior: menor bloco de memoria possivel
            menor_bloco++;
        }

        kout << "Numero de bytes solicitados: " << bytes << "+4(sizeof(int))" << endl;

        bytes += sizeof(int); //soma 4 bytes (sizeof(int)) para

        while(!potencia_de_dois(bytes)){
            bytes++;
        }

        if(bytes < menor_bloco){
            bytes = menor_bloco;
        }                        
       fim alteração*/
        //bytes += sizeof(int);

        kout << "chamando search_decrementing_buddy" << endl;
        Element * e = search_decrementing_buddy(bytes);


        if(!e) {
            out_of_memory();
            return 0;
        }

        kout << "Voltando para a heap. Temos o endereço " << e << " no tamanho de " << e->size() << " (a memoria passada para o usuário será a que vem depois deste bloco)" << endl;
        kout << "Addr: fazendo casting para int, com o offset para mandar a memória ao usuário" << endl;
        int * addr = reinterpret_cast<int *>(e->object() + e->size());

        kout << "Endereço inicial da memoria alocada (com 4 bytes para armazenar o tamanho): " << addr << endl;
        *addr++ = bytes;

        db<Heaps>(TRC) << ") => " << reinterpret_cast<void *>(addr) << endl;

        kout << "Endereço final retornado para a alocação: " << addr << " com tamanho de " << bytes-sizeof(int) << "bytes." << endl;
        kout << "-----FIM ALOCAÇÃO-----" << endl;
        return addr;
        
    }

    void free(void * ptr, unsigned int bytes) {
        db<Heaps>(TRC) << "Heap_buddy::free(this=" << this << ",ptr=" << ptr << ",bytes=" << bytes << ")" << endl;
        kout <<  "\n-----DANDO FREE EM " << ptr << "-----" << endl;
        if(ptr && (bytes >= sizeof(Element))) {
            Element * e = new (ptr) Element(reinterpret_cast<char *>(ptr), bytes);
            Element * m1, * m2;
            insert_merging(e, &m1, &m2);
        }
        kout << "-----FIM DO FREE-----" << endl;
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
