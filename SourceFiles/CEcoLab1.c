/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include <stdio.h>

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IEcoMemoryAllocator1.h"
#include "CEcoLab1.h"
#include "CEcoLab1EnumConnectionPoints.h"
#include "IEcoConnectionPointContainer.h"

// Добавляем CID компонентов калькуляторов
#ifndef __CID_EcoCalculatorA
static const UGUID CID_EcoCalculatorA = {0x01, 0x10, {0x48, 0x28, 0xF6, 0x55, 0x2E, 0x45, 0x40, 0xE7, 0x81, 0x21, 0xEB, 0xD2, 0x20, 0xDC, 0x36, 0x0E}};
#endif

#ifndef __CID_EcoCalculatorB  
static const UGUID CID_EcoCalculatorB = {0x01, 0x10, {0xAE, 0x20, 0x2E, 0x54, 0x3C, 0xE5, 0x45, 0x50, 0x89, 0x96, 0x03, 0xBD, 0x70, 0xC6, 0x25, 0x65}};
#endif

#ifndef __CID_EcoCalculatorC
static const UGUID CID_EcoCalculatorC = {0x01, 0x10, {0x48, 0x28, 0xF6, 0x55, 0x2E, 0x45, 0x40, 0xE7, 0x81, 0x21, 0xEB, 0xD2, 0x20, 0xDC, 0x36, 0x0E}};
#endif

#ifndef __CID_EcoCalculatorD
static const UGUID CID_EcoCalculatorD = {0x01, 0x10, {0x3A, 0x8E, 0x44, 0x67, 0x7E, 0x82, 0x47, 0x5C, 0xB4, 0xA3, 0x71, 0x9E, 0xD8, 0x39, 0x7E, 0x61}};
#endif

#ifndef __CID_EcoCalculatorE
static const UGUID CID_EcoCalculatorE = {0x01, 0x10, {0x87, 0x2F, 0xEF, 0x1D, 0xE3, 0x31, 0x4B, 0x87, 0xAD, 0x44, 0xD1, 0xE7, 0xC2, 0x32, 0xC2, 0xF0}};
#endif

/* Добавили вспомогательные функции для работы с сортировкой */
/* Получение максимального значения в массиве */
static int32_t getMaxInt32(int32_t* array, size_t size) {
    int32_t max = array[0];
    size_t i;
    for (i = 1; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

/*
 * Функция инициализации калькуляторов
 */
static int16_t ECOCALLMETHOD initCalculators(CEcoLab1* pCMe) {
    int16_t result = -1;
    
    /* Получение интерфейсной шины */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void**)&pCMe->m_pIBus);
    if (result != 0 || pCMe->m_pIBus == 0) {
        return result;
    }

    /* Пытаемся получить компонент E (включение) */
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pCalculatorY);
    if (result == 0 && pCMe->m_pCalculatorY != 0) {
        /* Получаем IEcoCalculatorX через QueryInterface */
        result = pCMe->m_pCalculatorY->pVTbl->QueryInterface(pCMe->m_pCalculatorY, &IID_IEcoCalculatorX, (void**)&pCMe->m_pCalculatorX);
        pCMe->m_bUseAggregation = 0;
        if (pCMe->m_pCalculatorX != 0) {
            printf("EcoLab1: Using Component E (Aggregation)\n");
        }
        return (pCMe->m_pCalculatorX != 0) ? 0 : -1;
    }

    /* Если E не доступен, пробуем D (включение) */
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pCalculatorY);
    if (result == 0 && pCMe->m_pCalculatorY != 0) {
        /* Получаем IEcoCalculatorX через QueryInterface */
        result = pCMe->m_pCalculatorY->pVTbl->QueryInterface(pCMe->m_pCalculatorY, &IID_IEcoCalculatorX, (void**)&pCMe->m_pCalculatorX);
        pCMe->m_bUseAggregation = 0;
        if (pCMe->m_pCalculatorX != 0) {
            printf("EcoLab1: Using Component D (Inclusion)\n");
        }
        return (pCMe->m_pCalculatorX != 0) ? 0 : -1;
    }

    /* Если D не доступен, пробуем C (включение) */
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalculatorC, 0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pCalculatorY);
    if (result == 0 && pCMe->m_pCalculatorY != 0) {
        /* Получаем IEcoCalculatorX через QueryInterface */
        result = pCMe->m_pCalculatorY->pVTbl->QueryInterface(pCMe->m_pCalculatorY, &IID_IEcoCalculatorX, (void**)&pCMe->m_pCalculatorX);
        pCMe->m_bUseAggregation = 0;
        if (pCMe->m_pCalculatorX != 0) {
            printf("EcoLab1: Using Component C (Extension)\n");
        }
        return (pCMe->m_pCalculatorX != 0) ? 0 : -1;
    }

    /* Если C не доступен, пробуем B (агрегирование) */
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalculatorB, 
                                                 (IEcoUnknown*)pCMe,
                                                 &IID_IEcoUnknown,
                                                 (void**)&pCMe->m_pInnerUnknown);
    if (result == 0 && pCMe->m_pInnerUnknown != 0) {
        result = pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, &IID_IEcoCalculatorX, (void**)&pCMe->m_pCalculatorX);
        if (result != 0 || pCMe->m_pCalculatorX == 0) {
            pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown);
            pCMe->m_pInnerUnknown = 0;
            return -1;
        }
        pCMe->m_bUseAggregation = 1;
        pCMe->m_pCalculatorY = 0;
        printf("EcoLab1: Using Component B (Aggregation)\n");
        return 0;
    }

    /* Если B не доступен, пробуем A (включение) */
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pCalculatorX);
    if (result == 0 && pCMe->m_pCalculatorX != 0) {
        pCMe->m_bUseAggregation = 0;
        pCMe->m_pCalculatorY = 0;
        printf("EcoLab1: Using Component A (Inclusion)\n");
        return 0;
    }

    return -1; // Ни один компонент не найден
}

