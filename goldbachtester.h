#ifndef GOLDBACHTESTER_H
#define GOLDBACHTESTER_H

#include <QCoreApplication>
#include <QFileInfo>
#include <QMap>

class GoldbachCalculator;

class GoldbachTester : public QCoreApplication
{
    Q_OBJECT
    Q_DISABLE_COPY(GoldbachTester)

protected:
    QMap<GoldbachCalculator*, QFileInfo> calculators;

public:

    explicit GoldbachTester(int &argc, char **argv);
    double succesCases = 0;
    double errorCases = 0;
    double totalCases = 0;
    int run();
    bool testDirectory(const QString& path);
    bool testFile(const QFileInfo& fileInfo);
    static QVector<QString> loadLines(const QFileInfo& fileInfo);

    void testPercent ();

protected slots:
    void calculationDone();
};

#endif // GOLDBACHTESTER_H
