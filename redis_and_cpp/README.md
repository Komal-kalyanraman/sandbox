# Redis C++ Example

This project demonstrates how to use the `hiredis` library to interact with a Redis database in C++.

## Prerequisites

- C++ compiler
- Redis server
- `hiredis` library

## Installation

### Install Redis Server

On a Linux machine, you can install Redis server using the following commands:

```sh
sudo apt-get update
sudo apt-get install redis-server
```

To start the Redis server, use the following command:

```sh
redis-server
```

### Install `hiredis` Library

You can install the `hiredis` library by cloning the repository and compiling it:

```sh
git clone https://github.com/redis/hiredis.git
cd hiredis
make
sudo make install
```

Here are a few steps you should take to complete installation:

1. Make sure that `hiredis` is correctly installed. You can check this by looking in the directory where it should be installed. This is typically `/usr/local/lib` for libraries installed from source.

2. If the library is installed but the system can't find it, you may need to add the directory containing the library to the `LD_LIBRARY_PATH` environment variable. This tells the system additional directories to look in for shared libraries. You can do this with the following command:

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/your/library
```

Replace `/path/to/your/library` with the actual path to the directory containing `libhiredis.so.1.2.1-dev`.

3. After setting `LD_LIBRARY_PATH`, try running your program again. If it still doesn't work, you may need to run `ldconfig` as root to update the system's library cache:

```sh
sudo ldconfig
```

## Compile and Run the Program

Compile the program with the following command:

```sh
g++ -o main main.cpp -lhiredis
```

Run the program with the following command:

```sh
./program
```

## Troubleshooting

If you encounter a "Connection refused" error, make sure that the Redis server is running and listening on the correct host and port.

If you encounter an "Address already in use" error when starting the Redis server, it means that the port 6379 is already being used by another process. You can find out which process is using port 6379 with the following command:

```sh
sudo lsof -i :6379
```

You can then stop the process with the following command, replacing `PID` with the actual process ID:

```sh
sudo kill -9 PID
```

If the process using port 6379 is another instance of Redis, you can stop it with the following command:

```sh
redis-cli shutdown
```

If you need to run multiple instances of Redis, you can configure each instance to use a different port by editing the Redis configuration file and changing the `port` directive.
