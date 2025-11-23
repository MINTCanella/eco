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
 *   UnitTest для демонстрации работы EcoLab1 с калькуляторами
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
#include "IdEcoLab1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorC.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"
#include <stdio.h>
#include <windows.h>

/* Глобальные массивы для демонстрации компонентов */
static const UGUID* g_componentCIDs[] = {
    &CID_EcoCalculatorA,
    &CID_EcoCalculatorB, 
    &CID_EcoCalculatorC,
    &CID_EcoCalculatorD,
    &CID_EcoCalculatorE
};

static const char* g_componentNames[] = {
    "Component A (Basic X)",
    "Component B (X with Aggregation support)", 
    "Component C (X + Y)",
    "Component D (X included + Y)",
    "Component E (X aggregated + Y)"
};

static const char* g_mechanism[] = {
    "Basic",
    "Aggregation Support", 
    "Extension",
    "Inclusion",
    "Aggregation"
};

/*
 * Демонстрация всех компонентов калькуляторов
 */
void demonstrateAllCalculators(IEcoInterfaceBus1* pIBus) {
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    int16_t result = -1;
    int32_t sum = 0, mul = 0;
    int16_t diff = 0, div_result = 0;
    int i;
    
    for (i = 0; i < 5; i++) {
        printf("%d. Testing %s [%s]:\n", i+1, g_componentNames[i], g_mechanism[i]);
        
        /* Пытаемся получить IEcoCalculatorY */
        result = pIBus->pVTbl->QueryComponent(pIBus, g_componentCIDs[i], 0, &IID_IEcoCalculatorY, (void**)&pIY);
        if (result == 0 && pIY != 0) {
            /* Компонент поддерживает Y */
            mul = pIY->pVTbl->Multiplication(pIY, 5, 6);
            div_result = pIY->pVTbl->Division(pIY, 20, 4);
            printf("   Multiplication(5, 6) = %d\n", mul);
            printf("   Division(20, 4) = %d\n", div_result);
            
            /* Пытаемся получить X через QueryInterface */
            result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void**)&pIX);
            if (result == 0 && pIX != 0) {
                sum = pIX->pVTbl->Addition(pIX, 3, 7);
                printf("   Addition(3, 7) via QueryInterface = %d\n", sum);
                pIX->pVTbl->Release(pIX);
            }
            
            pIY->pVTbl->Release(pIY);
        } else {
            /* Компонент не поддерживает Y, пробуем получить X */
            result = pIBus->pVTbl->QueryComponent(pIBus, g_componentCIDs[i], 0, &IID_IEcoCalculatorX, (void**)&pIX);
            if (result == 0 && pIX != 0) {
                sum = pIX->pVTbl->Addition(pIX, 4, 9);
                diff = pIX->pVTbl->Subtraction(pIX, 15, 6);
                printf("   Addition(4, 9) = %d\n", sum);
                printf("   Subtraction(15, 6) = %d\n", diff);
                pIX->pVTbl->Release(pIX);
            } else {
                printf("   Component not available\n");
            }
        }
        printf("\n");
    }
}

/*
 * Демонстрация работы с EcoLab1 компонентом
 */
