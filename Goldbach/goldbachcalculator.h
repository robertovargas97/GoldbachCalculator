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

    QVector <QString> resultsForTest; //Almacenara todos los resultados del calculo en orden una vez que se inicie la prueba

    QVector<QVector <QString>> results; //Almacenara los arreglos de resultados provenientes de cada worker

    QVector<GoldbachWorker*> goldbachWorkersArray; //Almacenará los workers para poder manipularlos segun su WorkerNumber

    bool * sieveOfEratosthenes = nullptr;


    /**
     * @brief totalNumbers calculara el numero de elemtos totales que existen en el vector de vectores, es decir la cantidad de
     * elementos que tenga res[i]+res[i]....+res[n]
     * @return la suma de la cantidad de todos los elemtos en el vector de vectores
     */
    long long totalNumbers () const;

    void fillSieveOfEratosthenes(long long number);

public:
    explicit GoldbachCalculator(QObject *parent = nullptr);

    /**
     * @brief getAllSums Consolidara todos los vectores de sumas en uno solo para realizar las pruebas en el tester
     * @return  retorna un arreglo con todas las sumas
     */
    QVector <QString> getAllSums();

    void calculate(long long number);
    void stop();

public: // Overriden methods from QAbastractListModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    virtual bool canFetchMore(const QModelIndex &parent) const override;
    virtual void fetchMore(const QModelIndex &parent) override;

signals:
    //Señal para emitir los datos necesarios en el GoldBachCalculator
    void calculationDone(int workerNumber,long long sumCount,double seconds,long beginning,long end,int ideal);

    //Señal para indiciar que se debe iniciar la prueba con el tester
    void beginTest ();

    //Señal para actualizar barra de progreso
    void updateProgressB(int percent);

protected slots:
    //Ranura o campo para recibir los datos necesarios desde el goldBachWorker
    void workerDone(int workerNumber, long long sumCount, double seconds, long beginning, long end, QVector<QString> workerResults);

    //Señal para actualizar la barra de progreso
    void updateProgress(long long percent);
};

#endif // GOLDBACHCALCULATOR_H
