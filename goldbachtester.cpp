#include <iostream>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QtMath>
#include "goldbachcalculator.h"
#include "goldbachtester.h"


GoldbachTester::GoldbachTester(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
}

int GoldbachTester::run()
{
    if ( this->arguments().count() <= 1 ){
        std::cout << "Use: GoldbachTester test_folder1 test_folder2 ... test_folderN " << std::endl;
        return 0;
    }
    else{
        for ( int index = 1; index < this->arguments().count(); ++index ){
            this->testDirectory( this->arguments()[index] );
        }

    }
    return this->exec();
}

bool GoldbachTester::testDirectory(const QString &path)
{
    std::cout << "Directory : " << qPrintable(path) << std::endl;

    QDir dir(path);
    if ( ! dir.exists() )
        return std::cerr << "error: could not open directory: " << qPrintable(path) << std::endl, false;

    dir.setFilter(QDir::Files);

    QFileInfoList fileList = dir.entryInfoList();
    for ( int index = 0; index < fileList.count(); ++index )
        this->testFile( fileList[index] );

    return true;
}

bool GoldbachTester::testFile(const QFileInfo &fileInfo)
{
    std::cout << "Testing " << qPrintable( fileInfo.filePath() ) << std::endl;

    bool ok = true;
    long long number = fileInfo.baseName().toLongLong(&ok);
    if ( ! ok )
        return std::cerr << "error: invalid number: " << qPrintable(fileInfo.fileName()) << std::endl, false;

    GoldbachCalculator* goldbachCalculator = new GoldbachCalculator(this);
    this->calculators.insert(goldbachCalculator, fileInfo);
    this->connect( goldbachCalculator, &GoldbachCalculator::beginTest, this, &GoldbachTester::calculationDone );
    goldbachCalculator->calculate(number);
    return true;
}

void GoldbachTester::calculationDone()
{
    GoldbachCalculator* goldbachCalculator = dynamic_cast<GoldbachCalculator*>( sender() );
    Q_ASSERT(goldbachCalculator);

    const QFileInfo& fileInfo = this->calculators.value( goldbachCalculator );
    const QVector<QString>& expectedSums = this->loadLines(fileInfo);
    const QVector<QString>& calculatorSums = goldbachCalculator->getAllSums();

    bool success = true;

    for (long long i = 0; i < expectedSums.size();i++ ){

        if (i < calculatorSums.size() ){ //Si se puede seguir recorriendo el vector de resultados Goldbach

            if ( calculatorSums[i] != expectedSums[i] ){ //Si el resultado es distinto

                std::cout <<  "\n" <<"Test case failed: " << qPrintable(fileInfo.fileName()) << " at line " << i+1 << std::endl;

                std::cout << "Expected: " << qPrintable( expectedSums[i] ) << "\n" << "Produced: " << qPrintable( calculatorSums[i] ) <<
                             "\n"  << std::endl;
                success = false;

                errorCases+=1;

                totalCases+=1;
            }
        }
        else{ //Se acabo el vector de resultados Goldbach

            std::cout <<  "\n" <<"Test case failed: " << qPrintable(fileInfo.fileName()) << " at line " << i+1 << std::endl;

            std::cout << "Expected: " << qPrintable( expectedSums[i] ) << "\n" << "Produced: " << "Nothing produced in this line" <<
                         "\n"  << std::endl;
            success = false;

            errorCases+=1;

            totalCases+=1;
        }
    }

    if (success){ //Si nada falla
        succesCases+=1;

        totalCases+=1;
        // std::cout << "Test case passed: " << qPrintable(fileInfo.fileName()) << std::endl;
    }
    goldbachCalculator->deleteLater();
    this->calculators.remove( goldbachCalculator );
    if ( this->calculators.count() <= 0 )
        this->quit();
}

void GoldbachTester::testPercent(){
    std::cout << "\nPassed cases: " << succesCases << " (" << qFloor( (succesCases/totalCases)*100)<<"% )" << std::endl;
    std::cout << "Failed cases: " << errorCases << " (" << qCeil( (errorCases/totalCases)*100)<<"% )" << std::endl;
    std::cout << "Total cases: " << totalCases << "\n" << std::endl;
}

QVector<QString> GoldbachTester::loadLines(const QFileInfo &fileInfo)
{
    QVector<QString> result;

    QFile file( fileInfo.absoluteFilePath() );

    if ( ! file.open(QFile::ReadOnly) )

        return std::cerr << "error: could not open: " << qPrintable(fileInfo.fileName()) << std::endl, result;

    QTextStream textStream( &file );

    while ( ! textStream.atEnd() ){

        QString line  = textStream.readLine(); //String con la linea leida

        if(line.trimmed().length() > 0){ //Si no hay espacios en blanco y hay algo en la linea
            result.append( line );
        }
    }

    return result;
}