/*
 * Функция освобождения калькуляторов
 */
static void ECOCALLMETHOD releaseCalculators(CEcoLab1* pCMe) {
    if (pCMe->m_pCalculatorX != 0) {
        pCMe->m_pCalculatorX->pVTbl->Release(pCMe->m_pCalculatorX);
        pCMe->m_pCalculatorX = 0;
    }
    
    if (!pCMe->m_bUseAggregation && pCMe->m_pCalculatorY != 0) {
        pCMe->m_pCalculatorY->pVTbl->Release(pCMe->m_pCalculatorY);
        pCMe->m_pCalculatorY = 0;
    }
    
    if (pCMe->m_bUseAggregation && pCMe->m_pInnerUnknown != 0) {
        pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown);
        pCMe->m_pInnerUnknown = 0;
    }
    
    if (pCMe->m_pIBus != 0) {
        pCMe->m_pIBus->pVTbl->Release(pCMe->m_pIBus);
        pCMe->m_pIBus = 0;
    }
}

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoLab1
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ struct IEcoLab1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoConnectionPointContainer) ) {
        *ppv = &pCMe->m_pVTblICPC;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) ) {
        if (pCMe->m_pCalculatorX != 0) {
            return pCMe->m_pCalculatorX->pVTbl->QueryInterface(pCMe->m_pCalculatorX, riid, ppv);
        } else {
            *ppv = 0;
            return -1;
        }
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
        if (pCMe->m_pCalculatorY != 0) {
            return pCMe->m_pCalculatorY->pVTbl->QueryInterface(pCMe->m_pCalculatorY, riid, ppv);
        } else {
            *ppv = 0;
            return -1;
        }
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/* Добавили функции Fire для всех событий радиксной сортировки */

/*
 *
 * <сводка>
 *   Функция Fire_OnSortStart
 * </сводка>
 *
 * <описание>
 *   Функция отправки события начала сортировки всем подписчикам
 * </описание>
 *
 */
int16_t CEcoLab1_Fire_OnSortStart(/* in */ struct CEcoLab1* me, /* in */ size_t arraySize, /* in */ int32_t maxValue) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pISinkCP != 0) {
        result = ((IEcoConnectionPoint*)pCMe->m_pISinkCP)->pVTbl->EnumConnections((IEcoConnectionPoint*)pCMe->m_pISinkCP, &pEnum);
        if ( (result == 0) && (pEnum != 0) ) {
            while (pEnum->pVTbl->Next(pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ( (result == 0) && (pIEvents != 0) ) {
                    result = pIEvents->pVTbl->OnSortStart(pIEvents, arraySize, maxValue);
                    pIEvents->pVTbl->Release(pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release(pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnProcessDigit
 * </сводка>
 *
 * <описание>
 *   Функция отправки события обработки разряда всем подписчикам
 * </описание>
 *
 */
int16_t CEcoLab1_Fire_OnProcessDigit(/* in */ struct CEcoLab1* me, /* in */ int32_t digitPosition, /* in */ int32_t divisor) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pISinkCP != 0) {
        result = ((IEcoConnectionPoint*)pCMe->m_pISinkCP)->pVTbl->EnumConnections((IEcoConnectionPoint*)pCMe->m_pISinkCP, &pEnum);
        if ( (result == 0) && (pEnum != 0) ) {
            while (pEnum->pVTbl->Next(pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ( (result == 0) && (pIEvents != 0) ) {
                    result = pIEvents->pVTbl->OnProcessDigit(pIEvents, digitPosition, divisor);
                    pIEvents->pVTbl->Release(pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release(pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnElementMove
 * </сводка>
 *
 * <описание>
 *   Функция отправки события перемещения элемента всем подписчикам
 * </описание>
 *
 */
int16_t CEcoLab1_Fire_OnElementMove(/* in */ struct CEcoLab1* me, /* in */ int32_t value, /* in */ size_t fromIndex, /* in */ size_t toIndex) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pISinkCP != 0) {
        result = ((IEcoConnectionPoint*)pCMe->m_pISinkCP)->pVTbl->EnumConnections((IEcoConnectionPoint*)pCMe->m_pISinkCP, &pEnum);
        if ( (result == 0) && (pEnum != 0) ) {
            while (pEnum->pVTbl->Next(pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ( (result == 0) && (pIEvents != 0) ) {
                    result = pIEvents->pVTbl->OnElementMove(pIEvents, value, fromIndex, toIndex);
                    pIEvents->pVTbl->Release(pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release(pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnDigitComplete
 * </сводка>
 *
 * <описание>
 *   Функция отправки события завершения обработки разряда всем подписчикам
 * </описание>
 *
 */
int16_t CEcoLab1_Fire_OnDigitComplete(/* in */ struct CEcoLab1* me, /* in */ int32_t divisor) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pISinkCP != 0) {
        result = ((IEcoConnectionPoint*)pCMe->m_pISinkCP)->pVTbl->EnumConnections((IEcoConnectionPoint*)pCMe->m_pISinkCP, &pEnum);
        if ( (result == 0) && (pEnum != 0) ) {
            while (pEnum->pVTbl->Next(pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ( (result == 0) && (pIEvents != 0) ) {
                    result = pIEvents->pVTbl->OnDigitComplete(pIEvents, divisor);
                    pIEvents->pVTbl->Release(pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release(pEnum);
        }
    }
    return result;
}

/*
 *
 * <сводка>
 *   Функция Fire_OnSortComplete
 * </сводка>
 *
 * <описание>
 *   Функция отправки события завершения сортировки всем подписчикам
 * </описание>
 *
 */
int16_t CEcoLab1_Fire_OnSortComplete(/* in */ struct CEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = 0;
    IEcoEnumConnections* pEnum = 0;
    IEcoLab1Events* pIEvents = 0;
    EcoConnectionData cd;

    if (me == 0 ) {
        return -1;
    }

    if (pCMe->m_pISinkCP != 0) {
        result = ((IEcoConnectionPoint*)pCMe->m_pISinkCP)->pVTbl->EnumConnections((IEcoConnectionPoint*)pCMe->m_pISinkCP, &pEnum);
        if ( (result == 0) && (pEnum != 0) ) {
            while (pEnum->pVTbl->Next(pEnum, 1, &cd, 0) == 0) {
                result = cd.pUnk->pVTbl->QueryInterface(cd.pUnk, &IID_IEcoLab1Events, (void**)&pIEvents);
                if ( (result == 0) && (pIEvents != 0) ) {
                    result = pIEvents->pVTbl->OnSortComplete(pIEvents);
                    pIEvents->pVTbl->Release(pIEvents);
                }
                cd.pUnk->pVTbl->Release(cd.pUnk);
            }
            pEnum->pVTbl->Release(pEnum);
        }
    }
    return result;
}

/* Добавили реализацию радиксной сортировки с вызовами событий */

/*
 *
 * <сводка>
 *   Функция RadixSortInt
 * </сводка>
 *
 * <описание>
 *   Функция радиксной сортировки целых чисел с генерацией событий
 * </описание>
 *
 */
static void ECOCALLMETHOD CEcoLab1_RadixSortInt(/* in */ IEcoLab1* me, /* in */ int32_t* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int32_t max;
    int32_t* output;
    int count[10];
    int i;
    size_t j;
    int exp;
    int index;
    int digitPosition = 0;

    /* КРИТИЧЕСКИ ВАЖНЫЕ ПРОВЕРКИ */
    if (me == 0) {
        return;
    }
    if (array == 0) {
        return;
    }
    if (size == 0) {
        return;
    }
    if (pCMe->m_pIMem == 0) {
        return;
    }

    if (size <= 1) {
        return;
    }

    max = getMaxInt32(array, size);

    /* Событие: начало сортировки */
    if (pCMe->Fire_OnSortStart != 0) {
        pCMe->Fire_OnSortStart(pCMe, size, max);
    }

    output = (int32_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int32_t));
    if (output == 0) return;

    for (exp = 1; max / exp > 0; exp *= 10) {
        digitPosition++;
        
        /* Событие: начало обработки разряда */
        pCMe->Fire_OnProcessDigit(pCMe, digitPosition, exp);
        
        for (i = 0; i < 10; i++) count[i] = 0;

        for (j = 0; j < size; j++) count[(array[j] / exp) % 10]++;

        for (i = 1; i < 10; i++) count[i] += count[i - 1];

        for (j = size; j > 0; j--) {
            index = j - 1;
            output[count[(array[index] / exp) % 10] - 1] = array[index];
            
            /* Событие: перемещение элемента */
            pCMe->Fire_OnElementMove(pCMe, array[index], index, count[(array[index] / exp) % 10] - 1);
            
            count[(array[index] / exp) % 10]--;
        }

        for (j = 0; j < size; j++) array[j] = output[j];
        
        /* Событие: завершение обработки разряда */
        pCMe->Fire_OnDigitComplete(pCMe, exp);
    }

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, output);
    
    /* Событие: завершение сортировки */
    pCMe->Fire_OnSortComplete(pCMe);
}

static void ECOCALLMETHOD CEcoLab1_RadixSortLong(IEcoLab1* me, int64_t* array, size_t size) {
    printf("[DEBUG] RadixSortLong not implemented\n");
}

static void ECOCALLMETHOD CEcoLab1_RadixSortFloat(IEcoLab1* me, float* array, size_t size) {
    printf("[DEBUG] RadixSortFloat not implemented\n");
}

static void ECOCALLMETHOD CEcoLab1_RadixSortDouble(IEcoLab1* me, double* array, size_t size) {
    printf("[DEBUG] RadixSortDouble not implemented\n");
}

/* Реализация методов калькулятора */
static int32_t ECOCALLMETHOD CEcoLab1_Addition(IEcoLab1* me, int16_t a, int16_t b) { 
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (pCMe->m_pCalculatorX != 0) {
        return pCMe->m_pCalculatorX->pVTbl->Addition(pCMe->m_pCalculatorX, a, b);
    }
    return 0; 
}

static int16_t ECOCALLMETHOD CEcoLab1_Subtraction(IEcoLab1* me, int16_t a, int16_t b) { 
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (pCMe->m_pCalculatorX != 0) {
        return pCMe->m_pCalculatorX->pVTbl->Subtraction(pCMe->m_pCalculatorX, a, b);
    }
    return 0; 
}

static int32_t ECOCALLMETHOD CEcoLab1_Multiplication(IEcoLab1* me, int16_t a, int16_t b) { 
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (pCMe->m_pCalculatorY != 0) {
        return pCMe->m_pCalculatorY->pVTbl->Multiplication(pCMe->m_pCalculatorY, a, b);
    }
    return 0; 
}

static int16_t ECOCALLMETHOD CEcoLab1_Division(IEcoLab1* me, int16_t a, int16_t b) { 
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (pCMe->m_pCalculatorY != 0) {
        return pCMe->m_pCalculatorY->pVTbl->Division(pCMe->m_pCalculatorY, a, b);
    }
    return 0; 
}

/*
 *
 * <сводка>
 *   Функция MyFunction
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_MyFunction(/* in */ struct IEcoLab1* me, /* in */ char_t* Name, /* out */ char_t** copyName) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t index = 0;

    /* Проверка указателей */
    if (me == 0 || Name == 0 || copyName == 0) {
        return -1;
    }

    /* Копирование строки */
    while(Name[index] != 0) {
        index++;
    }
    pCMe->m_Name = (char_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, index + 1);
    index = 0;
    while(Name[index] != 0) {
        pCMe->m_Name[index] = Name[index];
        index++;
    }
    pCMe->m_Name[index] = 0;
    *copyName = pCMe->m_Name;

    return 0;
}

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoConnectionPointContainer
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_QueryInterface(/* in */ struct IEcoConnectionPointContainer* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));

    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoConnectionPointContainer) ) {
        *ppv = &pCMe->m_pVTblICPC;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoConnectionPointContainer
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_AddRef(/* in */ struct IEcoConnectionPointContainer* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));

    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoConnectionPointContainer
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_Release(/* in */ struct IEcoConnectionPointContainer* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));

    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция EnumConnectionPoints
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_EnumConnectionPoints(/* in */ struct IEcoConnectionPointContainer* me, /* out */ struct IEcoEnumConnectionPoints **ppEnum) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));
    int16_t result = 0;

    if (me == 0 || ppEnum == 0 ) {
        return -1;
    }

    result = createCEcoLab1EnumConnectionPoints((IEcoUnknown*)pCMe->m_pISys, (IEcoConnectionPoint*)pCMe->m_pISinkCP, ppEnum);

    return result;
}

/*
 *
 * <сводка>
 *   Функция FindConnectionPoint
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_IEcoConnectionPointContainer_FindConnectionPoint(/* in */ struct IEcoConnectionPointContainer* me, /* in */ const UGUID* riid, /* out */ struct IEcoConnectionPoint **ppCP) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoUnknown*));
    int16_t result = 0;

    if (me == 0 || ppCP == 0 ) {
        return -1;
    }

    if ( !IsEqualUGUID(riid, &IID_IEcoLab1Events ) ) {
        *ppCP = 0;
        /* CONNECT_E_NOCONNECTION */
        return -1;
    }

    if (pCMe->m_pISinkCP == 0) {
        /* E_FAIL */
        return -1;
    }

    ((IEcoConnectionPoint*)pCMe->m_pISinkCP)->pVTbl->AddRef((IEcoConnectionPoint*)pCMe->m_pISinkCP);
    *ppCP =  (IEcoConnectionPoint*)pCMe->m_pISinkCP;

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ struct IEcoLab1* me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Инициализация калькуляторов */
    result = initCalculators(pCMe);

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Создание точки подключения */
    result = createCEcoLab1ConnectionPoint((IEcoUnknown*)pCMe->m_pISys, (IEcoConnectionPointContainer*)&pCMe->m_pVTblICPC, &IID_IEcoLab1Events, (IEcoConnectionPoint**)&(pCMe->m_pISinkCP));
    if (result == 0 && pCMe->m_pISinkCP != 0) {
        result = 0;
    }

    /* Инициализация указателей на функции Fire */
    pCMe->Fire_OnSortStart = CEcoLab1_Fire_OnSortStart;
    pCMe->Fire_OnProcessDigit = CEcoLab1_Fire_OnProcessDigit;
    pCMe->Fire_OnElementMove = CEcoLab1_Fire_OnElementMove;
    pCMe->Fire_OnDigitComplete = CEcoLab1_Fire_OnDigitComplete;
    pCMe->Fire_OnSortComplete = CEcoLab1_Fire_OnSortComplete;

    /* Освобождение */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    return result;
}


/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
    CEcoLab1_RadixSortInt,      // Ваша реализация с событиями
    CEcoLab1_RadixSortLong,     // Заглушка
    CEcoLab1_RadixSortFloat,    // Заглушка  
    CEcoLab1_RadixSortDouble,   // Заглушка
    CEcoLab1_Addition,          // Реализация через калькулятор
    CEcoLab1_Subtraction,       // Реализация через калькулятор
    CEcoLab1_Multiplication,    // Реализация через калькулятор
    CEcoLab1_Division           // Реализация через калькулятор
};

/* Create Virtual Table IEcoConnectionPointContainer */
IEcoConnectionPointContainerVTbl g_x0000000500000000C000000000000046VTblCPC = {
    CEcoLab1_IEcoConnectionPointContainer_QueryInterface,
    CEcoLab1_IEcoConnectionPointContainer_AddRef,
    CEcoLab1_IEcoConnectionPointContainer_Release,
    CEcoLab1_IEcoConnectionPointContainer_EnumConnectionPoints,
    CEcoLab1_IEcoConnectionPointContainer_FindConnectionPoint
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;
    CEcoLab1* pCMe = 0;

    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void **)&pISys);

    /* Проверка */
    if (result != 0 || pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Проверка */
    if (result != 0 || pIBus == 0) {
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        pIBus->pVTbl->Release(pIBus);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));
    
    if (pCMe == 0) {
        pIMem->pVTbl->Release(pIMem);
        pISys->pVTbl->Release(pISys);
        pIBus->pVTbl->Release(pIBus);
        return -1;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    /* Создание таблицы функций интерфейса IEcoConnectionPointContainer */
    pCMe->m_pVTblICPC = &g_x0000000500000000C000000000000046VTblCPC;

    /* Инициализация данных */
    pCMe->m_Name = 0;
    pCMe->m_pISinkCP = 0;
    pCMe->m_pIBus = 0;
    pCMe->m_pCalculatorX = 0;
    pCMe->m_pCalculatorY = 0;
    pCMe->m_bUseAggregation = 0;
    pCMe->m_pInnerUnknown = 0;

    result = createCEcoLab1ConnectionPoint((IEcoUnknown*)pCMe->m_pISys, (IEcoConnectionPointContainer*)pCMe, &IID_IEcoLab1Events, (IEcoConnectionPoint**)&(pCMe->m_pISinkCP));
    if (result != 0 || pCMe->m_pISinkCP == 0) {
        /* Освобождение памяти в случае ошибки */
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
        pISys->pVTbl->Release(pISys);
        pIBus->pVTbl->Release(pIBus);
        return -1;
    }

    /* Инициализация указателей на функции Fire */
    pCMe->Fire_OnSortStart = CEcoLab1_Fire_OnSortStart;
    pCMe->Fire_OnProcessDigit = CEcoLab1_Fire_OnProcessDigit;
    pCMe->Fire_OnElementMove = CEcoLab1_Fire_OnElementMove;
    pCMe->Fire_OnDigitComplete = CEcoLab1_Fire_OnDigitComplete;
    pCMe->Fire_OnSortComplete = CEcoLab1_Fire_OnSortComplete;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        /* Освобождение калькуляторов */
        releaseCalculators(pCMe);
        
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        /* Освобождение точки подключения */
        if ( pCMe->m_pISinkCP != 0 ) {
            deleteCEcoLab1ConnectionPoint((IEcoConnectionPoint*)pCMe->m_pISinkCP);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}