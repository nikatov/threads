#pragma once

#include <iostream>

#include "Barrier.hpp"

class AdvancedMatrix
{
public:
    AdvancedMatrix(int nWidth, int nThreads);
    ~AdvancedMatrix();
    void printVector();
    void startGauss();
    int getThreadsNumber() { return threadsNumber; }
    
    friend std::ostream& operator<< (std::ostream &out, const AdvancedMatrix &matrix);
private:
    enum direction
    {
        forward,
        backward
    };

    void gauss();
    void divisionString(int numDivisionString, int threadID, direction direct);
    void subtractionString(int numTermString, int numSubtrahendString);
    void divisionDiagonal(int row);

    int ind(int i, int j) const { return i*width + j; }

    const int width;
    double* p_matrix;
    double* p_vector;
    int threadsNumber;
    int threadsIDs;
    Barrier myBarrier;
};
