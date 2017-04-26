#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <stdio.h>
#include <stdlib.h>

/*
void end_Signal(int signal) {
     //printf("Signal : %u\n", signal);
     bool keepReading = false;
    emit keepReading;

}
*/

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){

        QString txt, ctxt;
        switch (type) {
        case QtDebugMsg:
            txt = QString("Debug: %1").arg(msg);
            break;
        case QtWarningMsg:
            txt = QString("Warning: %1").arg(msg);
        break;
        case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
        break;
        case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
        break;
        }
        ctxt = QString::fromUtf8(context.category).append(" - ")
                .append(QString::fromUtf8(context.file).append(" - ")
                        .append(QString::fromUtf8(context.function).append(" - ")
                                .append(QString::number(context.line).append(" - ")
                                        .append(QString::number(context.version)))));
        QFile outFile("log");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << ctxt << endl;
        ts << txt << endl;
}



int main(int argc, char *argv[]){

    qInstallMessageHandler(myMessageOutput); // Install the handler

//    signal(SIGINT, end_Signal);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
