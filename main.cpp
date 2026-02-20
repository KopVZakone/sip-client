#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QQmlContext>
#include <QDir>
#include "core/databasemanager.h"
#include "models/settingsmanager.h"
#include "sipmanager.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SipManager& sipManager = SipManager::instance();
    sipManager.start();
    QString path = "sip_client.db";
    DatabaseManager dbManager {path};
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("settingsManager", &SettingsManager::instance());
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
