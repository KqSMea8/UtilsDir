#pragma once

#ifdef CPPDLL_EXPORTS
#define DLLIMPORT_EXPORT __declspec(dllexport)
#else
#define DLLIMPORT_EXPORT __declspec(dllimport)
#endif

#include <map>
#include <string>
#include <functional>
#include <list>

#include <tgbot/tgbot.h>

// ����������� ������ � UTF-8
inline std::string getUtfStr(const CString& str) { return std::string(CW2A(str, CP_UTF8)); }

//----------------------------------------------------------------------------//
// ��������� ������������ ��� ��������� ���������� �� �������� ����
__interface DLLIMPORT_EXPORT ITelegramAllerter
{
    void onAllertFromTelegram(const CString& allertMessage) = 0;
};

typedef TgBot::Message::Ptr MessagePtr;
typedef std::function<void(const MessagePtr)> CommandFunction;
//----------------------------------------------------------------------------//
__interface DLLIMPORT_EXPORT ITelegramThread
{
    // ������ ������
    void startTelegramThread(const std::map<std::string, CommandFunction>& commandsList,
                             const CommandFunction& onAnyMessageCommand = nullptr) = 0;

    // ������� �������� ��������� � ����
    void sendMessage(const std::list<int64_t>& chatIds, const CString& msg, bool disableWebPagePreview = false, int32_t replyToMessageId = 0,
                     TgBot::GenericReply::Ptr replyMarkup = std::make_shared<TgBot::GenericReply>(), const std::string& parseMode = "", bool disableNotification = false) = 0;

    // ������� �������� ��������� � ���
    void sendMessage(int64_t chatId, const CString& msg, bool disableWebPagePreview = false, int32_t replyToMessageId = 0,
                     TgBot::GenericReply::Ptr replyMarkup = std::make_shared<TgBot::GenericReply>(), const std::string& parseMode = "", bool disableNotification = false) = 0;

    // ������� ��������� �� �������� �� ����� ����� �� ������������ ����� ������
    bool isCommandListener(const std::string& text) = 0;

    // ���������� ������� ���� ����� ������ ��� ������������
    TgBot::EventBroadcaster& getBotEvents() = 0;

    // ��������� ��� ����
    const TgBot::Api& getBotApi() = 0;
};

// ������� ��������� ������ ������
DLLIMPORT_EXPORT ITelegramThread* CreateTelegramThread(const std::string& token,
                                                       ITelegramAllerter* allertInterface = nullptr);