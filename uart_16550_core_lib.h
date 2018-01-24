#include "alt_16550_uart.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct UART_S UART;

#define UART_FREQ  50000000
#define BAUDRATE   38400
#define BUFFERSIZE 10000000

struct UART_S
{
  uint32_t uart_address;
  ALT_16550_DEVICE_t uart_device;
  ALT_16550_HANDLE_t uart_handle;
  ALT_STATUS_CODE status;
};

void uart_init(UART * uart_p, uint32_t baseadd);
void uart_config(UART * uart_p);
void uart_tx(UART * uart_p, char * buf, uint32_t len);
bool uart_rx(UART * uart_p);

// small aux functions
bool check_tx_idle(UART * uart_p);
bool check_rx_data(UART * uart_p);