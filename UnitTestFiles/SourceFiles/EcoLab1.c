/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
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
#include "IEcoLab1Events.h"
#include "IdEcoLab1.h"
#include "IdEcoList1.h"
#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"
#include <stdio.h>

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
    char_t* name = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;
    /* Указатель на интерфейс контейнера точек подключения */
    IEcoConnectionPointContainer* pICPC = 0;
    /* Указатель на интерфейс точки подключения */
    IEcoConnectionPoint* pICP = 0;
    /* Указатель на обратный интерфейс */
    IEcoLab1Events* pIEcoLab1Sink = 0;
    IEcoUnknown* pISinkUnk = 0;
    uint32_t cAdvise = 0;
    int32_t testArray[] = {170, 45, 75, 90, 2, 802, 24, 66};
    size_t arraySize = sizeof(testArray) / sizeof(testArray[0]);
    size_t i = 0;

    /* Проверка и создание системного интерфейса */
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
    if (result != 0 || pISys == 0) {
        goto Release;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);
    if (result != 0 || pIMem == 0) {
        goto Release;
    }

    /* Выделение блока памяти с проверкой */
    name = (char_t *)pIMem->pVTbl->Alloc(pIMem, 10);
    if (name == 0) {
        result = -1;
        goto Release;
    }

    /* Заполнение блока памяти */
    pIMem->pVTbl->Fill(pIMem, name, 'a', 9);
    name[9] = '\0'; // Добавляем нулевой терминатор

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        goto Release;
    }

    printf("\n=== LAB 3: RADIX SORT WITH CONNECTION POINTS ===\n\n");

    /* Проверка поддержки подключений обратного интерфейса */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void **)&pICPC);
    if (result != 0 || pICPC == 0) {
        printf("Error: Component does not support IEcoConnectionPointContainer (result=%d)\n", result);
        goto Release;
    }

    /* Запрос на получения интерфейса точки подключения */
    result = pICPC->pVTbl->FindConnectionPoint(pICPC, &IID_IEcoLab1Events, &pICP);
    if (result != 0 || pICP == 0) {
        printf("Error: Cannot find connection point for IEcoLab1Events (result=%d)\n", result);
        pICPC->pVTbl->Release(pICPC);
        goto Release;
    }
    
    /* Освобождение интерфейса контейнера */
    pICPC->pVTbl->Release(pICPC);
    pICPC = 0;

    /* Создание экземпляра обратного интерфейса (Sink) */
    result = createCEcoLab1Sink(pIMem, (IEcoLab1Events**)&pIEcoLab1Sink);
    if (result != 0 || pIEcoLab1Sink == 0) {
        printf("Error: Cannot create Sink (result=%d)\n", result);
        goto Release;
    }

    /* Получение IEcoUnknown от Sink */
    result = pIEcoLab1Sink->pVTbl->QueryInterface(pIEcoLab1Sink, &IID_IEcoUnknown, (void **)&pISinkUnk);
    if (result != 0 || pISinkUnk == 0) {
        printf("Error: Cannot get IEcoUnknown from Sink (result=%d)\n", result);
        goto Release;
    }

    /* Подключение Sink к компоненту */
    result = pICP->pVTbl->Advise(pICP, pISinkUnk, &cAdvise);
    if (result == 0 && cAdvise != 0) {
        printf("Successfully connected to event source (Cookie: %u)\n\n", cAdvise);
    } else {
        printf("Error: Failed to connect to event source (result=%d, cookie=%u)\n", result, cAdvise);
        pISinkUnk->pVTbl->Release(pISinkUnk);
        goto Release;
    }

    /* Освобождение IEcoUnknown Sink */
    pISinkUnk->pVTbl->Release(pISinkUnk);
    pISinkUnk = 0;

    printf("Testing Radix Sort with Events:\n");
    printf("-------------------------------\n");
    printf("Array before sorting: ");
    for (i = 0; i < arraySize; i++) {
        printf("%d ", testArray[i]);
    }
    printf("\n\n");

    /* Вызов RadixSortInt - будут генерироваться события */
    pIEcoLab1->pVTbl->RadixSortInt(pIEcoLab1, testArray, arraySize);

    printf("\nArray after sorting: ");
    for (i = 0; i < arraySize; i++) {
        printf("%d ", testArray[i]);
    }
    printf("\n\n");

    /* Отключение от событий */
    if (cAdvise != 0) {
        result = pICP->pVTbl->Unadvise(pICP, cAdvise);
        printf("Disconnected from event source\n");
        cAdvise = 0;
    }

Release:

    /* Освобождение в правильном порядке */
    if (pICP != 0) {
        pICP->pVTbl->Release(pICP);
        pICP = 0;
    }

    if (pIEcoLab1Sink != 0) {
        pIEcoLab1Sink->pVTbl->Release(pIEcoLab1Sink);
        pIEcoLab1Sink = 0;
    }

    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
        pIEcoLab1 = 0;
    }

    /* Освобождение памяти */
    if (name != 0 && pIMem != 0) {
        pIMem->pVTbl->Free(pIMem, name);
        name = 0;
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
        pIMem = 0;
    }

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
        pIBus = 0;
    }

    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
        pISys = 0;
    }
    return result;
}
