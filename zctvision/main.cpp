#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

#include "FileLogger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QDir dir;
    initLog(QString(dir.currentPath()+"//zctvision.dat").toLatin1().data(), 1024*1024*5, 3);
    logFile(FileLogger::info, "Init zctvision");

    QTranslator translator;
    translator.load(dir.currentPath()+"//zctvision_zh_cn.qm");
    qApp->installTranslator(&translator);

    MainWindow w;

    w.init();

    w.show();

    return a.exec();
}
