#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QQmlContext>
#include <QDir>
#include "databasemanager.h"
#include "sipmanager.h"
#include "accountsmanager.h"
#include "audiomanager.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    //Инициализация бд
    QString path = "sip_client.db";
    DatabaseManager dbManager {path};

    // Инициализация библиотеки pjsip
    auto& sipManager = SipManager::instance();
    sipManager.start();
    auto& accountsManager = AccountsManager::instance();
    auto& audioManager = AudioManager::instance();
    audioManager.applySettings();
    QQmlApplicationEngine engine;
    // Установка синглтонов для интерфейса
    engine.rootContext()->setContextProperty("accountsManager", &accountsManager);
    engine.rootContext()->setContextProperty("audioManager", &audioManager);

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
