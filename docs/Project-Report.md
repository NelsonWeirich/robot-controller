# DESIGN OF A SIMULATOR FOR HANDLING ROBOT CONTROL USING CLIENT-SERVER COMMUNICATION

Report of the final project of the Real-Time Operating Systems
discipline of the Computer Engineering course
at the Federal University of Santa Maria in the semester 2018/1. This report
was written and presented in 2018.


## 1 Objective

Develop an embedded software simulator containing two parts:

* Control interface (monitor): which sends and receives information via the network to the simulator software;
* Simulator: software simulating an embedded program, where you must implement at least three periodic tasks and use notions of concurrence (mutex condition variables).

### 1.1 Requirements

* Implementation of (at least) 3 periodic tasks;
* Network communication;
* Concurrent information and concurrency control (at least, use of 1 mutex and 1 conditional variable);
* Sending and receiving commands from the control interface to the simulator (at least 3
commands sent between control interface and simulator).

## 2. Introduction

A robot can perform many tasks according to the environment in which it is inserted. In addition, it is necessary to have stability control when it is stopped or even in movement. It can also be controlled remotely or be fully autonomous in its tasks. Robots for manipulating surgeries or for welding industrial parts are some examples of remotely controlled robot.

To achieve the objective of this work, a client-server communication system will be developed for simulation of a controlled Cartesian coordinate manipulator robot remotely. This will be a simulated system, i.e., the final tasks will not be performed for triggering any mechanism, it will only be a representation of the functioning of a real system.

The user will be able to interact with the machine using keywords to define the task to perform. By sending commands through a communication interface, the user controls the machine's actions and receives responses from the system's state sensors. Therefore, tasks to control communication and tasks to control the measurement system will be necessary.

## 3. Development

Three periodic tasks will be created which will be responsible for maintaining the stability of the robot on the three axes and informing the position of the robotic claw.

At the implementation level, the commands modify global system state variables that will be accessed by the periodic tasks described. To guarantee access to these variables, the mutex and condition variables will be used. These tasks will be staggered according to the priority that it exerts over the system, following according to a real time system.

The user will have two communication interface options: CLI (command line interface) or GUI (graphical user interface) - to facilitate interaction with the system. For GUI interface, the GTK library for C language will be used.

While the tasks are being scheduled and executed in the simulator, the robot maintains its stability guaranteed by the same tasks in deadline time. When the controller sends a command via the terminal (client interface), the server receives and filters it to command the related action. At this point, the execution of the current task is interrupted and, with guaranteed access to a critical section and conditioned to a specific variable, a change in the state of the system is carried out according to the command sent. The server sends a response to the client about the operation performed. The system returns to its normal running state.

The following initial commands to control the system will be defined:

* `right`: moves to the right;
* `left`: moves to the left;
* `forward`: moves to the front;
* `back`: moves backwards;
* `stop`: stops movement;
* `exit`: disconnects from the simulator.

The movement commands must have a parameter that defines the amount of movement that must be performed.

Some more implementation details may be added as the project is developed. Therefore, they will be added in the following reports.

## 4. Implementation

### 4.1 Part I

Based on the ideas described, the code that follows this document was written. This is a primitive system that works according to specifications. However, there are improvements to be made until the final delivery, such as, for example, improving the organization of some parts and handling information in the tasks created.

Three periodic tasks were created: one for linear position control, one for angular position control and one for speed control. For each task, until the moment, within an infinite loop, the value of the variable is printed from a condition defined as differing from zero (this condition will be improved until the end of the project). This piece of code is protected using mutex, as the variable can only be accessed with guarantees that it is not in use by another system task.

In the server code, the treatment of the input parameters was carried out and, after validation, the connection settings and signal set configuration are set, and the three periodic tasks are created. After that, the execution enters an infinite loop where, initially, it is waiting for a connection with the client through the function call `accept()`. In the client code, the input parameters are treated and the connection configuration is made in the same way. After that, the connection is made through the `connect()` function.

