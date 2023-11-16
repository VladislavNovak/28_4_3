#ifndef INC_28_4_3_THREADS_H
#define INC_28_4_3_THREADS_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "constants.h"
#include "utilities.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

void receptionThread(vector<string> &orderList);

void kitchenThread(vector<string> &orderList, vector<string> &cookedList);

void deliveryThread(vector<string> &cookedList, vector<string> &completedList);

#endif //INC_28_4_3_THREADS_H
