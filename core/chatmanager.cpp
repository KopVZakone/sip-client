#include "chatmanager.h"
#include <pjsua2.hpp>
#include "accountsmanager.h"
ChatManager &ChatManager::instance()
{
    static ChatManager inst;
    return inst;
}

ChatListModel *ChatManager::chatList() const
{
    return m_chatListModel.get();
}

ChatProxyModel *ChatManager::currentChat() const
{
    return m_currentChatProxy.get();
}

void ChatManager::receiveMessage(const QString &remoteNumber, const QString &text)
{
    auto isMessageRead {m_currentChatProxy->filterNumber() == remoteNumber};
    m_messagesModel->saveMessage(remoteNumber, text, false, isMessageRead);
    QMetaObject::invokeMethod(m_chatListModel.get(), "refresh", Qt::QueuedConnection);
}
void ChatManager::sendMessageInCurrentChat(const QString& text)
{
    auto remoteNumber {m_currentChatProxy->filterNumber() };
    auto account { AccountsManager::instance().getSelectedAccount() };
    if(!account)
        return;
    auto accountDomain {account->getDomain()};
    QString remoteUri = QString("sip:%1@%2").arg(remoteNumber, accountDomain);
    pj::SendInstantMessageParam prm;
    prm.content = text.toStdString();
    prm.contentType = "text/plain";

    pj::BuddyConfig bCfg;
    bCfg.uri = remoteUri.toStdString();
    bCfg.subscribe = false;
    std::unique_ptr<pj::Buddy> buddy {std::make_unique<pj::Buddy>()};

    try {
        buddy->create(*account, bCfg);
        buddy->sendInstantMessage(prm);
        m_messagesModel->saveMessage(remoteNumber, text, true, true);
        m_chatListModel->refresh();
    } catch (pj::Error &err) {
        qDebug() << "Ошибка отправки SIP:" << err.info().c_str();
    }
}

void ChatManager::openChat(const QString remoteNumber)
{
    m_currentChatProxy->setFilterNumber(remoteNumber);
    m_messagesModel->markVisibleMessagesAsRead(remoteNumber);
    m_chatListModel->refresh();
}

void ChatManager::closeChat()
{
    m_currentChatProxy->setFilterNumber("");
}

ChatManager::ChatManager(QObject *parent)
    : QObject{parent}, m_chatListModel{std::make_unique<ChatListModel>()},
    m_messagesModel{std::make_unique<MessagesModel>()},
    m_currentChatProxy{std::make_unique<ChatProxyModel>(m_messagesModel.get())}
{}
