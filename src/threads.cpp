#include <mutex>
#include "threads.h"

std::mutex watchReception;
std::mutex watchKitchen;
std::mutex watchDelivery;

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

// Последовательно, по истечении интервалов времени,
// перемещаем данные из заказов (orderList) в список готовых блюд (cookedList)
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

// Последовательно, по истечении интервалов времени,
// перемещаем данные из списка готовых блюд (cookedList) в список доставленных (completedList)
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