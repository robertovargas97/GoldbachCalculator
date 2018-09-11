#ifndef GOLDBACHCALCULATOR_H
#define GOLDBACHCALCULATOR_H

#include <QAbstractListModel>
#include <QVector>

class GoldbachWorker;

class GoldbachCalculator : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(GoldbachCalculator)

protected:

    int fetchedRowCount = 0;
    int ideal = 0; //Numero ideal de threads
    int workersFinished = 0; //Indicara la cantidad de workers que terminaron el proceso
    GoldbachWorker* goldbachWorker = nullptr;
    QVector <QString> resultsForTest; //Almacenara todos los resultados en orden
    QVector<QVector <QString>> results;
    QVector<GoldbachWorker*> goldbachWorkersArray;
    bool * sieveOfEratosthenes = nullptr;

    /**
     * @brief totalNumbers calculara el numero de elemtos totales que existen en el vector de vectores, es decir la cantidad de
     * elementos que tenga res[i]+res[i]....+res[n]
     * @return la suma de la cantidad de todos los elemtos en el vector de vectores
     */
    long long totalNumbers () const;

    void fillSieveOfEratosthenes(long long number);
    void evenNumbers (long long number);

public:
    explicit GoldbachCalculator(QObject *parent = nullptr);
    void calculate(long long number);
    void stop();

    /**
         * @brief getAllSums Consolidara todos los vectores de sumas en uno solo para realizar las pruebas en el tester
         * @return  retorna un arreglo con todas las sumas
    */
    QVector <QString> getAllSums();

public: // Overriden methods from QAbastractListModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    virtual bool canFetchMore(const QModelIndex &parent) const override;
    virtual void fetchMore(const QModelIndex &parent) override;

signals:
    void calculationDone(int workerNumber,long long sumCount,double seconds,long beginning,long end,int ideal);
    void updateProgressB(int percent);
    //Señal para indiciar que se debe iniciar la prueba con el tester
    void beginTest ();

protected slots:
    void workerDone(int workerNumber, long long sumCount, double seconds, long beginning, long end, QVector<QString> workerResults); //Para recibir la señal con los datos necesarios
    void updateProgress(int percent); //Para la barra de progreso
};

#endif // GOLDBACHCALCULATOR_H