When the connection is established, the server creates a thread for it, which will be responsible for maintaining client-server communication. When initiating this communication, the server sends an incoming message to the client that is waiting for this sending to create the communication thread with the server. Now, the client is waiting for user commands, while the server is waiting for client data to be sent.

The user enters a command, the client sends it to the server and waits for a response from it. The server receives the information, processes and executes something according to what was sent (if it is something outside the set of commands, it is recommended that the user type the `help` option to help him). For the `right` and` left` commands, the angular position variable is increased or decreased. For the `forward` and` back` commands, the linear position variable is increased or decreased. And for the `stop` command, the speed variable is reset to zero (something will be done to change the speed more or less for the robot). For all of these treatments, there are guarantees of access to the variables through mutex.

Thus, after processing, the server responds to the client with a message that will be shown to the user on his terminal. When the user wants to disconnect from the server, just type `exit`. The server will interpret this command as a termination of connection and will send the client a message with the same word, `exit`. The customer, upon receiving this data, will interpret it as the end of the connection. Therefore, both will end the execution of your current thread. In this way, the client will end the execution of the main program and end. On the other hand, when the server returns to the main program, it remains in the infinite loop and will wait for a new connection (function `accept()`). Now, a new user will be able to try a new connection to the server that will start the same process.

While this communication is carried out, the periodic tasks are carried out from time to time, following the regulation established for each one. When in execution, it sends the value present in the variable corresponding to its monitoring to the terminal. At the server terminal, this information appears among the connection information with the client. This will be improved for the final product.

For reasons of development time, the graphical interface was not implemented. Thus, only the command line interface, CLI, developed until then, was presented.

## 4.2 Part II

In this second step, the organization of the code was improved. The lperiodic library was added to control the periodicity of defined tasks, improved the execution of periodic tasks and corrected some implementation errors.

The general functioning of the system remained the same. In this way, the user enters a command to control the robot's movements and, through the client's task, it is sent to the server. This is responsible for filtering the command and performing an action according to what is necessary. Then, at the end, it returns a message to the customer that passes it on to the user.

It was thought to add, in place of the server's terminal prints, actions to send commands to a serial port where there would be a device that could handle this, such as a microcontroller. Thus, it would be a simulation more focused on an application, where the server would have control of a third connection (the microcontroller) for sending actions.

The organization of directories is separated by application. A directory responsible for storing customer codes, with separation of libraries and source codes. And another directory responsible for storing server codes, with separation of libraries and source codes, in the same way.

## 5. Conclusions

To build a system for controlling the manipulator robot using a client-server connection, two main programs were developed in C language code. These are responsible for client-server communication. The client is in charge of receiving commands from the user and sending them to the server and receiving the user's response and sending it to the user back through the terminal. The server is responsible for responding to requests that the client sends in the form of commands in the established set.

Following the specifications, the three periodic tasks were created, which are responsible for controlling three state variables: linear position, angular position and speed. To guarantee access to these global variables, avoiding unwanted results, the concept of mutex was used.

In the second part of development, the system was already in operation, but it was a primitive system. In order to finalize it, some parts of code were modified. Due to the development of other projects in parallel, the graphic inter-face part was compromised. But with a good command line interface, in the terminal, in the robust and classic way, for a programmer, it can be better than a friendly graphical interface. The contrary is better to present a didactic system to those who want to control a robot in a simpler and more direct way. However, the operation will be better completed.

The final system was as proposed and as previously planned. Therefore, it is possible to remotely control a robot manipulating Cartesian coordinates, at least at the simulation level. The same system can be applied to a real control with time guarantees, where it is necessary to use the concepts of Real-Time Operating Systems.

## References

KERRISK, M. **Linux man pages online**. 2018. Available at: <http://man7.org/linux/man-pages/index.html>.

PROJECT, T. G. **Getting Started with GTK+**. Available at: <https://developer.gnome.org/gtk3/stable/gtk-getting-started.html>.

SIMPLÍCIO, P. V. G.; LIMA, B. R.; JUNKES, J. A. **Manipuladores Robóticos Industriais**. 2016. Available at: <https://periodicos.set.edu.br/index.php/cadernoexatas/article/viewFile/3572/1950>.
