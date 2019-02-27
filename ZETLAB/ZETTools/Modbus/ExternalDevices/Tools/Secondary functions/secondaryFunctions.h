// Вспомогательные функции для конвертирования данных, преобразования и т.д. 
// которые могут пригодиться при работе с датчиками, стороннего производства
#pragma once
#include <afx.h>
#include <vector>

// Конвертирование строки в значение
void SF_convertCStringToChar(CString str_, char* charStr_, long charSz_);

// Конвертирование float значения в строку
CString SF_convertFloatValToCString(float val_);

// Конвертирование long значения в строку
CString SF_convertLongValToCString(long val_);

// Считываем список COM портов с описателями
// comPortNames_ - список имен COM портов
// comPortFriendlyNames_ - список легкочитаемых имен COM портов
// comPortInstanceIds_ - список уникальных идентификаторов COM портов
void SF_getCommPortsList(std::vector<CString>& comPortNames_, std::vector<CString>& comPortFriendlyNames_,
    std::vector<CString>& comPortInstanceIds_);