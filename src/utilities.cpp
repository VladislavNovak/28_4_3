#include "utilities.h"

// Возвращает случайное число в диапазоне от from до to (включительно)
int getRandomIntInRange(int from, int to) {
    return (from + std::rand() % (to - from + 1)); // NOLINT(cert-msc50-cpp)
}