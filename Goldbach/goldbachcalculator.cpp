#include "goldbachcalculator.h"
#include "goldbachworker.h"
#include <QDebug>



GoldbachCalculator::GoldbachCalculator(QObject *parent)
    : QAbstractListModel(parent)
{
}

void GoldbachCalculator::calculate(long long number)
{
    this->beginResetModel();
    workersFinished = 0;
    ideal = QThread::idealThreadCount() - 1;

    fillSieveOfEratosthenes(number);

    results.resize(ideal);//Se da tamaño al arreglo de arreglos de resultados

    for ( int current = 0; current < ideal; ++current ){ //For para crear tantos workers como se pueda

        this->goldbachWorker = new GoldbachWorker(number, current, ideal,sieveOfEratosthenes, this);//Se crea el worker
        goldbachWorkersArray.append(goldbachWorker);//Se agrega el worker al arreglo de workers

        //Conexion a señales
        this->connect( this->goldbachWorker, &GoldbachWorker::updateProgress, this, &GoldbachCalculator::updateProgress );
        this->connect( this->goldbachWorker, &GoldbachWorker::calculationDone, this, &GoldbachCalculator::workerDone );

        //Inicio de los workers
        this->goldbachWorker->start();
    }

    this->fetchedRowCount = 0;
    this->endResetModel();
}


void GoldbachCalculator::stop(){

    for ( int current = 0; current < ideal; ++current  ){//Se recorre el arreglo de workers

        if ( goldbachWorkersArray[current]!= nullptr){ //Si existe el worker en esa posicion
            Q_ASSERT( this->goldbachWorkersArray[current] );

            goldbachWorkersArray[current]->requestInterruption();//Se manda la señal de interrupcion al worker
        }
    }
}

int GoldbachCalculator::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->fetchedRowCount;
}

QVariant GoldbachCalculator::data(const QModelIndex &index, int role) const{


    if ( ! index.isValid() )
        return QVariant();

    if ( index.row() < 0 || index.row() >= totalNumbers() )
        return QVariant();

    if (role == Qt::DisplayRole){

        for (int i = 0;i < results.size(); i++){

            QString sumNumber = tr("%1").arg(index.row()+1);//Crea el string con el numero de posicion para la suma correspondiente

            if (results[i].size() > index.row() ){//Si se pueden extraer elementos del vector donde estoy
                //qDebug() << res [i] [ index.row() ];
                //Retorneme el elemento que corresponde a esa posicion
                return  tr("%1: %2 ").arg(sumNumber).arg(this-> results [i] [ index.row() ]);

            }
            else{ //Si no puedo extrae el elemento en el arreglo donde estoy
                int indexB =  index.row() - results[i].size();
                //qDebug() << res [i+1] [ indexB ];
                return  tr("%1: %2 ").arg(sumNumber).arg(this->results [i+1] [ indexB ]);
                // return results [i+1] [ indexB ];
                // return sum;
            }
        }

        //qDebug() << index.row();
        // return this->results[ index.row() ];
    }

    return QVariant();
}

bool GoldbachCalculator::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->fetchedRowCount < totalNumbers();
}

void GoldbachCalculator::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent);

    int remainder = totalNumbers() - this->fetchedRowCount;
    int itemsToFetch = qMin(100, remainder);

    if (itemsToFetch <= 0)
        return;

    int firstRow = this->fetchedRowCount;
    int lastRow = this->fetchedRowCount + itemsToFetch - 1;

    beginInsertRows(QModelIndex(), firstRow, lastRow);
    this->fetchedRowCount += itemsToFetch;
    endInsertRows();
}

void GoldbachCalculator::workerDone(int workerNumber, long long sumCount, double seconds, long beginning, long end, QVector<QString> workerResults)
{

    emit this->calculationDone(workerNumber,sumCount,seconds,beginning,end,this->ideal);//Se emite la señal con los datos necesarios
    results[workerNumber] = workerResults; //Asigna a la posicion correspondiente del worker el arreglo de resultados

    goldbachWorkersArray[workerNumber]->deleteLater(); //Se mata el worker que ha terminado
    goldbachWorkersArray[workerNumber] = nullptr;

    workersFinished+=1;
    if (workersFinished >= ideal){ //Si todos los workers terminaron los resultados están listos
        emit this->beginTest(); // Se envía la señal para empezar las pruebas
    }
}

QVector <QString> GoldbachCalculator::getAllSums(){

    long long index = 1;

    for (int i = 0; i < this->results.size();i++){

        for (int j = 0; j < this->results[i].size(); j++){

            this->resultsForTest.append (tr("%1: %2").arg(index).arg(this->results[i][j]));
            index+=1;
        }
    }
    return this->resultsForTest;
}


long long GoldbachCalculator::totalNumbers() const{
    long long totalSums = 0;
    for (int i = 0; i < this->results.size();i++){
        totalSums += results[i].size();
    }
    return totalSums;
}

void GoldbachCalculator::fillSieveOfEratosthenes(long long number){

    if (number > 0 ){ //La criba se crea solamente si el numero es mayor a 0
        sieveOfEratosthenes = new bool [number];
        for (int i = 0; i <= number; i++) {
            sieveOfEratosthenes[i] = true;
        }

        for (int i = 2; i <= number / 2; i++) {
            for (int j = 2; j * i <= number; j++) {
                sieveOfEratosthenes[i * j] = false;
            }
        }
    }
}

void GoldbachCalculator::updateProgress(long long percent){
    emit this->updateProgressB(percent);
}
