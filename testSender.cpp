//
// Created by geox on 28.07.15.
//


#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <cmath>

#include "SharedTable.h"


using namespace std;


void test1() {

    SharedTable<float> sharedTable("realTimePlotter_float",boost::interprocess::read_write);

    float table[10] = {};


    for (float time = .0f; time < 10.f; time += 0.1) {

        table[0] = time;
        table[1] = sin(time);
        table[2] = cos(time);

        sharedTable.writeData(table,10);
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }

}



int main() {

    test1();

}