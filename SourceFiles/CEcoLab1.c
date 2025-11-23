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

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"
#include <stdio.h>

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

/* Неделегирующий Unknown */
static int16_t ECOCALLMETHOD CEcoLab1_NondelegatingQueryInterface(/* in */ IEcoUnknown* me, /* in */ const UGUID* riid, /* out */ void** ppv);
static uint32_t ECOCALLMETHOD CEcoLab1_NondelegatingAddRef(/* in */ IEcoUnknown* me);
static uint32_t ECOCALLMETHOD CEcoLab1_NondelegatingRelease(/* in */ IEcoUnknown* me);

/* Таблица для неделегирующего Unknown */
IEcoUnknownVTbl g_NondelegatingUnknownVTbl = {
    CEcoLab1_NondelegatingQueryInterface,
    CEcoLab1_NondelegatingAddRef,
    CEcoLab1_NondelegatingRelease
};

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
                                                 (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk,
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

/* Функции калькулятора */
static int32_t ECOCALLMETHOD CEcoLab1_Addition(/* in */ IEcoLab1* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (pCMe->m_pCalculatorX != 0) {
        return pCMe->m_pCalculatorX->pVTbl->Addition(pCMe->m_pCalculatorX, a, b);
    }
    return 0;
}

static int16_t ECOCALLMETHOD CEcoLab1_Subtraction(/* in */ IEcoLab1* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (pCMe->m_pCalculatorX != 0) {
        return pCMe->m_pCalculatorX->pVTbl->Subtraction(pCMe->m_pCalculatorX, a, b);
    }
    return 0;
}

static int32_t ECOCALLMETHOD CEcoLab1_Multiplication(/* in */ IEcoLab1* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (pCMe->m_pCalculatorY != 0) {
        return pCMe->m_pCalculatorY->pVTbl->Multiplication(pCMe->m_pCalculatorY, a, b);
    }
    return 0;
}

static int16_t ECOCALLMETHOD CEcoLab1_Division(/* in */ IEcoLab1* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (pCMe->m_pCalculatorY != 0) {
        return pCMe->m_pCalculatorY->pVTbl->Division(pCMe->m_pCalculatorY, a, b);
    }
    return 0;
}

/*
 * Неделегирующий QueryInterface
 */
static int16_t ECOCALLMETHOD CEcoLab1_NondelegatingQueryInterface(/* in */ IEcoUnknown* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - offsetof(CEcoLab1, m_pVTblINondelegatingUnk));

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) || 
         IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        /* Для агрегирования - делегируем запрос агрегированному компоненту */
        if (pCMe->m_bUseAggregation && pCMe->m_pInnerUnknown != 0) {
            return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
        }
        /* Для включения - возвращаем интерфейс внутреннего компонента */
        else if (!pCMe->m_bUseAggregation && pCMe->m_pCalculatorX != 0) {
            return pCMe->m_pCalculatorX->pVTbl->QueryInterface(pCMe->m_pCalculatorX, riid, ppv);
        }
        else {
            *ppv = 0;
            return ERR_ECO_NOINTERFACE;
        }
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        /* Для включения - возвращаем интерфейс внутреннего компонента */
        if (!pCMe->m_bUseAggregation && pCMe->m_pCalculatorY != 0) {
            return pCMe->m_pCalculatorY->pVTbl->QueryInterface(pCMe->m_pCalculatorY, riid, ppv);
        }
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

static uint32_t ECOCALLMETHOD CEcoLab1_NondelegatingAddRef(/* in */ IEcoUnknown* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - offsetof(CEcoLab1, m_pVTblINondelegatingUnk));
    if (me == 0) {
        return -1;
    }
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoLab1_NondelegatingRelease(/* in */ IEcoUnknown* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint8_t*)me - offsetof(CEcoLab1, m_pVTblINondelegatingUnk));
    
    if (me == 0) {
        return -1;
    }
    
    --pCMe->m_cRef;
    
    if (pCMe->m_cRef == 0) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 * Делегирующий QueryInterface
 */
