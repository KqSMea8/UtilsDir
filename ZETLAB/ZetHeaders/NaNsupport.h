#pragma once
//проверка на Не Число и бесконечность
inline bool ZETIsNaN(float x)
{
	union {
        float value;
        struct {
            unsigned int frac : 23;
            unsigned int exp  : 8;
            unsigned int sign : 1;
        } fields;
    } conv;
    conv.value = x;
	return conv.fields.exp == 0xff;
}

inline bool ZETIsNaN(double x)
{
    union {
        double value;
        struct {
            unsigned long long frac : 52;
            unsigned long long exp  : 11;
            unsigned long long sign : 1;
        } fields;
    } conv;
    conv.value = x;
    return conv.fields.exp == 0x7ff;
}

//inline bool ZETIsNaN(double x)
//{
//	return _isnan(x) || !_isfinite(x);
//}

//возвращает Не Число
inline float ZETGetNaN()
{/*
    float v = 0.f;
    return 0.f/v;
}*/
// новая версия функции не даёт предупреждений при компиляции
	union
	{
		float value;
		struct
		{
			unsigned : 23;
			unsigned exp  : 8;
		} fields;
	} conv;
	conv.value = 0;
	conv.fields.exp = 0xff;
	return conv.value;
}
