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

#include <QApplication>
#include <string>
#include <iostream>
#include <stdexcept>
#include "mainwindow.h"
#include <boost/interprocess/shared_memory_object.hpp>


int main(int argc, char *argv[])
{
    boost::interprocess::named_mutex::remove((std::string("realTimePlotter_float")+std::string("_mtx")).c_str());
    boost::interprocess::shared_memory_object::remove("realTimePlotter_float");
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QApplication::setGraphicsSystem("raster");
#endif

  int numberOfMeasurements = 3;
  if (argc == 2) {
    try {
          numberOfMeasurements = std::stoi(std::string(argv[1]),nullptr);
      } catch(std::invalid_argument &err) {
          std::cerr << "Second argument must be an integer.\n";
          return -1;
      }
  }

  QApplication a(argc, argv);
  MainWindow w(numberOfMeasurements);
  w.show();
  
  return a.exec();
}
