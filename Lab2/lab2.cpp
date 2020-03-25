#include <iostream>
#include <unistd.h>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>

using namespace std;

int count = 0;
queue<int> q;

mutex mtx;
condition_variable conditionVariable;

void consumerFunc(){
    unique_lock<mutex> lck(mtx);
    conditionVariable.wait(lck);

    int signal = q.front();
    q.pop();

    cout << "Consumer получил из очереди сигнал: " << signal << endl;
}

void reducerFunc() {
    cout << "Reducer отправил в очередь сигнал: " << count << endl;

    q.push(count);

    count++;

    conditionVariable.notify_one();

    sleep(1);
}

int main(){
    while (true)
    {
        thread consumerThread(consumerFunc);
        thread reducerThread(reducerFunc);

        consumerThread.join();
        reducerThread.join(); 
    }



}