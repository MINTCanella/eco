/*
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"

/* Стандартные библиотеки для тестирования */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

/*
 *
 * <сводка>
 *   Функция сравнения для qsort
 * </сводка>
 *
 * <описание>
 *   Вспомогательная функция для сравнения элементов в qsort
 * </описание>
 *
 */
int __cdecl compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

/*
 *
 * <сводка>
 *   Функция заполнения массива случайными числами
 * </сводка>
 *
 * <описание>
 *   Заполняет массив случайными числами в заданном диапазоне
 * </описание>
 *
 */
static void fillRandomArray(int* array, size_t size, int min, int max) {
    size_t i;
    for (i = 0; i < size; i++) {
        array[i] = rand() % (max - min + 1) + min;
    }
}

/*
 *
 * <сводка>
 *   Функция проверки корректности сортировки
 * </сводка>
 *
 * <описание>
 *   Проверяет, что массив отсортирован правильно
 * </описание>
 *
 */
static int isSortedCorrectly(int* array, size_t size) {
    size_t i;
    for (i = 1; i < size; i++) {
        if (array[i] < array[i - 1]) {
            return 0;
        }
    }
    return 1;
}

/*
 *
 * <сводка>
 *   Функция измерения времени сортировки
 * </сводка>
 *
 * <описание>
 *   Измеряет время выполнения множественных вызовов сортировки
 * </описание>
 *
 */
static void measureSortingTime(IEcoLab1* pIEcoLab1, int* testArray, size_t size, int iterations, const char* testName) {
    LARGE_INTEGER frequency, start, end;
    double cpu_time_used;
    int* tempArray;
    size_t i, j;
    
    QueryPerformanceFrequency(&frequency);
    
    tempArray = (int*)malloc(size * sizeof(int));
    if (tempArray == 0) return;
    
    printf("\n=== %s ===\n", testName);
    printf("Array size: %Iu elements\n", size);
    printf("Number of iterations: %d\n", iterations);
    
    QueryPerformanceCounter(&start);
    
    /* Вызов сортировки */
    for (i = 0; i < (size_t)iterations; i++) {
        /* Копируем исходный массив */
        for (j = 0; j < size; j++) {
            tempArray[j] = testArray[j];
        }
        
        /* Сортируем */
        pIEcoLab1->pVTbl->RadixSortInt(pIEcoLab1, tempArray, (uint32_t)size);
    }
    
    QueryPerformanceCounter(&end);
    cpu_time_used = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    
    printf("Total execution time: %.6f seconds\n", cpu_time_used);
    printf("Average time per sort: %.8f seconds\n", cpu_time_used / iterations);
    
    /* Проверка корректности сортировки */
    if (isSortedCorrectly(tempArray, size)) {
        printf("Sorting verification: PASSED\n");
    } else {
        printf("Sorting verification: FAILED\n");
    }
    
    free(tempArray);
}

/*
 *
 * <сводка>
 *   Функция сравнения с qsort
 * </сводка>
 *
 * <описание>
 *   Сравнивает производительность нашей сортировки с qsort
 * </описание>
 *
 */
static void compareWithQsort(int* array, size_t size, int iterations, IEcoLab1* pIEcoLab1) {
    LARGE_INTEGER frequency, start, end;
    double qsort_time, our_time;
    int* tempArray;
    size_t i, j;
    
    QueryPerformanceFrequency(&frequency);
    tempArray = (int*)malloc(size * sizeof(int));
    if (tempArray == 0) return;
    
    printf("\n=== PERFORMANCE COMPARISON ===\n");
    printf("Array size: %Iu elements\n", size);
    printf("Number of iterations: %d\n", iterations);
    
    /* Тест qsort */
    QueryPerformanceCounter(&start);
    for (i = 0; i < (size_t)iterations; i++) {
        for (j = 0; j < size; j++) {
            tempArray[j] = array[j];
        }
        qsort(tempArray, size, sizeof(int), compare);
    }
    QueryPerformanceCounter(&end);
    qsort_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    
    /* Тест radix */
    QueryPerformanceCounter(&start);
    for (i = 0; i < (size_t)iterations; i++) {
        for (j = 0; j < size; j++) {
            tempArray[j] = array[j];
        }
        pIEcoLab1->pVTbl->RadixSortInt(pIEcoLab1, tempArray, (uint32_t)size);
    }
    QueryPerformanceCounter(&end);
    our_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    
    printf("qsort: %.6f seconds\n", qsort_time);
    printf("RadixSort: %.6f seconds\n", our_time);
    
    if (qsort_time > 0) {
        printf("Ratio (radix/qsort): %.2f\n", our_time / qsort_time);
    } else {
        printf("Ratio: qsort time too small to calculate ratio\n");
    }
    
    free(tempArray);
}


/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */

