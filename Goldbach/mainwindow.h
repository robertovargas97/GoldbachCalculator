#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

class GoldbachCalculator;
class QProgressBar;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)

protected:
    Ui::MainWindow *ui;
    QProgressBar* progressBar = nullptr;
    GoldbachCalculator* goldbachCalculator = nullptr;
    QTime time;

    bool pushed = false; //Para activar y desactivar el boton de informacion
    long long sumsFound = 0; //Para el numero de sumas encontradas por cada worker
    double progress = 0; //Guarda el progreso que viene como señal
    double barPercent = 0; //Guarda el calculo del procentaje para la barra de progreso
    long long number = 0; //Se utiliza como variable global para usarla en el calculo del porcentaje de la barra
    int workerDone = 0;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /**
     * @brief Start the calculations of sums for the given @a number
     * @param number The number given by user
     */
    void startCalculation(long long number);

    /**
     * @brief prepare reinicia todas la variables y botones antes de empezar el calculo para así no acumular valores en tiempo, cantidad de sumas o porcentaje de la barra de progreso
     */
    void prepare();

private slots:
    void on_lineEditNumber_textEdited(const QString &arg1);
    void on_pushButtonStart_clicked();
    void on_pushButtonInfo_clicked();
    void on_pushButtonStop_clicked();

protected slots:
    void calculationDone(int workerNumber, long long sumCount,double seconds,long beginning,long end,int ideal);
    void updateProgressBar(int percent);
};

#endif // MAINWINDOW_H
