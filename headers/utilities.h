#ifndef INC_28_4_3_UTILITIES_H
#define INC_28_4_3_UTILITIES_H

#include <vector>
#include <cstdlib>
#include <ctime>

// Извлекает первый элемент из вектора.
// Затем в исходном векторе второй элемент смещается на место первого
template<typename T>
T popFront (std::vector<T> &list) {
    T temp = list.front();
    list.erase(list.begin());
    return temp;
}

int getRandomIntInRange(int from, int to);

#endif //INC_28_4_3_UTILITIES_H
