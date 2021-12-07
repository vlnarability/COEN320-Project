# COEN320-Project

Immediately upon running the program, the producer threads will be created and and begin reading the data from the given .csv file.
A consumer thread is also created that receives the data from the producer threads and outputs to the console based upon the current frequency.

In order to change the frequency of the consumed data, a command processor was added.
The user can input the following into the console in change options:
    number + " " + new period (in millisecond)

The number corresponds to which producer should be updated.
  1 - Fuel Consumption
  2 - Engine Speed
  3 - Engine Coolant Temperature
  4 - Current Gear
  5 - Vehicle Speed
  8 - Update All Producers
