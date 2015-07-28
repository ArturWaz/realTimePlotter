
add_library(qCustomPlot qcustomplot.cpp qcustomplot.h)

qt5_use_modules(qCustomPlot Widgets PrintSupport)