
#include "ina219.h"
#include "iostream"
#include "fstream"
#include "date.h"
#include "chrono"

int main(){

    INA219 ina219(0.1, 3, 0x45);
    ina219.calibrate(6, 0.4, 5);

    std::fstream writeOut("ina219.data", std::fstream::binary|std::fstream::app);
    auto time = date::format("%T", std::chrono::system_clock::now());
    writeOut << time;
    // get the sensor data
    ina219.wake();
    float current = ina219.current();
    float voltage = ina219.voltage();
    float power = ina219.power();
    float supply_voltage = ina219.supply_voltage();
    ina219.sleep();
    std::string data;
    data  = "Current, " + std::to_string(current) + ", Voltage, " + std::to_string(voltage) + ", Power, " + std::to_string(power) + ", Supply Voltage" +
            std::to_string(supply_voltage);

    writeOut.write(data.c_str(), data.size());


}