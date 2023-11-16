#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>

using std::cout;
using std::endl;
using std::string;
using std::vector;

std::mutex watchReception;
std::mutex watchKitchen;

template<typename T>
T popFront (std::vector<T> &list) {
    T temp = list.front();
    list.erase(list.begin());
    return temp;
}

int getRandomIntInRange(int from, int to) {
    return (from + std::rand() % (to - from + 1)); // NOLINT(cert-msc50-cpp)
}

// Последовательно, по истечении интервалов времени, заполняем лист онлайн заказов (orderList).
void receptionThread(vector<string> &orderList) {
    // Набираем десять заказов:
    const int MAX_ORDER_COUNT{10};
    int orderCount{0};
    vector<string> dishes = { "Pizza", "Soup", "Steak", "Salad", "Sushi" };

    while (orderCount < MAX_ORDER_COUNT) {
        int interval = getRandomIntInRange(1, 2);
        std::this_thread::sleep_for(std::chrono::seconds(interval));

        watchReception.lock();
        string order = dishes[getRandomIntInRange(0, static_cast<int> (dishes.size() - 1))];
        cout << "Online order [#" << orderCount << "] is made for " << order << endl;
        orderList.emplace_back(order);
        watchReception.unlock();
        ++orderCount;
    }
}

void kitchenThread(vector<string> &orderList, vector<string> &cookedList) {
    // Набираем десять заказов:
    const int MAX_ORDER_COUNT{10};
    int orderCount{0};

    while (orderCount < MAX_ORDER_COUNT) {
        if (!orderList.empty()) {
            watchKitchen.lock();
            string dish = popFront(orderList);
            cout << "dish to prepare:: " << dish << endl;

            int interval = getRandomIntInRange(1, 3);
            std::this_thread::sleep_for(std::chrono::seconds(interval));

            cookedList.emplace_back(dish);
            cout << "Cooked [#" << orderCount << "] is made for " << orderList.back() << endl;
            watchKitchen.unlock();
            ++orderCount;
        }
        else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

int main() {
    // Список онлайн заказов
    vector<string> orderList;
    // Список приготовленных блюд
    vector<string> cookedList;

    std::srand(std::time(nullptr)); // NOLINT(*-msc51-cpp)

    // --->

    std::thread runReception(receptionThread, std::ref(orderList));
    std::thread runKitchen(kitchenThread, std::ref(orderList), std::ref(cookedList));

    if (runReception.joinable()) { runReception.join(); }
    if (runKitchen.joinable()) { runKitchen.join(); }

    cout << "-- DISHES --" << endl;
    for (const auto &dish : cookedList) { cout << dish << endl; }

    return 0;
}
