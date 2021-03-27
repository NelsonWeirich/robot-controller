# DESIGN OF A SIMULATOR FOR HANDLING ROBOT CONTROL USING CLIENT-SERVER COMMUNICATION 

This is the final project of the Real-Time Operating Systems discipline of the Computer Engineering course at the Federal University of Santa Maria in the semester 2018/1.

## OBJECTIVE

Develop an embedded software simulator containing two parts:

* Control interface (monitor): which sends and receives information via the network to the simulator software;
* Simulator: software simulating an embedded program, where you must implement at least three periodic tasks and use notions of concurrence (mutex condition variables).

## REQUIREMENTS

* Implementation of (at least) 3 periodic tasks;
* Network communication;
* Concurrent information and concurrency control (at least, use of 1 mutex and 1 conditional variable);
* Sending and receiving commands from the control interface to the simulator (at least 3
commands sent between control interface and simulator).
