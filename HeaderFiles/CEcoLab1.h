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
 *   Данный заголовок описывает реализацию компонента CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECOLAB1_H__
#define __C_ECOLAB1_H__

#include "IEcoLab1.h"
#include "IEcoLab1Events.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoEnumConnections.h"
#include "IEcoConnectionPointContainer.h"
#include "CEcoLab1ConnectionPoint.h"

typedef struct CEcoLab1 {
    /* Таблица функций интерфейса IEcoLab1 */
    IEcoLab1VTbl* m_pVTblIEcoLab1;

    /* Таблица функций интерфейса IEcoConnectionPointContainer */
    IEcoConnectionPointContainerVTbl* m_pVTblICPC;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

    /* Интерфейсная шина */
    IEcoInterfaceBus1* m_pIBus;

    /* Внешний объект для агрегирования */
    IEcoUnknown* m_pIUnkOuter;

    /* Точка подключения для событий */
    CEcoLab1ConnectionPoint* m_pISinkCP;

    /* Данные экземпляра */
    char_t* m_Name;

    /* Включенные компоненты калькулятора */
    IEcoCalculatorX* m_pCalculatorX;
    IEcoCalculatorY* m_pCalculatorY;

    /* Флаг использования агрегирования */
    uint8_t m_bUseAggregation;

    /* Агрегированный компонент B */
    IEcoUnknown* m_pInnerUnknown;

    /* Неделегирующий Unknown */
    IEcoUnknownVTbl* m_pVTblINondelegatingUnk;

    /* Добавили функции Fire для всех событий радиксной сортировки */
    /* Вспомогательные функции для уведомлений */
    int16_t (*Fire_OnSortStart)(/* in */ struct CEcoLab1* me, /* in */ size_t arraySize, /* in */ int32_t maxValue);
    int16_t (*Fire_OnProcessDigit)(/* in */ struct CEcoLab1* me, /* in */ int32_t digitPosition, /* in */ int32_t divisor);
    int16_t (*Fire_OnElementMove)(/* in */ struct CEcoLab1* me, /* in */ int32_t value, /* in */ size_t fromIndex, /* in */ size_t toIndex);
    int16_t (*Fire_OnDigitComplete)(/* in */ struct CEcoLab1* me, /* in */ int32_t divisor);
    int16_t (*Fire_OnSortComplete)(/* in */ struct CEcoLab1* me);

} CEcoLab1, *CEcoLab1Ptr;

/* Инициализация экземпляра */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ struct IEcoLab1* me, /* in */ IEcoUnknown *pIUnkSystem);
/* Создание экземпляра */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1);
/* Удаление */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1);

#endif /* __C_ECOLAB1_H__ */