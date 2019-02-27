// ѕеречень стандартных цветов дл€ графиков прин€тых  в компании ZETLab
#pragma once
#include "windef.h"
#include "wingdi.h"

namespace ZETColors
{
	static const COLORREF ControlColorError = RGB(255, 128, 128);	// ÷вет устанавливаемый на контрол когда возникла ошибка
	static const COLORREF ControlColorWarning = RGB(255, 255, 128);	// ÷вет устанавливаемый на контрол когда возникло предупреждение
	static const COLORREF ControlColorOk	= RGB(192, 255, 192);	// ÷вет устанавливаемый на контрол когда все хорошо

	static const COLORREF GridColor			= RGB(192, 192, 192);	// ÷вет сетки
	static const COLORREF BackgroundColor   = RGB(240, 240, 240);	// ÷вет фона
	// ѕеречень сервисных цветов (маркеры, максимумы, среднее)
	static const COLORREF CursorColor		= RGB(0,   0,   255);	// ÷вет курсора
	static const COLORREF DigitsColor		= RGB(90,  90,  90);	// ÷вет разметки осей
	static const COLORREF LegendColor		= RGB(32,  32,  192);	// ÷вет легенды
	static const COLORREF TextColor			= RGB(0, 0, 0);			// ÷вет текста по умолчанию

	static const COLORREF AddtionalWindowFon = RGB(210, 210, 210);	// ÷вет фона дополнительных окон сетки
	
#define SERVICE_COLORS_COUNT 6
	// ѕеречень сервисных цветов (маркеры, максимумы, среднее)
	static const COLORREF Service_Colors[SERVICE_COLORS_COUNT] = {
		RGB(255, 0,   0),		// красный
		RGB(0,   255, 0),		// зеленый
		RGB(0,   0,   255),		// синий
		RGB(0,   255, 255),		// фиолетовый
		RGB(255, 0,   255),		// голубой
		RGB(255, 255, 0) };		// желтый

#define GRAPH_COLORS_COUNT 32
	// ѕеречень цветов дл€ графиков
	static const COLORREF Graph_Colors[GRAPH_COLORS_COUNT] = {
		RGB(78,  80,  134),
		RGB(136, 62,  43),
		RGB(65,  136, 76),
		RGB(129, 88,  159),
		RGB(149, 94,  63),
		RGB(37,  103, 96),
		RGB(112, 61,  105),
		RGB(86,  122, 180),
		RGB(89,  86,  65),
		RGB(154, 74,  76),
		RGB(112, 78,  81),
		RGB(15,  129, 25),
		RGB(110, 60,  83),
		RGB(105, 80,  133),
		RGB(88,  102, 103),
		RGB(111, 74,  65),
		RGB(82,  91,  163),
		RGB(86,  118, 79),
		RGB(141, 83,  88),
		RGB(48,  117, 132),
		RGB(132, 58,  72),
		RGB(78,  90,  142),
		RGB(53,  91,  80),
		RGB(128, 90,  72),
		RGB(70,  74,  116),
		RGB(110, 70,  53),
		RGB(54,  120, 87),
		RGB(99,  78,  72),
		RGB(84,  96,  122),
		RGB(116, 100, 67),
		RGB(115, 116, 92),
		RGB(138, 95,  48) };
}