void demonstrateEcoLab1(IEcoInterfaceBus1* pIBus) {
    IEcoLab1* pIEcoLab1 = 0;
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    int16_t result = -1;
    int32_t comp_result = 0;
    int testArray[] = {170, 45, 75, 90, 2, 802, 24, 66};
    size_t arraySize = sizeof(testArray) / sizeof(testArray[0]);
    size_t i = 0;
    
    printf("\n=== DEMONSTRATING EcoLab1 COMPONENT ===\n\n");
    
    /* Получение компонента EcoLab1 */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**)&pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        printf("Error: Cannot get EcoLab1 component\n");
        return;
    }

    printf("1. Testing Calculator operations through IEcoLab1:\n");
    
    /* Используем функции калькулятора через IEcoLab1 */
    comp_result = pIEcoLab1->pVTbl->Addition(pIEcoLab1, 12, 8);
    printf("   Addition(12, 8) = %d\n", comp_result);
    
    comp_result = pIEcoLab1->pVTbl->Subtraction(pIEcoLab1, 20, 7);
    printf("   Subtraction(20, 7) = %d\n", comp_result);
    
    comp_result = pIEcoLab1->pVTbl->Multiplication(pIEcoLab1, 6, 9);
    printf("   Multiplication(6, 9) = %d\n", comp_result);
    
    comp_result = pIEcoLab1->pVTbl->Division(pIEcoLab1, 45, 5);
    printf("   Division(45, 5) = %d\n", comp_result);

    printf("\n2. Testing QueryInterface mechanism:\n");
    
    /* Тестируем QueryInterface */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        printf("   IEcoLab1 -> IEcoCalculatorX [SUCCESS]\n");
        comp_result = pIX->pVTbl->Addition(pIX, 2, 3);
        printf("   Addition(2, 3) via IX = %d\n", comp_result);
        pIX->pVTbl->Release(pIX);
    } else {
        printf("   IEcoLab1 -> IEcoCalculatorX [FAILED]\n");
    }
    
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {
        printf("   IEcoLab1 -> IEcoCalculatorY [SUCCESS]\n");
        comp_result = pIY->pVTbl->Multiplication(pIY, 4, 5);
        printf("   Multiplication(4, 5) via IY = %d\n", comp_result);
        pIY->pVTbl->Release(pIY);
    } else {
        printf("   IEcoLab1 -> IEcoCalculatorY [FAILED]\n");
    }

    printf("\n3. Testing Radix Sort functionality:\n");
    
    printf("   Before sort: ");
    for (i = 0; i < arraySize; i++) {
        printf("%d ", testArray[i]);
    }
    printf("\n");
    
    pIEcoLab1->pVTbl->RadixSortInt(pIEcoLab1, testArray, arraySize);
    
    printf("   After sort: ");
    for (i = 0; i < arraySize; i++) {
        printf("%d ", testArray[i]);
    }
    printf("\n");

    pIEcoLab1->pVTbl->Release(pIEcoLab1);
}

/*
 * Демонстрация QueryInterface между разными интерфейсами
 */
void demonstrateCrossQueryInterface(IEcoInterfaceBus1* pIBus) {
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    IEcoLab1* pIEcoLab1 = 0;
    int16_t result = -1;
    
    printf("=== DEMONSTRATING CROSS-QUERYINTERFACE ===\n\n");
    
    /* Получаем компонент E (включение) */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {
        printf("1. From IEcoCalculatorY (E):\n");
        
        /* Y -> X */
        result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void**)&pIX);
        if (result == 0 && pIX != 0) {
            printf("   IEcoCalculatorY -> IEcoCalculatorX [SUCCESS]\n");
            pIX->pVTbl->Release(pIX);
        } else {
            printf("   IEcoCalculatorY -> IEcoCalculatorX [FAILED]\n");
        }
        
        /* Y -> IEcoLab1 (не должен работать) */
        result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoLab1, (void**)&pIEcoLab1);
        if (result == 0 && pIEcoLab1 != 0) {
            printf("   IEcoCalculatorY -> IEcoLab1 [SUCCESS]\n");
            pIEcoLab1->pVTbl->Release(pIEcoLab1);
        } else {
            printf("   IEcoCalculatorY -> IEcoLab1 [FAILED - as expected]\n");
        }
        
        pIY->pVTbl->Release(pIY);
    }
    
    /* Получаем компонент B (агрегирование) */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, 0, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        printf("\n2. From IEcoCalculatorX (B - Aggregation):\n");
        
        /* X -> Y (должен провалиться для B) */
        result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void**)&pIY);
        if (result == 0 && pIY != 0) {
            printf("   IEcoCalculatorX -> IEcoCalculatorY [SUCCESS]\n");
            pIY->pVTbl->Release(pIY);
        } else {
            printf("   IEcoCalculatorX -> IEcoCalculatorY [FAILED - as expected for B]\n");
        }
        
        /* X -> IEcoLab1 (не должен работать) */
        result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoLab1, (void**)&pIEcoLab1);
        if (result == 0 && pIEcoLab1 != 0) {
            printf("   IEcoCalculatorX -> IEcoLab1 [SUCCESS]\n");
            pIEcoLab1->pVTbl->Release(pIEcoLab1);
        } else {
            printf("   IEcoCalculatorX -> IEcoLab1 [FAILED - as expected]\n");
        }
        
        pIX->pVTbl->Release(pIX);
    }
    
    /* Получаем компонент A (включение) */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        printf("\n3. From IEcoCalculatorX (A - Inclusion):\n");
        
        /* X -> Y (должен провалиться для A) */
        result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void**)&pIY);
        if (result == 0 && pIY != 0) {
            printf("   IEcoCalculatorX -> IEcoCalculatorY [SUCCESS]\n");
            pIY->pVTbl->Release(pIY);
        } else {
            printf("   IEcoCalculatorX -> IEcoCalculatorY [FAILED - as expected for A]\n");
        }
        
        pIX->pVTbl->Release(pIX);
    }
}

