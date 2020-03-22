#include <unistd.h>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

using namespace std;

queue<int> q;

pthread_mutex_t mutex;
pthread_t threadReducer;
pthread_t threadConsumer;  

void * threadReducerFunc(void *arg) {
    int count = 0;

    while (true) {
        pthread_mutex_lock(&mutex);

        q.push(count);

        printf("Поток отправил в очередь сигнал: %i\n", count);

        count++;

        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}

void * threadConsumerFunc(void *arg) {
    while (true) {
        pthread_mutex_lock(&mutex);

        if (!q.empty()) {
            int signal = q.front();

            q.pop();

            printf("Поток получил из очереди сигнал: %i\n", signal);
        }   

        pthread_mutex_unlock(&mutex);     
    }
}

int main() 
{
    int threadRedicerId = 1;
    int threadConsumerId = 2;  
    
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&threadReducer, NULL, threadReducerFunc, &threadRedicerId);
    pthread_create(&threadConsumer, NULL, threadConsumerFunc, &threadConsumerId);

    pthread_join(threadReducer, NULL);
    pthread_join(threadConsumerId, NULL);

    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}