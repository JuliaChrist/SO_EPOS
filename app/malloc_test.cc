// EPOS Memory Allocation Utility Test Program

#include <utility/ostream.h>
#include <utility/string.h>
#include <utility/malloc.h>
#include <utility/list.h>
#include <utility/math.h>
#include <chronometer.h>

#define NUM_TESTES 200

using namespace EPOS;

float sqrt(float num){
    int n;
    float aux = num;

    for (n = 0; n < 100; n++)
          aux = aux/2 + num/(2*aux);

    return(aux);
}

float media(unsigned int * vetor, int size){
    float soma = 0;
    for(int i = 0; i < size; i++){
        soma += vetor[i];
    }
    return(soma/(float)size);
}

float desvio(unsigned int * vetor, int size){
    float p = 0.0;
    float m = media(vetor, size);

    for(int i = 0; i < size; i++){
        p += (((float)vetor[i] - m)*((float)vetor[i] - m));
    }
    return(sqrt(p/(float)(size-1)));
}

int main()
{
    OStream cout;
    Chronometer cronometro;
    Heap_Test heap;
    int count, i;
    char *vet[255];

    unsigned int test1[NUM_TESTES], test2[NUM_TESTES], test3[NUM_TESTES];

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
    for(count = 0; count < NUM_TESTES; count++){

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
    for(count = 0; count < NUM_TESTES; count++){

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
    for(count = 0; count < NUM_TESTES; count++){
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

    cout << "\nMédia de tempo[Teste 1]: " << media(test1, NUM_TESTES) << endl;
    cout << "Desvio Padrão [Teste 1]: " << desvio(test1, NUM_TESTES) << endl;
    cout << "\nMédia de tempo[Teste 2]: " << media(test2, NUM_TESTES) << endl;
    cout << "Desvio Padrão [Teste 2]: " << desvio(test2, NUM_TESTES) << endl;
    cout << "\nMédia de tempo[Teste 3]: " << media(test3, NUM_TESTES) << endl;
    cout << "Desvio Padrão [Teste 3]: " << desvio(test3, NUM_TESTES) << endl;
    cout << endl << endl;

    return 0;
}
