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
std::mutex watchDelivery;

const int MAX_ORDER_COUNT{10};
const int INCREASE{1};

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
    int count{0};
    vector<string> dishes = { "Pizza", "Soup", "Steak", "Salad", "Sushi" };

    while (count < MAX_ORDER_COUNT) {
        int interval = getRandomIntInRange(1 * INCREASE, 2 * INCREASE);
        std::this_thread::sleep_for(std::chrono::seconds(interval));

        watchReception.lock();
        string order = dishes[getRandomIntInRange(0, static_cast<int> (dishes.size() - 1))];
        cout << "RECEPTION! Online order [#" << count << "] is made for " << order << endl;
        orderList.emplace_back(order);
        watchReception.unlock();
        ++count;
    }
}

void kitchenThread(vector<string> &orderList, vector<string> &cookedList) {
    int count{0};

    while (count < MAX_ORDER_COUNT) {
        if (!orderList.empty()) {
            watchKitchen.lock();
            string dish = popFront(orderList);
            cout << "KITCHEN!   Dish [#" << count << "] " << dish << " has started to cook" << endl;

            int interval = getRandomIntInRange(1 * INCREASE, 3 * INCREASE);
            std::this_thread::sleep_for(std::chrono::seconds(interval));

            cookedList.emplace_back(dish);
            cout << "KITCHEN!   Dish [#" << count << "] " << dish << " has finished to cooking" << endl;
            watchKitchen.unlock();
            ++count;
        }
        else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void deliveryThread(vector<string> &cookedList, vector<string> &completedList) {
    int count{0};

    while (count < MAX_ORDER_COUNT) {
        if (!cookedList.empty()) {
            watchDelivery.lock();
            string cooked = popFront(cookedList);
            cout << "DELIVERY!  Courier took the dish [#" << count << "] " << cooked << endl;

            completedList.emplace_back(cooked);
            watchDelivery.unlock();
            ++count;

            std::this_thread::sleep_for(std::chrono::seconds(6 * INCREASE));
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
    // Список выполненных заказов
    vector<string> completedList;

    std::srand(std::time(nullptr)); // NOLINT(*-msc51-cpp)

    // --->

    std::thread runReception(receptionThread, std::ref(orderList));
    std::thread runKitchen(kitchenThread, std::ref(orderList), std::ref(cookedList));
    std::thread runDelivery(deliveryThread, std::ref(cookedList), std::ref(completedList));

    if (runReception.joinable()) { runReception.join(); }
    if (runKitchen.joinable()) { runKitchen.join(); }
    if (runDelivery.joinable()) { runDelivery.join(); }

    cout << "-- List of completed orders --" << endl;
    for (const auto &dish : completedList) { cout << dish << endl; }

    return 0;
}
