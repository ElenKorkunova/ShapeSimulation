#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

//    engine.loadFromModule("ShapeSimulation", "Main");
    // FIXED: Instead of using generic engine.load(url), we use loadFromModule.
    // First argument: The exact URI specified in your CMakeLists.txt (Simulation.Backend)
    // Second argument: The main QML file name registered inside that module (Main)
    engine.loadFromModule("Simulation.Backend", "Main");

    return app.exec();
}
