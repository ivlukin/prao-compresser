//
// Created by sorrow on 23.02.19.
//

#ifndef PRAO_COMPRESSER_SIGNALPROCESSOR_H
#define PRAO_COMPRESSER_SIGNALPROCESSOR_H

#include "GPUContext.h"

/**
 * Класс для процессинга сигналов с помощью GPU
 */
class SignalProcessor {
private:
    /**
     * Контекст устройства GPU
     */
    GPUContext context;


public:
    SignalProcessor() {
        context = GPUContext();
    }

    /**
     * Метод для сортировки массива флоатов
     * @param array - массив
     * @param arraySize размер массива
     * @param workitems - количество параллельно выполняемых потоков
     */
    void sort(const float *array[], size_t arraySize, size_t workitems);
};


#endif //PRAO_COMPRESSER_SIGNALPROCESSOR_H
