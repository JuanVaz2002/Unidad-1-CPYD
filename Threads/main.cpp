#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>


#define THREADS 10

using namespace std;

class RandomAddition {
public:
    int id;
    RandomAddition(int id) : id(id), sum(0) {}
    void operator()() {
        random_device rd;
        mt19937 gen(rd());
        
        uniform_int_distribution<> dist(1, 1000);
        
        for (int i = 0; i < MAX; i++) {
            sum += dist(gen);
        }

        lock_guard<mutex> guard(output_mutex);
        cout << "Sum in thread " << id << ": " << sum << endl;
    }
    
    long getSum() const {
        return sum;
    }

private:
    long sum = 0;
    static const int MAX = 100;
    static mutex output_mutex;
};

mutex RandomAddition::output_mutex;

int main() {
    
    vector<thread> threads;
    vector<unique_ptr<RandomAddition>> workers;

    for (int i = 0; i < THREADS; i++) {
        workers.push_back(make_unique<RandomAddition>(i + 1));
        threads.emplace_back(ref(*workers.back()));
    }

    for (auto &t : threads) t.join();

    // Encontrar thread con la mayor suma
    std::sort(workers.begin(), workers.end(), [](const auto &w1, const auto &w2) {
        return w1->getSum() > w2->getSum();
    });
    
    int bestId = workers[0]->id;
    long bestSum = workers[0]->getSum();

    cout << "\nEl thread con mayor puntuacion es: " 
         << bestId << " con suma = " << bestSum << endl;

    return 0;
}
