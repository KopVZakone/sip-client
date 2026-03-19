#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QQmlContext>
#include <QDir>
#include <callmanager.h>
#include "databasemanager.h"
#include "sipmanager.h"
#include "accountsmanager.h"
#include "audiomanager.h"
#include "chatmanager.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setOrganizationName("KopVZakone");
    QCoreApplication::setApplicationName("SipClient");
    //Инициализация бд
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString path = dir.filePath("sip_client.db");
    DatabaseManager dbManager {path};

    // Инициализация библиотеки pjsip
    auto& sipManager = SipManager::instance();
    sipManager.start();
    auto& accountsManager = AccountsManager::instance();
    auto& audioManager = AudioManager::instance();
    auto& callManager = CallManager::instance();
    auto& chatManager = ChatManager::instance();
    audioManager.applySettings();
    accountsManager.applySettings();
    QQmlApplicationEngine engine;
    // Установка синглтонов для интерфейса
    engine.rootContext()->setContextProperty("accountsManager", &accountsManager);
    engine.rootContext()->setContextProperty("audioManager", &audioManager);
    engine.rootContext()->setContextProperty("callManager", &callManager);
    engine.rootContext()->setContextProperty("chatManager", &chatManager);
    const QUrl url(QStringLiteral("qrc:/SipClient/ui/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);


    int appResult = app.exec();

    sipManager.stop();

    return appResult;
}
