#include <QtMath>
#include <QDebug>
//#include <QMutex>
#include "goldbachworker.h"
#include <QMetaType>




GoldbachWorker::GoldbachWorker(long long number, int workerNumber, int workerCount, bool * sieve, QObject *parent)
    : QThread(parent)
    , number{number}
    ,workerNumber{workerNumber}
    ,workerCount{workerCount}
    ,sieve{sieve}
{
    qRegisterMetaType<QVector<QString>>("QVector<QString>");//Se registra el tipo QVector para poder emitirlo por señal
}

void GoldbachWorker::run(){

    timePerWorker.start(); //Inicia el tiempo
    sumCount = this->calculate(number);//Se guarda la cantidad de sumas resultante
    seconds = timePerWorker.elapsed() / 1000.0; //Guarda el tiempo de duracion en segundos

    emit this->calculationDone(this->workerNumber,this->sumCount,this->seconds,this->beginnig,this->end,this->workerResults);//Emite la señal con los datos necesarios hacia GoldBachCalculator
}

long long GoldbachWorker::calculate(long long number){
    if ( number < 4 || number == 5 ) return 0;
    return number % 2 == 0 ? calculateEvenGoldbach(number) : calculateOddGoldbach(number);
}

long GoldbachWorker::quantityPerWorker(int number, int workerCount){

    while (number < workerCount){ //Si el numero a examinar es menor a la cantidad de workers
        workerCount -=1; //Se ajusta para dar la cantidad de numeros necesarios a cada worker
    }

    int quantity = number / workerCount; //Se guarda la cantidad de numeros necesarios para cada worker
    return quantity;
}

long GoldbachWorker::setBeginning(int workerNumber, int quantity){

    beginnig = (workerNumber)*quantity+2 ; //Se calcula el inicio de cada worker
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

    long long percent = 0; //Se usará para emitir un uno por cada iteracion para actualizar la barra de progreso

    long long results = 0; //Guardara la cantidad de sumas encontradas

    int quantity = quantityPerWorker(number, this->workerCount);//Indica la cantidad de numeros que le toca examinar a cada trajador

    long beginnig = setBeginning(this->workerNumber, quantity) ; //Se calcula el inicio de cada worker

    long end = setEnd(this->workerNumber,quantity) ; //Se calcula donde debe finalizar el worker


    if ( beginnig < number){ //Si el worker no es necesarion , no se entra al if y no inicia el calculo

        for ( long long a = beginnig; a <= end; ++a ){

            percent +=1;

            if ( sieve[a] == false ) continue;

            long long b = number - a;

            if ( b >= a && sieve[b] == true ){
                results+=1;
                this->workerResults.append( tr("%1 + %2").arg(a).arg(b));
            }

            // Si el usuario cancela la operacion
            if ( this->isInterruptionRequested() ){
                //Actualiza la barra de progreso
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
    long long percent = 0; //Se usará para emitir un uno por cada iteracion para actualizar la barra de progreso

    long long results = 0; //Guardara la cantidad de sumas encontradas

    int quantity = quantityPerWorker(number, this->workerCount);//Indica la cantidad de numeros que le toca examinar a cada trajador

    long beginnig = setBeginning(this->workerNumber, quantity) ; //Se calcula el inico de cada worker

    long end = setEnd(this->workerNumber,quantity) ; //Se calcula donde debe finalizar el worker

    if ( beginnig < number){ //Si no es necesario el worker,no entra al if y no inicia el calculo

        for ( long long a = beginnig; a <= end; ++a ) {

            percent +=1;

            if ( sieve[a] == false ) continue;

            for ( long long b = a; b < number; ++b ){
                if ( sieve[b] == false ) continue;

                long long c = number - a - b;

                if ( c >= b && sieve[c] == true ){
                    results+=1;
                    this->workerResults.append( tr("%1 + %2 + %3").arg(a).arg(b).arg(c));
                }

                // Si el usuario cancela el calculo
                if ( this->isInterruptionRequested() ){
                    //Actualiza la barra de progreso
                    emit this->updateProgress(percent);
                    return results;
                }
            }
        }
    }
    //Actualiza la barra de progreso
    emit this->updateProgress(percent);
    return results;
}

bool GoldbachWorker::isPrime(long long number){

    if ( number < 2 ) return false;

    for ( long long i = 2, last = qSqrt(number); i <= last; ++i )
        if ( number % i == 0 )
            return false;

    return true;

    /*
    if ( number < 2 || (number !=2  && number % 2 == 0) )  { //Primera optimizacion
        return false;
    }

    for ( long long i = 2; i*i <= number; ++i ){ //Segunda optimizacion
        if ( number % i == 0 ){
            return false;
        }
    }
    return true;
 */

}
