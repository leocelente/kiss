#include <kiss.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

uint8_t rx_buffer[256] = {0};

kiss_error_t tx_byte(uint8_t byte) {
  return write(STDOUT_FILENO, &byte, 1) == 1;
}

void on_msg(kiss_cmd_t type, uint8_t *buffer, size_t len, bool overflow) {}

int main(int argc, char **argv) {
  kiss_t kiss = {.sender = tx_byte,
                 .callback = on_msg,
                 .rx_buffer = rx_buffer,
                 .rx_buffer_len = sizeof(rx_buffer)};

  kiss_init(&kiss);
  uint8_t msg[] = "Keep It Simple Stupid (KISS)";
  kiss_send(&kiss, CMD_DATA, msg, sizeof(msg));
}