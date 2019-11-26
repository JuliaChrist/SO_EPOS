// EPOS Memory Allocation Utility Test Program

#include <utility/ostream.h>
#include <utility/string.h>
#include <utility/malloc.h>
#include <utility/list.h>
#include <chronometer.h>

using namespace EPOS;

float media(unsigned int * vetor, int size){
    float soma = 0;
    int i;
    for(i = 0; i < size; i++){
        soma += vetor[i];
    }
    return(soma/(float)size);
}

int main()
{
    OStream cout;
    Chronometer cronometro;
    Heap_Test heap;
    int count, i;
    char *vet[255];

    unsigned int test1[100], test2[100], test3[100];

    cout << "\nMemory allocation test" << endl;


    /*char *A = new char[60];
        cout << "\nThe heap has now " << heap.heap_test->size() << " elements that group "
         << heap.heap_test->get_grouped_size() << " bytes in total" << endl;  
    char *B = new char[124];
        cout << "\nThe heap has now " << heap.heap_test->size() << " elements that group "
         << heap.heap_test->get_grouped_size() << " bytes in total" << endl;  
    char *C = new char[60];
        cout << "\nThe heap has now " << heap.heap_test->size() << " elements that group "
         << heap.heap_test->get_grouped_size() << " bytes in total" << endl;  
    char *D = new char[124];
        cout << "\nThe heap has now " << heap.heap_test->size() << " elements that group "
         << heap.heap_test->get_grouped_size() << " bytes in total" << endl;  

    cout << "\nDELETENDO " << endl;
    delete C;
        cout << "\nThe heap has now " << heap.heap_test->size() << " elements that group "
         << heap.heap_test->get_grouped_size() << " bytes in total" << endl;  
    delete A;
        cout << "\nThe heap has now " << heap.heap_test->size() << " elements that group "
         << heap.heap_test->get_grouped_size() << " bytes in total" << endl;  
    delete B;
        cout << "\nThe heap has now " << heap.heap_test->size() << " elements that group "
         << heap.heap_test->get_grouped_size() << " bytes in total" << endl;  
    delete D;
        cout << "\nThe heap has now " << heap.heap_test->size() << " elements that group "
         << heap.heap_test->get_grouped_size() << " bytes in total" << endl; */ 
    

    /*Teste 1: alocação de espaços pequenos (bytes)*/
    for(count = 0; count < 100; count++){

        cronometro.start();

        for(i=0; i < 100; i++){
            vet[i] = new char[20];
        }

        for(i = 0; i < 100; i++){
            delete (vet[i]);
        }

        cronometro.stop();
        test1[count] = cronometro.read();
        cronometro.reset();
    }



    /*Teste 2: Alocação de espaços grandes (kbytes)*/
    for(count = 0; count < 100; count++){

        cronometro.start();

        for(i=0; i < 100; i++){
            vet[i] = new char[64*1024];
        }   

        for(i = 0; i < 100; i++){
            delete (vet[i]);
        }

        cronometro.stop();
        test2[count] = cronometro.read();
        cronometro.reset();
    }
 

    /*Teste 3: Alocação de espaços pequenos e grandes (faixa de bytes a kbytes)*/
    for(count = 0; count < 100; count++){
        int aux;
        cronometro.start();

        for(aux=8, i=0; aux < 64*1024, i < 20; aux += (64*1024 - 8)/20, i++){
            vet[i] = new char[aux];
        }

        for(i = 0; i < 20; i++){
            delete (vet[i]);
        }

        cronometro.stop();
        test3[count] = cronometro.read();
        cronometro.reset();
    }

    cout << "Média de tempo[Teste 1]: " << media(test1, 100) << endl;
    cout << "Média de tempo[Teste 2]: " << media(test2, 100) << endl;
    cout << "Média de tempo[Teste 3]: " << media(test3, 100) << endl;
    cout << endl;

    
    return 0;
}