int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;

	/* Объявляем переменные */
    int testArray[] = {170, 45, 75, 90, 2, 802, 24, 66};
    size_t arraySize = sizeof(testArray) / sizeof(testArray[0]);
    
    /* Размеры массивов для тестирования */
    size_t smallSize = 100;      // 100 элементов
    size_t mediumSize = 1000;    // 1000 элементов  
    size_t largeSize = 10000;    // 10,000 элементов
    size_t veryLargeSize = 50000; // 50,000 элементов
    
    int *smallArray, *mediumArray, *largeArray, *veryLargeArray;
    size_t i;
    
    /* Инициализация генератора случайных чисел */
    srand((unsigned int)time(NULL));

    /* Выделяем память для массивов */
    smallArray = (int*)malloc(smallSize * sizeof(int));
    mediumArray = (int*)malloc(mediumSize * sizeof(int));
    largeArray = (int*)malloc(largeSize * sizeof(int));
    veryLargeArray = (int*)malloc(veryLargeSize * sizeof(int));
    
    if (!smallArray || !mediumArray || !largeArray || !veryLargeArray) {
        printf("Memory allocation failed!\n");
        goto Cleanup;
    }

    /* Заполняем массивы случайными числами */
    fillRandomArray(smallArray, smallSize, 0, 1000);
    fillRandomArray(mediumArray, mediumSize, 0, 10000);
    fillRandomArray(largeArray, largeSize, 0, 100000);
    fillRandomArray(veryLargeArray, veryLargeSize, 0, 1000000);


 /* Проверка и создание системного интерфейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

	/* Получение тестируемого интерфейса */
	result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
	if (result != 0 || pIEcoLab1 == 0) {
		/* Освобождение интерфейсов в случае ошибки */
		goto Release;
	}

    printf("=== RADIX SORT TESTING ===\n");

    /* Базовый тест */
    printf("\n--- Basic test ---\n");
    printf("Initial array: ");
    for (i = 0; i < arraySize; i++) {
        printf("%d ", testArray[i]);
    }
    printf("\n");

    /* Вызов сортировки */
    pIEcoLab1->pVTbl->RadixSortInt(pIEcoLab1, testArray, (uint32_t)arraySize);

    printf("Sorted array: ");
    for (i = 0; i < arraySize; i++) {
        printf("%d ", testArray[i]);
    }
    printf("\n");

    /* Проверка корректности базового теста */
    if (isSortedCorrectly(testArray, arraySize)) {
        printf("Basic test: PASSED\n");
    } else {
        printf("Basic test: FAILED\n");
    }

      /* Тесты для типов данных */
    printf("\n=== TESTING ALL DATA TYPES ===\n");
    
    /* Тест для int64_t */
    {
        int64_t testArrayLong[] = {170LL, 45LL, 75LL, 90LL, 2LL, 802LL, 24LL, 66LL};
        size_t arraySizeLong = sizeof(testArrayLong) / sizeof(testArrayLong[0]);
        size_t j;
        int sorted_long;
        
        printf("\n--- Basic test for int64_t ---\n");
        printf("Initial array: ");
        for (j = 0; j < arraySizeLong; j++) {
            printf("%lld ", testArrayLong[j]);
        }
        printf("\n");
        
        pIEcoLab1->pVTbl->RadixSortLong(pIEcoLab1, testArrayLong, arraySizeLong);
        
        printf("Sorted array: ");
        for (j = 0; j < arraySizeLong; j++) {
            printf("%lld ", testArrayLong[j]);
        }
        printf("\n");
        
        /* Проверка корректности */
        sorted_long = 1;
        for (j = 1; j < arraySizeLong; j++) {
            if (testArrayLong[j] < testArrayLong[j - 1]) {
                sorted_long = 0;
                break;
            }
        }
        printf("Basic test for int64_t: %s\n", sorted_long ? "PASSED" : "FAILED");
    }

    /* Тест для float */
    {
        float testArrayFloat[] = {17.5f, 4.5f, 7.5f, 9.0f, 0.2f, 80.2f, 2.4f, 6.6f, -3.5f, -1.2f};
        size_t arraySizeFloat = sizeof(testArrayFloat) / sizeof(testArrayFloat[0]);
        size_t j;
        int sorted_float;
        
        printf("\n--- Basic test for float ---\n");
        printf("Initial array: ");
        for (j = 0; j < arraySizeFloat; j++) {
            printf("%.2f ", testArrayFloat[j]);
        }
        printf("\n");
        
        pIEcoLab1->pVTbl->RadixSortFloat(pIEcoLab1, testArrayFloat, arraySizeFloat);
        
        printf("Sorted array: ");
        for (j = 0; j < arraySizeFloat; j++) {
            printf("%.2f ", testArrayFloat[j]);
        }
        printf("\n");
        
        /* Проверка корректности */
        sorted_float = 1;
        for (j = 1; j < arraySizeFloat; j++) {
            if (testArrayFloat[j] < testArrayFloat[j - 1]) {
                sorted_float = 0;
                break;
            }
        }
        printf("Basic test for float: %s\n", sorted_float ? "PASSED" : "FAILED");
    }

    /* Тест для double */
    {
        double testArrayDouble[] = {170.5, 45.3, 75.1, 90.8, 2.7, 802.4, 24.9, 66.2, -15.5, -8.3};
        size_t arraySizeDouble = sizeof(testArrayDouble) / sizeof(testArrayDouble[0]);
        size_t j;
        int sorted_double;
        
        printf("\n--- Basic test for double ---\n");
        printf("Initial array: ");
        for (j = 0; j < arraySizeDouble; j++) {
            printf("%.2f ", testArrayDouble[j]);
        }
        printf("\n");
        
        pIEcoLab1->pVTbl->RadixSortDouble(pIEcoLab1, testArrayDouble, arraySizeDouble);
        
        printf("Sorted array: ");
        for (j = 0; j < arraySizeDouble; j++) {
            printf("%.2f ", testArrayDouble[j]);
        }
        printf("\n");
        
        /* Проверка корректности */
        sorted_double = 1;
        for (j = 1; j < arraySizeDouble; j++) {
            if (testArrayDouble[j] < testArrayDouble[j - 1]) {
                sorted_double = 0;
                break;
            }
        }
        printf("Basic test for double: %s\n", sorted_double ? "PASSED" : "FAILED");
    }

    /* Тест разных размеров массивов */
    printf("\n--- Test of different array sizes ---\n");
    
    /* Small array тест */
    printf("Small array (%Iu elements, showing first 10): ", smallSize);
    pIEcoLab1->pVTbl->RadixSortInt(pIEcoLab1, smallArray, (uint32_t)smallSize);
    for (i = 0; i < 10 && i < smallSize; i++) {
        printf("%d ", smallArray[i]);
    }
    printf("...\n");
    printf("Small array sorting: %s\n", isSortedCorrectly(smallArray, smallSize) ? "PASSED" : "FAILED");

    /* Medium array тест */
    printf("Medium array (%Iu elements, showing first 10): ", mediumSize);
    pIEcoLab1->pVTbl->RadixSortInt(pIEcoLab1, mediumArray, (uint32_t)mediumSize);
    for (i = 0; i < 10 && i < mediumSize; i++) {
        printf("%d ", mediumArray[i]);
    }
    printf("...\n");
    printf("Medium array sorting: %s\n", isSortedCorrectly(mediumArray, mediumSize) ? "PASSED" : "FAILED");

    /* Large array тест */
    printf("Large array (%Iu elements, showing first 10): ", largeSize);
    pIEcoLab1->pVTbl->RadixSortInt(pIEcoLab1, largeArray, (uint32_t)largeSize);
    for (i = 0; i < 10 && i < largeSize; i++) {
        printf("%d ", largeArray[i]);
    }
    printf("...\n");
    printf("Large array sorting: %s\n", isSortedCorrectly(largeArray, largeSize) ? "PASSED" : "FAILED");

    /* Very large array тест */
    printf("Very large array (%Iu elements, showing first 10): ", veryLargeSize);
    pIEcoLab1->pVTbl->RadixSortInt(pIEcoLab1, veryLargeArray, (uint32_t)veryLargeSize);
    for (i = 0; i < 10 && i < veryLargeSize; i++) {
        printf("%d ", veryLargeArray[i]);
    }
    printf("...\n");
    printf("Very large array sorting: %s\n", isSortedCorrectly(veryLargeArray, veryLargeSize) ? "PASSED" : "FAILED");

    /* Тесты производительности */
    printf("\n=== PERFORMANCE TESTS ===\n");
    measureSortingTime(pIEcoLab1, smallArray, smallSize, 1000, "SMALL ARRAY (100 elements)");
    measureSortingTime(pIEcoLab1, mediumArray, mediumSize, 500, "MEDIUM ARRAY (1,000 elements)");
    measureSortingTime(pIEcoLab1, largeArray, largeSize, 100, "LARGE ARRAY (10,000 elements)");
    measureSortingTime(pIEcoLab1, veryLargeArray, veryLargeSize, 50, "VERY LARGE ARRAY (50,000 elements)");

    /* Сравнение с qsort на разных размерах */
    printf("\n=== COMPARISON WITH QSORT ===\n");
    compareWithQsort(smallArray, smallSize, 1000, pIEcoLab1);
    compareWithQsort(mediumArray, mediumSize, 200, pIEcoLab1);
    compareWithQsort(largeArray, largeSize, 50, pIEcoLab1);

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }


    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }
Cleanup:
    /* Освобождение памяти массивов */
    if (smallArray) free(smallArray);
    if (mediumArray) free(mediumArray);
    if (largeArray) free(largeArray);
    if (veryLargeArray) free(veryLargeArray);

    printf("\nTesting complete. Press any key to exit...\n");
    system("pause");
    return result;
}
