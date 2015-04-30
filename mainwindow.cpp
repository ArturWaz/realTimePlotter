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


MainWindow::MainWindow(size_t numberOfMeasurements, QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow), numberOfMeasurements_(numberOfMeasurements)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
  
  if (numberOfMeasurements_ == 0)
    setupRealtimeDataDemo(ui->customPlot);
  else if (numberOfMeasurements_ < 9)
    setupRealTimePlot(ui->customPlot);
  else
    std::exit(-1);

  setWindowTitle("Plot");
  statusBar()->clearMessage();
  ui->customPlot->replot();
}


void MainWindow::setupRealTimePlot(QCustomPlot *customPlot) {
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif

  size_t num = 0;
  if (num < numberOfMeasurements_) {

      customPlot->addGraph();
      customPlot->graph(num)->setPen(QPen(Qt::blue));
      customPlot->addGraph();
      customPlot->graph(num+1)->setPen(QPen(Qt::blue));
      customPlot->graph(num+1)->setLineStyle(QCPGraph::lsNone);
      customPlot->graph(num+1)->setScatterStyle(QCPScatterStyle::ssDisc);

      num += 2;
  }
  if (num < numberOfMeasurements_) {

      customPlot->addGraph();
      customPlot->graph(num)->setPen(QPen(Qt::red));
      customPlot->addGraph();
      customPlot->graph(num+1)->setPen(QPen(Qt::red));
      customPlot->graph(num+1)->setLineStyle(QCPGraph::lsNone);
      customPlot->graph(num+1)->setScatterStyle(QCPScatterStyle::ssDisc);

      num += 2;
  }
  if (num < numberOfMeasurements_) {

      customPlot->addGraph();
      customPlot->graph(num)->setPen(QPen(Qt::green));
      customPlot->addGraph();
      customPlot->graph(num+1)->setPen(QPen(Qt::green));
      customPlot->graph(num+1)->setLineStyle(QCPGraph::lsNone);
      customPlot->graph(num+1)->setScatterStyle(QCPScatterStyle::ssDisc);

      num += 2;
  }
  if (num < numberOfMeasurements_) {

      num += 2;
  }
  if (num < numberOfMeasurements_) {

      num += 2;
  }
  if (num < numberOfMeasurements_) {

      num += 2;
  }
  if (num < numberOfMeasurements_) {

      num += 2;
  }
  if (num < numberOfMeasurements_) {

      num += 2;
  }
  if (num < numberOfMeasurements_) {

      num += 2;
  }

  customPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(2);
  customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(refreshPlot()));
  dataTimer.start(20); // Interval 0 means to refresh as fast as possible
}



void MainWindow::refreshPlot() {
  ui->customPlot->replot();
  ulong numberOfPoints = 0;
  for (int i = 0; i < numberOfMeasurements_; ++i)
      numberOfPoints += ui->customPlot->graph(i*2)->data()->count();
  ui->statusBar->showMessage(
        QString("%1 FPS, Total Data points: %2")
        .arg(double(1000.0/double(dataTimer.interval())), 0, 'f', 2)
        .arg(numberOfPoints)
        , 0);
}


void MainWindow::updateData(DataFromExternalSource &newData) {
  for (int i = 0; i < newData.numberOfMeasurements_; ++i) {
      ui->customPlot->graph(i*2)->addData(newData.time_, newData.elements_[i]); // line
      ui->customPlot->graph(i*2)->removeDataBefore(newData.time_-8); // line
      ui->customPlot->graph(i*2)->rescaleValueAxis(); // line
      ui->customPlot->graph(i*2+1)->clearData(); // dot
      ui->customPlot->graph(i*2+1)->addData(newData.time_, newData.elements_[i]); // dot
  }
}



void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
  
  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */

  // FIRST LINE
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->addGraph(); // blue dot
  customPlot->graph(1)->setPen(QPen(Qt::blue));
  customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
  
  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(2);
  customPlot->axisRect()->setupFullAxesBox();
  
  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
  
  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}


void MainWindow::realtimeDataSlot()
{
  // calculate two new data points:
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  double key = 0;
#else
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
#endif
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.01) // at most add point every 10 ms
  {
    double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
    double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;

    // ADD DATA TO FIRST DATA LINE AND DOT
    ui->customPlot->graph(0)->addData(key, value0); // line
    ui->customPlot->graph(0)->removeDataBefore(key-8); // line
    ui->customPlot->graph(0)->rescaleValueAxis(); // line
    ui->customPlot->graph(1)->clearData(); // dot
    ui->customPlot->graph(1)->addData(key, value0); // dot

    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
  ui->customPlot->replot();
  
  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->customPlot->graph(0)->data()->count()+ui->customPlot->graph(1)->data()->count())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}



MainWindow::~MainWindow()
{
  delete ui;
}






