static int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ IEcoLab1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pIUnkOuter != 0 && pCMe->m_pIUnkOuter != (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk) {
        return pCMe->m_pIUnkOuter->pVTbl->QueryInterface(pCMe->m_pIUnkOuter, riid, ppv);
    }

    return CEcoLab1_NondelegatingQueryInterface((IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk, riid, ppv);
}

/*
 * Функция AddRef
 */
static uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Если есть внешний компонент - делегируем ему */
    if (pCMe->m_pIUnkOuter != 0 && pCMe->m_pIUnkOuter != (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk) {
        return pCMe->m_pIUnkOuter->pVTbl->AddRef(pCMe->m_pIUnkOuter);
    }
    
    return CEcoLab1_NondelegatingAddRef((IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk);
}

/*
 * Функция Release
 */
static uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Если есть внешний компонент - делегируем ему */
    if (pCMe->m_pIUnkOuter != 0 && pCMe->m_pIUnkOuter != (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk) {
        return pCMe->m_pIUnkOuter->pVTbl->Release(pCMe->m_pIUnkOuter);
    }
    
    return CEcoLab1_NondelegatingRelease((IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk);
}

/* Radix Sort функции */
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

static void ECOCALLMETHOD CEcoLab1_RadixSortInt(/* in */ IEcoLab1* me, /* in */ int32_t* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int32_t max;
    int32_t* output;
    int count[10];
    int i;
    size_t j;
    int exp;
    int index;

    if (size <= 1) return;

    max = getMaxInt32(array, size);

    output = (int32_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int32_t));
    if (output == 0) return;

    for (exp = 1; max / exp > 0; exp *= 10) {
        for (i = 0; i < 10; i++) count[i] = 0;

        for (j = 0; j < size; j++) count[(array[j] / exp) % 10]++;

        for (i = 1; i < 10; i++) count[i] += count[i - 1];

        for (j = size; j > 0; j--) {
            index = j - 1;
            output[count[(array[index] / exp) % 10] - 1] = array[index];
            count[(array[index] / exp) % 10]--;
        }

        for (j = 0; j < size; j++) array[j] = output[j];
    }

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, output);
}

static int64_t getMaxInt64(int64_t* array, size_t size) {
    int64_t max = array[0];
    size_t i;
    for (i = 1; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

static void ECOCALLMETHOD CEcoLab1_RadixSortLong(/* in */ IEcoLab1* me, /* in */ int64_t* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int64_t max, min;
    int64_t* output;
    int count[256];
    int i;
    size_t j;
    int shift;
    int64_t offset = 0;
    int index;
    int digit;

    if (size <= 1) return;

    max = min = array[0];
    for (j = 1; j < size; j++) {
        if (array[j] > max) max = array[j];
        if (array[j] < min) min = array[j];
    }

    if (min < 0) {
        offset = -min;
        for (j = 0; j < size; j++) {
            array[j] += offset;
        }
        max += offset;
    }

    output = (int64_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int64_t));
    if (output == 0) {
        if (min < 0) {
            for (j = 0; j < size; j++) {
                array[j] -= offset;
            }
        }
        return;
    }

    for (shift = 0; shift < 64; shift += 8) {
        for (i = 0; i < 256; i++) count[i] = 0;

        for (j = 0; j < size; j++) count[(array[j] >> shift) & 0xFF]++;

        for (i = 1; i < 256; i++) count[i] += count[i - 1];

        for (j = size; j > 0; j--) {
            index = j - 1;
            digit = (array[index] >> shift) & 0xFF;
            output[count[digit] - 1] = array[index];
            count[digit]--;
        }

        for (j = 0; j < size; j++) array[j] = output[j];
    }

    if (min < 0) {
        for (j = 0; j < size; j++) {
            array[j] -= offset;
        }
    }

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, output);
}

