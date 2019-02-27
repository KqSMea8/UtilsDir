#pragma once

// Данная библиотека формирует HTML документ, в котором используются Яндекс Карты.
// Алгоритм работы с Яндекс Картами через эту DLL:
//     1. Проинициализировать шаблонный HTML документ с Яндекс Картами (zyapiInitMap)
//     2. Добавить бабл на карту (zyapiAddCustomBubleOnMap)
//     3. Отобразить карту (zyapiShowMap)
//     4. При необходимости удалить созданный файл (zyapiCloseMap)

#include <string>

#ifdef ZETMAPSAPI_EXPORTS
    // Данный дефайн определен только в текущей DLL, функции подготавливаются к экспорту
    #define ZETMAPSAPI_API _declspec(dllexport)
#else
    // Данный дефайн не определен в проектах использующих данную DLL, функции подготавливаются к импорту
    #define ZETMAPSAPI_API _declspec(dllimport)
#endif

class ZETMAPSAPI_API CZetMapsAPI
{
private:
    class Impl;

    Impl *m_iImpl = nullptr;
public:
    CZetMapsAPI();
    ~CZetMapsAPI();

    // Функция для инициализации библиотеки
    // Входной аргумент: Адрес по которому будет распологаться сформированный HTML документ, при пустом значении, файл будет сформирован в директории по умолчанию
    // Выходной аргумент - адрес по которому сформировался документ, пустое значение - ошибка
    std::wstring Init(std::wstring htmlPath_ = L"");

    // Функция для добавления бабла на карте
    // Входные аргументы:
    //                    latitude_    - широта
    //                    longitude_   - долгота
    //                    hintText_    - всплывающий текст
    //                    captionText_ - текст, отображаемый рядом с баблом
    void AddCustomBubble(double latitude_, double longitude_,
        std::wstring hintText_, std::wstring captionText_);

    // Функция для добавления линии на карте
    // Входные аргументы:
    //                    latitudeStart_    - широта начала линии
    //                    longitudeStart_   - долгота начала линии
    //                    latitudeEnd_      - широта конца линии
    //                    longitudeEnd_     - долгота конца линии
    //                    hintText_    - всплывающий текст
    void AddCustomLine(double latitudeStart_, double longitudeStart_,
        double latitudeEnd_, double longitudeEnd_, std::wstring hintText_);
    void AddCustomLines(double* latitude_, double* longitude_, unsigned long coordCount_, std::wstring hintText_);

    // Отображаем сформированную карту
    void Show();

    // Деинициализируем экземпляр
    void Close();
};