/*
 * Демонстрация механизма агрегирования
 */
void demonstrateAggregation(IEcoInterfaceBus1* pIBus) {
    IEcoLab1* pIEcoLab1 = 0;
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    IEcoCalculatorX* pIX1 = 0;
    IEcoCalculatorX* pIX2 = 0;
    int16_t result = -1;
    int32_t sum = 0;
    int32_t mul = 0;
    
    printf("\n=== DEMONSTRATING AGGREGATION MECHANISM ===\n\n");
    
    /* Создаем EcoLab1 с агрегированием */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**)&pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        printf("Error: Cannot get EcoLab1 component\n");
        return;
    }
    
    printf("Testing aggregation scenarios:\n");
    
    /* Тестируем получение интерфейсов через EcoLab1 */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        printf("1. IEcoLab1 -> IEcoCalculatorX [SUCCESS]\n");
        
        /* Проверяем, что интерфейс работает */
        sum = pIX->pVTbl->Addition(pIX, 10, 5);
        printf("   Addition(10, 5) = %d\n", sum);
        
        pIX->pVTbl->Release(pIX);
    } else {
        printf("1. IEcoLab1 -> IEcoCalculatorX [FAILED]\n");
    }
    
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {
        printf("2. IEcoLab1 -> IEcoCalculatorY [SUCCESS]\n");
        
        /* Проверяем, что интерфейс работает */
        mul = pIY->pVTbl->Multiplication(pIY, 4, 3);
        printf("   Multiplication(4, 3) = %d\n", mul);
        
        pIY->pVTbl->Release(pIY);
    } else {
        printf("2. IEcoLab1 -> IEcoCalculatorY [FAILED]\n");
    }
    
    printf("\n3. Testing reference counting with aggregation:\n");
    
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX1);
    if (result == 0 && pIX1 != 0) {
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX2);
        if (result == 0 && pIX2 != 0) {
            printf("   Multiple QueryInterface calls successful\n");
            printf("   Both interfaces point to same object: %s\n", 
                   (pIX1 == pIX2) ? "YES" : "NO");
            pIX2->pVTbl->Release(pIX2);
        }
        pIX1->pVTbl->Release(pIX1);
    }
    
    pIEcoLab1->pVTbl->Release(pIEcoLab1);
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
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;

    printf("=== ECO LAB2 ===\n\n");

    /* Получение системных интерфейсов */
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void**)&pISys);
    if (result != 0 || pISys == 0) {
        printf("Error: Cannot get system interface\n");
        return result;
    }
    
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == 0) {
        printf("Error: Cannot get interface bus\n");
        pISys->pVTbl->Release(pISys);
        return result;
    }

#ifdef ECO_LIB
    printf("Registering components...\n");
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0) printf("  EcoLab1 registration: %d\n", result);
    
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorA, (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0) printf("  CalculatorA registration: %d\n", result);
    
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorB, (IEcoUnknown*)GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565);
    if (result != 0) printf("  CalculatorB registration: %d\n", result);
    
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorC, (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0) printf("  CalculatorC registration: %d\n", result);
    
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorD, (IEcoUnknown*)GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61);
    if (result != 0) printf("  CalculatorD registration: %d\n", result);
    
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorE, (IEcoUnknown*)GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0);
    if (result != 0) printf("  CalculatorE registration: %d\n", result);
    printf("\n");
#endif

    /* Демонстрации */
    demonstrateAllCalculators(pIBus);
    demonstrateCrossQueryInterface(pIBus);
    demonstrateAggregation(pIBus);
    demonstrateEcoLab1(pIBus);

    /* Освобождение ресурсов */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }
    
    printf("\nPress any key to exit...\n");
    system("pause");
    
    return 0;
}