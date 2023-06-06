#include <kiss.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

uint8_t rx_buffer[256] = {0};
bool done = false;

kiss_error_t tx_byte(uint8_t byte) {
  return write(STDOUT_FILENO, &byte, 1) == 1;
}

void on_msg(kiss_cmd_t type, uint8_t *buffer, size_t len, bool overflow) {
  if (overflow) {
    printf("OVERFLOW!\r\n");
  }
  printf("[%s (%lu)]> %s\r\n", type == CMD_CONF ? "CONF" : "DATA", len, buffer);
  done = true;
}

int main(int argc, char **argv) {
  kiss_t kiss = {.sender = tx_byte,
                 .callback = on_msg,
                 .rx_buffer = rx_buffer,
                 .rx_buffer_len = sizeof(rx_buffer)};

  kiss_init(&kiss);
  while (!done) {
    uint8_t byte = getc(stdin);
    kiss_ingest_byte(&kiss, byte);
  }

  /*
      for(int i = 0; i < sizeof(rx_buffer); ++i){
          printf("%.2X ", rx_buffer[i]);
          if(i % 8 == 7) printf(" ");
          if(i && i % 16 == 15) printf("\r\n");
      }
      printf("\r\n");

  */
}