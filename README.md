# robot-controller

Simulated client-server system for robot controller. The system is divided into two
parts: a client and a server. The client sends some commands to the server to
control the mechanic of the robot.

This is the final project
of the Real-Time Operating Systems discipline
of the Computer Engineering course
at the Federal University of Santa Maria
in the semester 2018/1.

## Instalation

Clone the repository and enter into the directory:

```sh
git clone https://github.com/NelsonWeirich/robot-controller.git
cd robot-controller
```

Enter into the "cliente" directory and compile the source code with `make`:

```sh
cd cliente
```

Enter into the "servidor" directory and compile the source code with `make`:

```sh
cd ../servidor
make
```

## Usage

Open two terminal windows. Enter into the `robot-controller` directory in the
both terminal.

In one, enter into the "servidor" directory and run it with port number
(>= 1024). The server will run in the localhost:

```sh
cd servidor
./servidor 8080
```

In the another one, enter into the "cliente" directory and run it with the
hostname and port number:

```sh
cd cliente
./cliente localhost 8080
```

In the client terminal, you cand send the following commands:

* `right [NN]`
* `left [NN]`
* `forward [NN]`
* `back [NN]`
* `stop`
* `exit`

Where `NN` indicates the number of units to move.

## Contributing

This project is archived. For now, it will no longer receive contributions.

## Credits

**Nelson Weirich** <https://github.com/NelsonWeirich>

## Licence

This project, including all its documentation, is licensed under the
MIT License. These means that anyone can do
almost anything they want with this project. See the LICENSE file for
more details.
