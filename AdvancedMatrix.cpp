#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <vector>

#include "AdvancedMatrix.hpp"
#include "Barrier.hpp"

AdvancedMatrix::AdvancedMatrix(int nWidth , int nThreads) : width(nWidth),
                                             p_matrix(new double[nWidth * nWidth]),
                                             p_vector(new double[nWidth]),
                                             threadsNumber(nThreads),
                                             threadsIDs(0),
                                             myBarrier(nThreads)
{
    srand(time(NULL));
    double sumElements;
    for (int i = 0; i < width; ++i)
    {
        sumElements = 0;
        for (int j = 0; j < width; ++j)
        {
            p_matrix[ind(i, j)] = (double)rand() / RAND_MAX*2 - 1;
            sumElements += p_matrix[ind(i, j)];
        }
        p_vector[i] = sumElements;
    }
}

AdvancedMatrix::~AdvancedMatrix()
{
    delete[] p_matrix;
    delete[] p_vector;
}

std::ostream& operator<< (std::ostream &out, const AdvancedMatrix &matrix)
{
    for (int i = 0; i < matrix.width * matrix.width ; ++i){
        out << std::setw(7) << std::fixed << std::setprecision(3) << matrix.p_matrix[i];
        if ((i + 1)%matrix.width == 0) {
            out << "  |" << std::setw(7) << std::fixed << std::setprecision(3) << matrix.p_vector[i/matrix.width] << std::endl;
        }
    }
    return out;
}

void AdvancedMatrix::printVector()
{
    for (int i = 0; i < width; ++i){
        std::cout << "x" << i << " = " << std::fixed << std::setprecision(3) << p_vector[i] << std::endl;
    }
}

void AdvancedMatrix::startGauss()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < getThreadsNumber(); ++i){
        threads.push_back(std::thread([&](){ gauss(); }));
    }
    for (auto& th : threads){
        th.join();
    }
}

void AdvancedMatrix::gauss()
{
    if (threadsIDs == threadsNumber)
    {
        std::cout << "Количество потоков, вызвавших метод Гаусса превышает допустимое число, барьер не выдержит." << std::endl;
        return;
    }
    int threadID = threadsIDs++;
    
//==================== ПРЯМОЙ ХОД ====================
    if (threadID == 0){
        std::cout << "Прямой ход:" << std::endl;
    }
    for (int j = 0; j < width; ++j){
        divisionString(j, threadID, forward);
        myBarrier.wait();
        for (int i = j + 1; i < width; ++i){
            if(i % threadsNumber == threadID){
                subtractionString(i, j);
            }
        }
        myBarrier.wait();

        //Печать матрицы
        if (threadID == 0){
            std::cout << *this << std::endl;
        }
        myBarrier.wait();
    }
    
//==================== ОБРАТНЫЙ ХОД ====================

    if (threadID == 0){
        std::cout << "Обратный ход:" << std::endl;
    }
    for (int j = width - 1; j > 0; --j){
        for (int i = j - 1; i >= 0; --i){
            if(i % threadsNumber == threadID){
                subtractionString(i, j);
            }
        }
        myBarrier.wait();

        //Печать матрицы
        if (threadID == 0){
            std::cout << *this << std::endl;
        }
        myBarrier.wait();
    }
}

void AdvancedMatrix::divisionString(int numDivisionString, int threadID, direction direct)
{
    double weight = p_matrix[ind(numDivisionString, numDivisionString)];
    myBarrier.wait();
    if (threadID == 0){
        p_vector[numDivisionString] /= weight;
    }
    if (direct == forward)
    {
        for (int i = numDivisionString; i < width; ++i){
            if (i % threadsNumber == threadID){
                p_matrix[ind(numDivisionString, i)] /= weight;
            }
        }
    }
    else
    {
        for (int i = numDivisionString; i >= 0; --i){
            if (i % threadsNumber == threadID){
                p_matrix[ind(numDivisionString, i)] /= weight;
            }
        }
    }
}

void AdvancedMatrix::subtractionString(int numTermString, int numSubtrahendString)
{
    double weight = p_matrix[ind(numTermString, numSubtrahendString)];
    for (int i = numSubtrahendString; i < width; ++i){
        p_matrix[ind(numTermString, i)] -= p_matrix[ind(numSubtrahendString, i)] * weight;
    }
    p_vector[numTermString] -= p_vector[numSubtrahendString] * weight;
}
