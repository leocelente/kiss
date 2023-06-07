# KISS

**C implementation of the Keep-It-Simple-Stupid Terminal-Node-Controller (KISS-TNC) protocol**

## Build

```shell
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Run Tests

```shell
$ make test
```

## Example 

There is an example that communicates two executables using the `stdout` -> Named Pipe -> `stdin`. After building, run the `scripts/run_example.sh`

### Transmitting

```c
kiss_init(&kiss);
uint8_t msg[] = "Keep It Simple Stupid (KISS)";
kiss_send(&kiss, CMD_DATA, msg, sizeof(msg));
```

### Receiving

```c
kiss_init(&kiss);
while (!done) {
    uint8_t byte = getc(stdin);
    kiss_ingest_byte(&kiss, byte);
}
```

## Intended Use

The main use-case for this library is communication via some sort of streaming serial interface, where there is no defined "frame". Be it simple UART or radio.

## Diferences from the Original KISS

The original KISS protocol is an adaptation of the Serial Line Internet Protocol (SLIP), adding special commands to configure TNCs. This version, removes all predefined commands, but keeps the "command" field. So that you can still differentiate plain-old-data from a configuration command. What the configuration does is up to the user.

# Reference
[Description of the KISS-TNC Protocol](http://www.ka9q.net/papers/kiss.html)

[Wikipedia KISS](https://en.wikipedia.org/wiki/KISS_(TNC))
[Wikipedia SLIP](https://en.wikipedia.org/wiki/Serial_Line_Internet_Protocol)