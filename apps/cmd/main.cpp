#include <QCoreApplication>
#include <QProcess>
#include <QStringList>
#include <QTime>
#include <QTextStream>
#include <QFile>
#include <QDebug>

#include <iostream>

using namespace std;

static QString outputFile;
static QString sourceFile;

static void processOptions(QStringList &args);
static void getSourceFile(const QStringList &chunks);

int main (int argc, char **argv)
{
    const int maxTimeInMsecs = 30000;
    const QString timeFormat("hh:mm:ss.zzz");
    QCoreApplication app (argc, argv);
    QStringList args = app.arguments();

    processOptions(args);
    getSourceFile(args);
    QString argsTogeter = args.join(" ");

    QProcess process;
    process.setProcessChannelMode(QProcess::SeparateChannels);

    QTime startTime = QTime::currentTime();
    process.start(argsTogeter);
    if (process.waitForFinished(maxTimeInMsecs) == false) {
        cerr << "application timeout: " << process.errorString().toStdString() << endl;
        return EXIT_FAILURE;
    }
    QTime endTime = QTime::currentTime();
    const int msecs = startTime.msecsTo(endTime);

    if (outputFile.isEmpty()) {
        cout << "Application finished correctly in " << msecs << " msecs" << endl;
    } else {
        QFile data(outputFile);
        if (data.open(QFile::WriteOnly | QFile::Append)) {
            QTextStream out(&data);
            out << sourceFile << " " << msecs << " "
                << startTime.toString(timeFormat) << " "
                << endTime.toString(timeFormat) << endl;
        } else {
            cerr << "Could not open output file " << outputFile.toStdString() << endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

static void processOptions(QStringList &args)
{
    args.removeFirst(); // The first argument is this own application
    qDebug() << "Inputs args" << args;
    const int indexOutputFile = args.indexOf("--outputFile");
    if (indexOutputFile != -1) {
        outputFile = args.at(indexOutputFile+1);
        args.erase(args.begin()+indexOutputFile, args.begin()+(indexOutputFile+2));
    }
    qDebug() << "Output args" << args;
}

static void getSourceFile(const QStringList &chunks)
{
    int indexSourceFile = chunks.indexOf("-c");
    sourceFile  = chunks.at(indexSourceFile + 1);
}
