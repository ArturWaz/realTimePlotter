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

#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <cstdlib>

#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(int numberOfMeasurements, QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow),
        sharedTable_("realTimePlotter_float",10,boost::interprocess::read_write),
        numberOfMeasurements_(numberOfMeasurements),
        previousTime(-0.0f),
        yMin(-0.5f),
        yMax(0.5f) {

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

    ui->customPlot->xAxis->setRange((previousTime - 10.) + double(ui->timeSlider->value()) / 100, previousTime);

    if (ui->autoScalling->isChecked()) {
        ui->customPlot->yAxis->setRange(yMin,yMax);
    } else {
        float level = ui->levelSignificand->value()*std::pow(10,ui->levelMagnitude->value());
        float width = ui->widthSignificand->value()*std::pow(10,ui->widthMagnitude->value())/2;
        ui->customPlot->yAxis->setRange(level - width, level + width);
        ui->levelDisplay->display(level);
        ui->widthDisplay->display(width*2);
    }
    ui->customPlot->replot();

    ulong numberOfPoints = 0;
    for (int i = 0; i < numberOfMeasurements_; ++i)
        numberOfPoints += ui->customPlot->graph(i*2)->data()->count();
    ui->statusBar->showMessage(
        QString("%1 FPS, Total Data points: %2")
        .arg(1000.0/double(plotRefreshTimer_.interval()), 0, 'f', 2)
        .arg(numberOfPoints)
        , 0);
}



void MainWindow::updateData() {
    float actualData[10];
    sharedTable_.readData(actualData);
    if (actualData[0] > previousTime) {
        previousTime = actualData[0];
        for (int i = 0; i < numberOfMeasurements_; ++i) {
            ui->customPlot->graph(i*2)->addData(previousTime, actualData[i+1]); // line
            ui->customPlot->graph(i*2)->removeDataBefore(previousTime-10); // line
            ui->customPlot->graph(i*2+1)->clearData(); // dot
            ui->customPlot->graph(i*2+1)->addData(previousTime, actualData[i+1]); // dot
            if (actualData[i+1] > yMax) yMax = actualData[i+1];
            else if (actualData[i+1] < yMin) yMin = actualData[i+1];
        }
    }
    if (actualData[0] < previousTime) {
        previousTime = actualData[0];
        yMax = 0.5f;
        yMin = -0.5f;
        for (int i = 0; i < numberOfMeasurements_; ++i) {
            ui->customPlot->graph(i*2)->clearData(); // line
            ui->customPlot->graph(i*2+1)->clearData(); // dot
        }
    }
}

