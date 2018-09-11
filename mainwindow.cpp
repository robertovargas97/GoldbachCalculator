#include "goldbachcalculator.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Goldbach");
    this->ui->statusBar->showMessage(tr("Ready"));

    this->progressBar = new QProgressBar();
    this->ui->statusBar->addPermanentWidget(this->progressBar);
    this->ui->plainTextEdit->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEditNumber_textEdited(const QString &arg1)
{
    (void)arg1;
    bool enable = this->ui->lineEditNumber->text().trimmed().length() > 0;
    this->ui->pushButtonStart->setEnabled( enable );

    if (!enable){
        this->ui->pushButtonInfo->setEnabled(enable);
    }
}

void MainWindow::on_pushButtonStart_clicked()
{
    const QString& text = this->ui->lineEditNumber->text();
    bool valid = true;
    number = text.toLongLong(&valid);

    if ( valid ){

        this->startCalculation(number);
    }

    else{

        this->ui->statusBar->showMessage( tr("Invalid number: %1").arg(text) );
    }
}

void MainWindow::on_pushButtonStop_clicked(){
    Q_ASSERT( this->goldbachCalculator );
    this->goldbachCalculator->stop();
}

void MainWindow::on_pushButtonInfo_clicked(){
    if (!pushed){
        this->ui->plainTextEdit->show();
        pushed = true;
        this->ui->pushButtonInfo->setText("Hide info");
    }
    else{
        this->ui->plainTextEdit->hide();
        this->ui->pushButtonInfo->setText("info");
        pushed = false;
    }

}

void MainWindow::prepare(){
    this->ui->pushButtonStart->setEnabled(false);
    this->ui->pushButtonInfo->setEnabled(false);
    this->ui->pushButtonInfo->setText("info");
    this->ui->statusBar->clearMessage();//Se limpia el mensaje de la barra de estado
    this->progressBar->setValue(0);
    this->ui->plainTextEdit->clear();
    this->ui->plainTextEdit->hide();

    workerDone = 0; //Reinicia el numero de workers finalizado
    progress = 0; //Se reinicia el progreso de la barra
    barPercent =0; //Se reinicia el porcentaje de la barra
    sumsFound = 0; //Se reinicia la cantidad de sumas para cada vez que se presione start
    pushed = false; //Se reinicia el estado del boton de informacion
}

void MainWindow::startCalculation(long long number)
{
    this->prepare();

    this->ui->pushButtonStop->setEnabled(true);

    if ( this->goldbachCalculator )

        this->goldbachCalculator->deleteLater();

    this->goldbachCalculator = new GoldbachCalculator(this);

    this->connect( this->goldbachCalculator, &GoldbachCalculator::updateProgressB, this, &MainWindow::updateProgressBar);

    this->connect( this->goldbachCalculator, &GoldbachCalculator::calculationDone, this, &MainWindow::calculationDone );

    this->ui->listViewResults->setModel( this->goldbachCalculator );

    this->goldbachCalculator->calculate( number );

    this->ui->statusBar->showMessage(tr("Calculating..."));

    time.start();

}

void MainWindow::updateProgressBar(int percent)
{
    //Calculo del porcentaje
    progress += percent;
    barPercent = (progress/(number-2));
    barPercent *=100;
    this->progressBar->setValue(barPercent);
}

void MainWindow::calculationDone(int workerNumber, long long sumCount, double seconds, long beginning, long end, int ideal)
{
    double totalTime = time.elapsed()/1000.0;
    workerDone +=1;

    if(beginning >= number){
        this->ui->plainTextEdit->appendPlainText(tr(" WorkerNumber: %1 \n Uninitialized worker\n").arg(workerNumber));
    }
    else{
        this->ui->plainTextEdit->appendPlainText(tr(" WorkerNumber: %1 \n Range: %2 to %3, SumsFound: %4, WorkerTime : %5 \n").arg(workerNumber).arg(beginning).arg(end).arg(sumCount).arg(seconds));

    }

    sumsFound+= sumCount; //Se acumulan las sumas encontradas

    this->ui->pushButtonInfo->setEnabled(true);


    if (workerDone >= ideal ){
        this->ui->pushButtonStart->setEnabled(true);
        this->ui->statusBar->showMessage(tr("%1 sums found in %2 seconds").arg(sumsFound).arg(totalTime));
        this->ui->pushButtonStop->setEnabled(false);
    }

}
