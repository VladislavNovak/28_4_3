#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include "threads.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main() {
    // Список онлайн заказов
    vector<string> orderList;
    // Список приготовленных блюд
    vector<string> cookedList;
    // Список выполненных заказов
    vector<string> completedList;

    std::srand(std::time(nullptr)); // NOLINT(*-msc51-cpp)

    // START --->

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
