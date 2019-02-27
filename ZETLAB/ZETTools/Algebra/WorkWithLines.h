//------------------------------------------------------------------------------
// Швблонный класс предназначен для работы с линиями вида
//		y = k * x + b
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
template <class T>				// T - это float, double
struct PointForLine
{
	T x;
	T y;
	PointForLine()
	{
		x = y = 0;
	}
	PointForLine(T _x, T _y)
	{
		x = _x;
		y = _y;
	}
};
//------------------------------------------------------------------------------
template <class T>				// T - это float, double
struct Line
{
	T k;
	T b;
	Line()
	{
		k = b = 0;
	}
	Line(T _k, T _b)
	{
		k = _k;
		b = _b;
	}
};
//------------------------------------------------------------------------------
template <class T>				// T - это float, double
class CWorkWithLines
{
public:
	CWorkWithLines() {}
	~CWorkWithLines() {}

	bool CrossTwoLine(Line<T> &line1, Line<T> &line2, PointForLine<T> &point);
	void DistanceBetweenPoints(
		PointForLine<T> &point1, PointForLine<T> &point2,  T &distance);
	bool NormalToLineFromPoint(
		Line<T> &line, PointForLine<T> &point, Line<T> &normal);
	void DistanceFromPointToLine(
		PointForLine<T> &point, Line<T> &line, T &distance);
};
//------------------------------------------------------------------------------
// точка, в которой пересекаются две прямые
template <class T>				// T - это float, double
bool CWorkWithLines<T>::CrossTwoLine(
	Line<T> &line1,
	Line<T> &line2,
	PointForLine<T> &point)
{
	T det = line2.k - line1.k;
	bool ret = det != (T)0;		// IsNotEqual нельзя
	if (ret)
	{
		point.x = (line2.b - line1.b) / det;
		point.y = (line2.k * line1.b - line1.k * line2.b) / det;
	}
	return ret;
}
//------------------------------------------------------------------------------
// расстояния между двумя точками
template <class T>				// T - это float, double
void CWorkWithLines<T>::DistanceBetweenPoints(
	PointForLine<T> &point1,
	PointForLine<T> &point2,
	T &distance)
{
	T x = point2.x - point1.x;
	T y = point2.y - point1.y;
	distance = sqrt(x * x + y * y);
}
//------------------------------------------------------------------------------
// нормаль к линии, проходящей через точку (нормаль проходит через точку)
template <class T>				// T - это float, double
bool CWorkWithLines<T>::NormalToLineFromPoint(
	Line<T> &line,
	PointForLine<T> &point,
	Line<T> &normal)
{
	bool ret = line.k != (T)0;		// IsNotEqual нельзя
	if (ret)
	{
		normal.k = -1 / line.k;
		normal.b = point.y - normal.k * point.x;
	}
	return ret;
}
//------------------------------------------------------------------------------
// минимальное расстояние от точки до прямой
template <class T>				// T - это float, double
void CWorkWithLines<T>::DistanceFromPointToLine(
	PointForLine<T> &point,
	Line<T> &line,
	T &distance)
{
	if (line.k != (T)0)		// IsNotEqual нельзя
	{// линия под углом к оси х
		Line<T> normal;
		PointForLine<T> cross;
		NormalToLineFromPoint(line, point, normal);
		CrossTwoLine(line, normal, cross);
		DistanceBetweenPoints(cross, point, distance);
	}
	else
	{// линия параллельна оси х
		distance = fabs(point.y - line.b);
	}
}
//------------------------------------------------------------------------------