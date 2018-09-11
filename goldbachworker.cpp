#include <QtMath>
#include "goldbachworker.h"
#include <QMetaType>


GoldbachWorker::GoldbachWorker(long long number, int workerNumber, int workerCount, bool *sieveOfEratosthenes, QObject *parent)
    : QThread(parent)
    , number{number}
    ,workerNumber{workerNumber/*+1*/}
    ,workerCount{workerCount}
    ,workerSieve{sieveOfEratosthenes}
{
    qRegisterMetaType<QVector<QString>>("QVector<QString>");//Se registra el tipo QVector para poder emitirlo por señal
}

void GoldbachWorker::run(){

    timePerWorker.start(); //Inicia el tiempo
    sumCount = this->calculate(number);
    seconds = timePerWorker.elapsed() / 1000.0; //Saca el tiempo en segundos
    emit this->calculationDone(this->workerNumber,this->sumCount,this->seconds,this->beginnig,this->end,this->workerResults);//Emite la señal con los datos necesarios
}

long long GoldbachWorker::calculate(long long number){
    if ( number < 4 || number == 5 ) return 0;
    return number % 2 == 0 ? calculateEvenGoldbach(number) : calculateOddGoldbach(number);
}

long GoldbachWorker::quantityPerWorker(int number, int workerCount){
    while (number < workerCount){ //Si el numero es menor a la cantidad de workers
        workerCount -=1; //Se ajusta para dar la cantidad de numeros necesarios a cada worker
    }

    int quantity = number / workerCount; //Cantidad de numeros necesarios para cada worker
    return quantity;
}

long GoldbachWorker::setBeginning(int workerNumber, int quantity){
    beginnig = (workerNumber/*-1*/)*quantity+2 ; //Se calcula el inicio de cada worker
    return  beginnig;
}

long GoldbachWorker::setEnd(int workerNumber, int quantity){

    end = (workerNumber+1)*quantity+1 ; //Se calcula donde debe finalizar cada worker

    if (workerNumber == workerCount-1){
        end = number-1; //El ultimo worker debe llegar al final
    }

    return end;
}

long long GoldbachWorker::calculateEvenGoldbach(long long number){
    int percent = 0;

    long long results = 0;

    int quantity = quantityPerWorker(number, this->workerCount);//Indica la cantidad de numeros que le toca examinar a cada trajador

    long beginnig = setBeginning(this->workerNumber, quantity) ; //Se calcula el inicio de cada worker

    long end = setEnd(this->workerNumber,quantity) ; //Se calcula donde debe finalizar el worker


    if ( beginnig < number){ //Si no es necesario el worker,no entra al if y no inicia el calculo

        for ( long long a = beginnig; a <= end; ++a ){

            percent+=1;

            if ( ! workerSieve[a] ) continue;
            long long b = number - a;
            if ( b >= a &&  workerSieve[b] ){
                results+=1;
                this->workerResults.append( tr("%1 + %2")/*.arg(results/)*/.arg(a).arg(b));
            }

            // If user cancelled, stop calculations
            if ( this->isInterruptionRequested() ){
                emit this->updateProgress(percent);
                return results;
            }
        }
    }

    emit this->updateProgress(percent);
    return results;
}

long long GoldbachWorker::calculateOddGoldbach(long long number)
{
    long long results = 0;

    int percent = 0;

    int quantity = quantityPerWorker(number, this->workerCount);//Indica la cantidad de numeros que le toca examinar a cada trajador

    long beginnig = setBeginning(this->workerNumber, quantity) ; //Se calcula el inico de cada worker

    long end = setEnd(this->workerNumber,quantity) ; //Se calcula donde debe finalizar el worker

    if ( beginnig < number){ //Si no es necesario el worker,no entra al if y no inicia el calculo

        for ( long long a = beginnig; a <= end; ++a ) {

            percent+=1;

            if ( !  workerSieve[a] ) continue;
            for ( long long b = a; b < number; ++b ){
                if ( !  workerSieve[b] ) continue;
                long long c = number - a - b;
                if ( c >= b &&  workerSieve[c] ){
                    results+=1;
                    this->workerResults.append( tr(/*"%1:*/"%1 + %2 + %3")/*.arg(results/)*/.arg(a).arg(b).arg(c));
                }

                // If user cancelled, stop calculations
                if ( this->isInterruptionRequested() ){
                    emit this->updateProgress(percent);
                    return results;
                }
            }

        }
    }

    emit this->updateProgress(percent);
    return results;
}

bool GoldbachWorker::isPrime(long long number){

    if ( number < 2 || (number !=2  && number % 2 == 0) )  { //Primera optimizacion
        return false;
    }

    for ( long long i = 2; i*i <= number; ++i ){ //Segunda optimizacion
        if ( number % i == 0 ){
            return false;
        }
    }
    return true;
}
