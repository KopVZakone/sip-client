#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QDir>
#include "Core/databasemanager.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QString path = "sip_client.db";
    DatabaseManager manager {path};
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/SipClient/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
