#include <QApplication>
#include "qmlapplicationviewer.h"

#include <QDeclarativeContext>
#include <QtDeclarative/qdeclarative.h>
#include "mycryptor.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qmlRegisterType<MyCryptor>("Cryptor", 1, 0, "MyCryptor");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);
    viewer.setMainQmlFile(QLatin1String("qml/KeySaver/main.qml"));
    viewer.showExpanded();

//    viewer.rootContext()->setContextProperty("cryptor", new MyCryptor(&viewer));
    return app->exec();
}
