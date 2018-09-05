#ifndef GOLDBACHWORKER_H
#define GOLDBACHWORKER_H

#include <QThread>
#include <QTime>
#include <QVector>

class GoldbachWorker : public QThread
{
    Q_OBJECT

protected:
    long beginnig = 0;//Inicio del trabajo, será enviado por señal
    long end = 0;//Final del trabajo, será enviado por señal
    long long number = 0;//Numero a examinar
    double seconds = 0; //Segundos que dura cada worker en realizar el trabajo, será enviado por señal
    long long sumCount= 0;//Numero de sumas encontradas por cada worker, será enviado por señal
    int workerCount = 0;//Cantidad total de workers
    int workerNumber = 0;//Numero del worker
    QTime timePerWorker; //Marca el tiempo por caa worker
    QVector<QString> workerResults; //Almacenara las sumas de cada worker
    bool * sieve;


public:
    explicit GoldbachWorker(long long number, int workerNumber, int workerCount, bool *  sieve, QObject *parent = nullptr);

    /**
     * @brief calcula la cantidad de numeros que debe recorrer cada worker
     * @param el numero a inspeccionar que se esta inspeccionado
     * @param workerCount : Cantidad total de workers
     * @return la cantidad de numeros que debe recorrer el worker por ejmplo 3, 4...
     */
    long quantityPerWorker (int number, int workerCount);

    /**
     * @brief calcula el inicio de cada worker
     * @param workerNumber es el numero de worker
     * @param quantity es la cantidad de numeros que le tocan al worker
     * @return retorna el numero donde debe iniciar el worker
     */
    long setBeginning(int workerNumber,int quantity);

    /**
     * @brief calcula el final de cada worker
     * @param workerNumber es el numero de worker
     * @param quantity es la cantidad de numeros que le tocan al worker
     * @return retorna el numero donde debe finalizar el worker
     */
    long setEnd(int workerNumber,int quantity);

protected:
    void run() override;

signals:
    //Señal para emitir los datos necesarios en el GoldBachCalculator
    void calculationDone(int workerNumber, long long sumCount,double seconds,long beginning,long end, QVector<QString> workerResults);
    void updateProgress (long long percent); //Señal para actualizar la barra de progreso

public slots:

protected:
    /**
    * @brief Calcula las sumas de Goldbach para el numero dado y las agrega a una pizarra
    * @param number El numero dado por el usuario
    * @return La cantidad de sumas encontradas
    */
    long long calculate(long long number);
    /**
    * Calcula todas las sumas de dos primos que equivalen al numero dado, y las presenta
    * en el area resultado. Incrementa la barra de progreso mientras realiza el calculo
    * de la conjetura fuerte de Goldbach, por ejemplo:
    *
    *   4 == 2 + 2
    *   6 == 3 + 3
    *  10 == 3 + 7 == 5 + 5
    *
    * @param numero Un numero entero par mayor o igual a 4
    * @return la cantidad de sumas de dos primos encontradas
    */
    long long calculateEvenGoldbach(long long number);
    /**
    * Calcula todas las sumas de tres primos que equivalen al numero dado, y las presenta
    * en el area resultado. Incrementa la barra de progreso mientras realiza el calculo
    * de la conjetura debil de Goldbach, por ejemplo:
    *
    *   7 == 2 + 2 + 3
    *   9 == 2 + 2 + 5 == 3 + 3 + 3
    *
    * @param numero Un numero entero impar mayor o igual a 7
    * @return la cantidad de sumas de tres primos encontradas
    */
    long long calculateOddGoldbach(long long number);
    /**
    * Retorna true si numero es primo, false si numero no es primo o menor que 2
    * Por definicion 1 no es primo ni compuesto, este metodo retorna false
    */
    static bool isPrime(long long numero);
};

#endif // GOLDBACHWORKER_H
