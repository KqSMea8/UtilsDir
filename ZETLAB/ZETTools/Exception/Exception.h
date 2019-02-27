#pragma once

class Exception
{
public:
	Exception(unsigned int err) : m_Error(err) {};
	virtual ~Exception(void) {};

	unsigned int GetErrorCode() {return m_Error;};

protected:
	unsigned int	m_Error;			//Номер ошибки исключения
};