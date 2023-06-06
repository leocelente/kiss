#include <kiss.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t rx_buffer[256] = {0};
uint8_t tx_buffer[512] = {0};
size_t tx_index = 0;

bool done = false;
void on_msg(kiss_cmd_t type, uint8_t *buffer, size_t len, bool overflow) {
  done = true;
}

kiss_error_t send_byte(uint8_t byte) {
  tx_buffer[tx_index++] = byte;
  return E_OK;
}

static uint8_t const SPECIAL[] = {
    FESC,  TFESC, FEND,  TFEND, FESC,  TFESC, TFEND, FEND,  FESC,  FEND, TFESC,
    TFEND, FESC,  FEND,  TFEND, TFESC, FESC,  TFEND, TFESC, FEND,  FESC, TFEND,
    FEND,  TFESC, TFESC, FESC,  FEND,  TFEND, TFESC, FESC,  TFEND, FEND, TFESC,
    FEND,  FESC,  TFEND, TFESC, FEND,  TFEND, FESC,  TFESC, TFEND, FESC, FEND,
    TFESC, TFEND, FEND,  FESC,  FEND,  FESC,  TFESC, TFEND, FEND,  FESC, TFEND,
    TFESC, FEND,  TFESC, FESC,  TFEND, FEND,  TFESC, TFEND, FESC,  FEND, TFEND,
    FESC,  TFESC, FEND,  TFEND, TFESC, FESC,  TFEND, FESC,  TFESC, FEND, TFEND,
    FESC,  FEND,  TFESC, TFEND, TFESC, FESC,  FEND,  TFEND, TFESC, FEND, FESC,
    TFEND, FEND,  FESC,  TFESC, TFEND, FEND,  TFESC, FESC};

static uint8_t const EMPTY[] = {};
static uint8_t const NORMAL[] = {0x01, 0x02, 0x03, 0x04};
static uint8_t const STRING[] = "This is a Test String";
static uint8_t RANGE[256] = {0};
static uint8_t OVERFLOW[384] = {0};

/**
 * @brief Test sending a message DATA, and checks that the receive buffer
 * contains DATA
 *
 * @param data
 * @param len
 * @param should_fail
 * @return true Failed with Error
 * @return false No errors
 */
bool test(uint8_t const *data, size_t len) {
  tx_index = 0;
  kiss_t kiss = {.sender = send_byte,
                 .callback = on_msg,
                 .rx_buffer = rx_buffer,
                 .rx_buffer_len = sizeof(rx_buffer)};

  kiss_init(&kiss);
  kiss_send(&kiss, CMD_DATA, data, len); // fills tx_buffer

  for (int i = 0; i < tx_index; ++i) {
    kiss_ingest_byte(&kiss, tx_buffer[i]); // fills rx_buffer
  }

  if (!done)
    return true;

  done = false;
  int diff = memcmp(rx_buffer, data, len);
  return (diff != 0);
}

int main(int argc, char **argv) {
  int failures = 0;
  failures += test(SPECIAL, sizeof(SPECIAL));
  failures += test(EMPTY, sizeof(EMPTY));
  failures += test(STRING, sizeof(STRING));
  failures += test(NORMAL, sizeof(NORMAL));
  for (int i = 0; i < sizeof(RANGE); ++i) {
    RANGE[i] = i;
  }
  failures += test(RANGE, sizeof(RANGE));
  for (int i = 0; i < sizeof(OVERFLOW); ++i) {
    OVERFLOW[i] = i;
  }
  failures += 1 != test(OVERFLOW, sizeof(OVERFLOW));
  for (int i = 0; i < sizeof(rx_buffer); ++i) {
    printf("%.2X ", rx_buffer[i]);
    if (i % 8 == 7)
      printf(" ");
    if (i && i % 16 == 15)
      printf("\r\n");
  }
  printf("\r\n");

  return failures;
}