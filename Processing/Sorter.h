//
// Created by sorrow on 23.02.19.
//

#ifndef PRAO_COMPRESSER_SIGNALPROCESSOR_H
#define PRAO_COMPRESSER_SIGNALPROCESSOR_H
#define MERGESORT_SMALL_STRIDE 1024 * 64

#include "OpenCLContext.h"
#include "Processor.h"

typedef unsigned int uint;

/**
 * Класс для процессинга сигналов с помощью GPU
 */
class Sorter : Processor {
private:

    size_t getPaddedSize(size_t n);

    size_t getGlobalWorkSize(size_t DataElemCount, size_t LocalWorkSize);

public:

    explicit Sorter(OpenCLContext context) : Processor(context) {

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
