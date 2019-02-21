#include <iostream>
#include <chrono>

#include "AdvancedMatrix.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "   Аргументы командной строки:" << endl;
        cout << "1. Размерность генерируемой матрицы." << endl;
        cout << "2. Количество потоков." << endl;
        return 1;
    }

    int widthMatrix = atoi(argv[1]);
    int threadsNumber = atoi(argv[2]);

    AdvancedMatrix matrix(widthMatrix, threadsNumber);
    cout << "Сгенерированная матрицa:\n" << matrix << endl;
    
    auto start_time = chrono::steady_clock::now();
    
    matrix.startGauss();
    
    auto end_time = chrono::steady_clock::now();
    auto elapsed_us = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    
    cout << "Ответ: " << endl;
    matrix.printVector();
    
    cout << "Время работы алгоритма: " << (double) elapsed_us.count() / 1000000000 << endl;

    return 0;
}
