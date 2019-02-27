#pragma once

#include <assert.h>
#include <future>
#include <map>
#include <memory>
#include <set>
#include <windows.h>
#include "Singleton.h"

//*****************************************************************************
struct GUIDComparer
{
	bool operator()(const GUID & Left, const GUID & Right) const
	{
		// comparison logic goes here
		return memcmp(&Left, &Right, sizeof(Right)) < 0;
	}
};
//*****************************************************************************
// структура передаваемая в сообщении
__interface IMessageData { };
// класс получающий сообщение
class IMessagesRecipient;
//*****************************************************************************
// класс отправки сообщений
class CMassages
{
	friend class CSingleton<CMassages>;
public://----------------------------------------------------------------------
	// подписка/отписка на события
	void subscribe(const GUID& code, IMessagesRecipient* recipient);
	void unsubscribe(const GUID& code, IMessagesRecipient* recipient);
	//-------------------------------------------------------------------------
	// отправка сообщений
	void sendMessage(const GUID& code, float massageValue = 0,
					 std::shared_ptr<IMessageData> messageData = nullptr);
	void postMessage(const GUID& code, float massageValue = 0,
					 std::shared_ptr<IMessageData> messageData = nullptr);
private://---------------------------------------------------------------------
	std::map<GUID, std::set<IMessagesRecipient*>, GUIDComparer> m_messageRecipietns;
};

//*****************************************************************************
class IMessagesRecipient
{
public:
	IMessagesRecipient() {};
	virtual ~IMessagesRecipient() {};
protected:
	void subscribe(const GUID& code);		// подписывание на события
	void unsubscribe(const GUID& code);		// отписка от события
public:
	virtual void onMessageEvent(const GUID& code, float massageValue, 
								std::shared_ptr<IMessageData> messageData) = 0;
};

//*****************************************************************************
inline void CMassages::subscribe(const GUID& code, IMessagesRecipient* recipient)
{
	auto it = m_messageRecipietns.find(code);
	if (it != m_messageRecipietns.end())
		it->second.insert(recipient);
	else
		m_messageRecipietns.insert(std::make_pair(code, std::set<IMessagesRecipient*>({ recipient })));
}

inline void CMassages::unsubscribe(const GUID& code, IMessagesRecipient* recipient)
{
	auto it = m_messageRecipietns.find(code);
	if (it != m_messageRecipietns.end())
	{
		it->second.erase(recipient);
		if (it->second.empty())
			m_messageRecipietns.erase(it);
	}		
}

//-----------------------------------------------------------------------------
inline void CMassages::sendMessage(const GUID& code, float massageValue /*= 0*/,
								   std::shared_ptr<IMessageData> messageData /*= nullptr*/)
{
	auto it = m_messageRecipietns.find(code);
	if (it != m_messageRecipietns.end())
	{
		for (auto &recipient : it->second)
		{
			recipient->onMessageEvent(code, massageValue, messageData);
		}
	}
	else
		assert(false);
}

inline void CMassages::postMessage(const GUID& code, float massageValue /*= 0*/,
								   std::shared_ptr<IMessageData> messageData /*= nullptr*/)
{
	auto messageRecipients = m_messageRecipietns.find(code);
	if (messageRecipients != m_messageRecipietns.end())
	{
		std::async([messageRecipients, code, massageValue, messageData]()
		{
			for (auto &recipient : messageRecipients->second)
				recipient->onMessageEvent(code, massageValue, messageData);
		});
	}
	else
		assert(false);
}

//*****************************************************************************
inline void IMessagesRecipient::subscribe(const GUID& code)
{
	get_service<CMassages>().subscribe(code, this);
}

inline void IMessagesRecipient::unsubscribe(const GUID& code)
{
	get_service<CMassages>().unsubscribe(code, this);
}
//*****************************************************************************