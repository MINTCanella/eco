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
static int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ IEcoLab1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
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
static uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
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
static uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
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
 *   Функция MyFunction
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
/* --- Для int --- */
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

/* Поразрядная сортировка для int */
static void ECOCALLMETHOD CEcoLab1_RadixSortInt(/* in */ IEcoLab1Ptr_t me, /* in */ int32_t* array, /* in */ size_t size) {
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

/* Вспомогательная функция для int64_t */
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

/* Поразрядная сортировка для int64_t */
static void ECOCALLMETHOD CEcoLab1_RadixSortLong(/* in */ IEcoLab1Ptr_t me, /* in */ int64_t* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int64_t max, min;
    int64_t* output;
    int count[256];  // 256 корзин для одного байта
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

/* Поразрядная сортировка для битовых представлений int64_t*/
static void ECOCALLMETHOD CEcoLab1_RadixSortInt64Bits(/* in */ IEcoLab1Ptr_t me, /* in */ int64_t* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int64_t* output;
    int count[256];
    int i;
    size_t j;
    int shift;
    int index;
    int digit;

    if (size <= 1) return;

    output = (int64_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int64_t));
    if (output == 0) return;

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
    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, output);
}

/* Поразрядная сортировка для битовых представлений int32_t*/
static void ECOCALLMETHOD CEcoLab1_RadixSortInt32Bits(/* in */ IEcoLab1Ptr_t me, /* in */ int32_t* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int32_t* output;
    int count[256];
    int i;
    size_t j;
    int shift;
    int index;
    int digit;

    if (size <= 1) return;

    output = (int32_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int32_t));
    if (output == 0) return;

    for (shift = 0; shift < 32; shift += 8) {
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
    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, output);
}

/* --- Для float --- */
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

static void ECOCALLMETHOD CEcoLab1_RadixSortFloat(/* in */ IEcoLab1Ptr_t me, /* in */ float* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int32_t* tempArray;
    size_t i;

    if (size <= 1) return;

    tempArray = (int32_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int32_t));
    if (tempArray == 0) return;

    for (i = 0; i < size; i++) {
        tempArray[i] = floatToSortableInt(array[i]);
    }

    CEcoLab1_RadixSortInt32Bits(me, tempArray, size);

    for (i = 0; i < size; i++) {
        array[i] = sortableIntToFloat(tempArray[i]);
    }

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, tempArray);
}

/* --- Для double --- */
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

static void ECOCALLMETHOD CEcoLab1_RadixSortDouble(/* in */ IEcoLab1Ptr_t me, /* in */ double* array, /* in */ size_t size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int64_t* tempArray;
    size_t i;

    if (size <= 1) return;

    tempArray = (int64_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int64_t));
    if (tempArray == 0) return;

    for (i = 0; i < size; i++) {
        tempArray[i] = doubleToSortableInt(array[i]);
    }

    CEcoLab1_RadixSortInt64Bits(me, tempArray, size);

    for (i = 0; i < size; i++) {
        array[i] = sortableIntToDouble(tempArray[i]);
    }

    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, tempArray);
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
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ IEcoLab1Ptr_t me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

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

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    /* Инициализация данных */
    pCMe->m_Name = 0;

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
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
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
