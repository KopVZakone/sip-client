#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QQmlContext>
#include <QDir>
#include "Core/databasemanager.h"
#include "Models/settingsmanager.h"
#include "sipmanager.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SipManager& sipManager = SipManager::instance();
    sipManager.start();
    QString path = "sip_client.db";
    DatabaseManager dbManager {path};
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/SipClient/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
    engine.rootContext()->setContextProperty("settingsManager", &SettingsManager::instance());

    int appResult = app.exec();

    sipManager.stop();

    return appResult;
}
