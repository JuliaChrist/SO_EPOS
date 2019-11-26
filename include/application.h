// EPOS Global Application Abstraction Declarations

#ifndef __application_h
#define __application_h

#include <utility/ostream.h>
#include <utility/heap.h>

extern "C"
{
    void * malloc(size_t);
    void free(void *);
}

__BEGIN_SYS


class Application
{
    friend class Init_Application;
    friend class Heap_Test;
    friend void * ::malloc(size_t);
    friend void ::free(void *);

private:
    static void init();

private:
    static char _preheap[sizeof(Heap_Application)];
    //static char _preheap[sizeof(Heap)];    
    static Heap_Application * _heap;
    //static Heap * _heap;
};

/*classe utilizada para os testes de desempenho da alocação da memória*/
class Heap_Test
{
public:
	Heap_Application *heap_test;
	//Heap *heap_test;
	Heap_Test(){
		heap_test = Application::_heap;
	}	
};

__END_SYS

#include <utility/malloc.h>

#endif
