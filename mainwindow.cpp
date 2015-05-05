/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2015 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.04.15                                             **
**          Version: 1.3.1                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <cstdlib>




MainWindow::MainWindow(int numberOfMeasurements, QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow),
        sharedTable_(boost::interprocess::create_only,"realTimePlotter_float",boost::interprocess::read_write,10),
        numberOfMeasurements_(numberOfMeasurements),
        previousTime(-0.0f) {

    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);

    if (numberOfMeasurements_ == 0);
        //setupRealtimeDataDemo(ui->customPlot);
    else if (numberOfMeasurements_ < 9) {
        setupRealTimePlot(ui->customPlot);
    }
    else
        std::exit(-1);

    setWindowTitle("Plot");
    statusBar()->clearMessage();
    ui->customPlot->replot();
}


MainWindow::~MainWindow() {
    delete ui;
}



void MainWindow::setupRealTimePlot(QCustomPlot *customPlot) {

    size_t num = 0;
    if (num < numberOfMeasurements_) {

        customPlot->addGraph();
        customPlot->graph(num*2)->setPen(QPen(Qt::blue));
        customPlot->addGraph();
        customPlot->graph(num*2+1)->setPen(QPen(Qt::blue));
        customPlot->graph(num*2+1)->setLineStyle(QCPGraph::lsNone);
        customPlot->graph(num*2+1)->setScatterStyle(QCPScatterStyle::ssDisc);

        ++num;
    }
    if (num < numberOfMeasurements_) {

        customPlot->addGraph();
        customPlot->graph(num*2)->setPen(QPen(Qt::red));
        customPlot->addGraph();
        customPlot->graph(num*2+1)->setPen(QPen(Qt::red));
        customPlot->graph(num*2+1)->setLineStyle(QCPGraph::lsNone);
        customPlot->graph(num*2+1)->setScatterStyle(QCPScatterStyle::ssDisc);

        ++num;
    }
    if (num < numberOfMeasurements_) {

        customPlot->addGraph();
        customPlot->graph(num*2)->setPen(QPen(Qt::green));
        customPlot->addGraph();
        customPlot->graph(num*2+1)->setPen(QPen(Qt::green));
        customPlot->graph(num*2+1)->setLineStyle(QCPGraph::lsNone);
        customPlot->graph(num*2+1)->setScatterStyle(QCPScatterStyle::ssDisc);

        ++num;
    }
    if (num < numberOfMeasurements_) {

        ++num;
    }
    if (num < numberOfMeasurements_) {

        ++num;
    }
    if (num < numberOfMeasurements_) {

        ++num;
    }
    if (num < numberOfMeasurements_) {

        ++num;
    }
    if (num < numberOfMeasurements_) {

        ++num;
    }
    if (num < numberOfMeasurements_) {

        ++num;
    }

    customPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(2);
    customPlot->axisRect()->setupFullAxesBox();


    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));



    connect(&readDataTimer_, SIGNAL(timeout()), this, SLOT(updateData()));
    readDataTimer_.start(0);


    connect(&plotRefreshTimer_, SIGNAL(timeout()), this, SLOT(refreshPlot()));
    plotRefreshTimer_.start(20);
}



void MainWindow::refreshPlot() {
    ui->customPlot->replot();
    ulong numberOfPoints = 0;
    for (int i = 0; i < numberOfMeasurements_; ++i)
        numberOfPoints += ui->customPlot->graph(i*2)->data()->count();
    ui->statusBar->showMessage(
        QString("%1 FPS, Total Data points: %2")
        .arg(double(1000.0/double(plotRefreshTimer_.interval())), 0, 'f', 2)
        .arg(numberOfPoints)
        , 0);
}



void MainWindow::updateData() {
    float actualData[10];
    //std::cout << "asdfasf\n";
    sharedTable_.readData(actualData);
    if (actualData[0] != previousTime) {
        previousTime = actualData[0];
        for (int i = 0; i < numberOfMeasurements_; ++i) {
            ui->customPlot->graph(i*2)->addData(previousTime, actualData[i+1]); // line
            ui->customPlot->graph(i*2)->removeDataBefore(previousTime-8); // line
            ui->customPlot->graph(i*2)->rescaleValueAxis(); // line
            ui->customPlot->graph(i*2+1)->clearData(); // dot
            ui->customPlot->graph(i*2+1)->addData(previousTime, actualData[i+1]); // dot
        }
        ui->customPlot->xAxis->rescale();
    }
}





































