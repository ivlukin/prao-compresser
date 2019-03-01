//
// Created by sorrow on 23.02.19.
//

#ifndef PRAO_COMPRESSER_SIGNALPROCESSOR_H
#define PRAO_COMPRESSER_SIGNALPROCESSOR_H
#define MERGESORT_SMALL_STRIDE 1024 * 64

#include "GPUContext.h"

/**
 * Класс для процессинга сигналов с помощью GPU
 */
class Sorter {
private:
    /**
     * Контекст устройства GPU
     */
    GPUContext context;


private:

    size_t getPaddedSize(size_t n);

    size_t getGlobalWorkSize(size_t DataElemCount, size_t LocalWorkSize);

    void printError(std::string, int errorCode);


public:
    explicit Sorter(GPUContext context) {
        this->context = context;
    }

    /**
     * Метод для сортировки массива флоатов
     * @param array - массив
     * @param arraySize размер массива
     * @param workitems - the number of work-items you wish to execute (called the global size) (256)
     */
    uint *sort(const uint array[], size_t arraySize, size_t workitems);

};


#endif //PRAO_COMPRESSER_SIGNALPROCESSOR_H