/* Вспомогательные функции для float/double */
static int32_t floatToSortableInt(float f) {
    int32_t bits = *(int32_t*)&f;
    if (bits < 0) {
        return ~bits;
    } else {
        return bits ^ 0x80000000;
    }
}

static float sortableIntToFloat(int32_t bits) {
    if (bits & 0x80000000) {
        bits = bits ^ 0x80000000;
    } else {
        bits = ~bits;
    }
    return *(float*)&bits;
}

static void ECOCALLMETHOD CEcoLab1_RadixSortFloat(/* in */ IEcoLab1* me, /* in */ float* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int32_t* tempArray;
    size_t i;

    if (size <= 1) return;

    tempArray = (int32_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int32_t));
    if (tempArray == 0) return;

    for (i = 0; i < size; i++) {
        tempArray[i] = floatToSortableInt(array[i]);
    }

    CEcoLab1_RadixSortInt(me, tempArray, size);

    for (i = 0; i < size; i++) {
        array[i] = sortableIntToFloat(tempArray[i]);
    }

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, tempArray);
}

static int64_t doubleToSortableInt(double d) {
    int64_t bits = *(int64_t*)&d;
    if (bits < 0) {
        return ~bits;
    } else {
        return bits ^ 0x8000000000000000;
    }
}

static double sortableIntToDouble(int64_t bits) {
    if (bits & 0x8000000000000000) {
        bits = bits ^ 0x8000000000000000;
    } else {
        bits = ~bits;
    }
    return *(double*)&bits;
}

static void ECOCALLMETHOD CEcoLab1_RadixSortDouble(/* in */ IEcoLab1* me, /* in */ double* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int64_t* tempArray;
    size_t i;

    if (size <= 1) return;

    tempArray = (int64_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int64_t));
    if (tempArray == 0) return;

    for (i = 0; i < size; i++) {
        tempArray[i] = doubleToSortableInt(array[i]);
    }

    CEcoLab1_RadixSortLong(me, tempArray, size);

    for (i = 0; i < size; i++) {
        array[i] = sortableIntToDouble(tempArray[i]);
    }

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, tempArray);
}

/*
 * Функция Init
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ IEcoLab1* me, /* in */ IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Инициализация калькуляторов */
    result = initCalculators(pCMe);

    return result;
}

/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
    CEcoLab1_RadixSortInt,
    CEcoLab1_RadixSortLong,
    CEcoLab1_RadixSortFloat,
    CEcoLab1_RadixSortDouble,
    CEcoLab1_Addition,
    CEcoLab1_Subtraction,
    CEcoLab1_Multiplication,
    CEcoLab1_Division
};

/*
 * Функция Create
 */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Сохранение указателя на внешний объект для агрегирования */
    pCMe->m_pIUnkOuter = (pIUnkOuter != 0) ? pIUnkOuter : (IEcoUnknown*)pCMe;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    /* Инициализация неделегирующего Unknown */
    pCMe->m_pVTblINondelegatingUnk = &g_NondelegatingUnknownVTbl;

    /* Настройка агрегации */
    pCMe->m_pIUnkOuter = (pIUnkOuter != 0) ? pIUnkOuter : (IEcoUnknown*)&pCMe->m_pVTblINondelegatingUnk;

    /* Инициализация данных */
    pCMe->m_Name = 0;
    pCMe->m_pIBus = 0;
    pCMe->m_pCalculatorX = 0;
    pCMe->m_pCalculatorY = 0;
    pCMe->m_bUseAggregation = 0;
    pCMe->m_pInnerUnknown = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 * Функция Delete
 */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        /* Освобождение калькуляторов */
        releaseCalculators(pCMe);
        
        /* Освобождение агрегированного компонента */
        if (pCMe->m_pInnerUnknown != 0) {
            pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown);
            pCMe->m_pInnerUnknown = 0;
        }
        
        pIMem = pCMe->m_pIMem;
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
