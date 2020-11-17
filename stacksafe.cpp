#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <stack>
#include <mutex>
#include <optional>
#include <vector>

struct safestack {

    private:
        std::stack<int> stack;
        std::mutex m;

    public:

        void push_to_stack(int x) {
            std::unique_lock<std::mutex> lock(m);
            this->stack.push(x);
        }

        int pop_from_stack() {
            std::unique_lock<std::mutex> lock(m);
            int x;
            if (this->stack.empty() == false) {
                x = this->stack.top();
            } else {
                return NULL;
            }
            this->stack.pop();
            return x;
        }
};

int main() {
    safestack stack;
    for (int i=0; i < 40; i++) {
        stack.push_to_stack(i);
    }

    std::vector<std::thread> threads;

    for (int x=0; x<20; x++) {
        std::thread t;
        threads.push_back(std::move(t));
    }
    for (auto &it : threads) {
        it = std::thread([s = &stack](){s->pop_from_stack();});
    }
    for (auto &it : threads) {
        it.join();
    }

    int val;
    while ((val = stack.pop_from_stack()) != NULL) {
        printf("%d\n",val);
    }

}