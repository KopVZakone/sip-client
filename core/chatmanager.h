#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "chatlistmodel.h"
#include "chatproxymodel.h"
#include "messagesmodel.h"
#include <QObject>
#include <QtQml>
class ChatManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(ChatListModel* chatList READ chatList CONSTANT)
    Q_PROPERTY(ChatProxyModel* currentChat READ currentChat CONSTANT)
public:
    static ChatManager& instance();
    ChatListModel *chatList() const;
    ChatProxyModel *currentChat() const;
    void receiveMessage(const QString& remoteNumber, const QString& text);
    Q_INVOKABLE void sendMessageInCurrentChat(const QString& text);
    Q_INVOKABLE void openChat(const QString remoteNumber);
    Q_INVOKABLE void closeChat();
signals:
private:
    explicit ChatManager(QObject *parent = nullptr);
    std::unique_ptr<ChatListModel> m_chatListModel;
    std::unique_ptr<MessagesModel> m_messagesModel;
    std::unique_ptr<ChatProxyModel> m_currentChatProxy;
};

#endif // CHATMANAGER_H
