// Nicolas Hasbun A, 2018

// This is a wrapper library from the API delivered from Altera
// It's configured to make use of Tx + FIFO for now
// Rx isn't configured so far

#include "uart_16550_core_lib.h"

void uart_init(UART * uart_p, uint32_t baseadd)
{
  uart_p -> uart_device = ALT_16550_DEVICE_ALTERA_16550_UART;

  uart_p -> status = alt_16550_init(
    uart_p -> uart_device, (void*)baseadd, UART_FREQ, & uart_p -> uart_handle);

  printf("%s\n", "");
  printf("%s\n", "");
  printf("%s\n", "");
  printf("%s\n", "");

  if(uart_p -> status == ALT_E_SUCCESS)
    printf("%s\n", "Uart 16550 init: OK");
  else if(uart_p -> status == ALT_E_ERROR)
    printf("%s\n", "Uart 16550 init: ERROR");
  else if(uart_p -> status == ALT_E_BAD_ARG)
    printf("%s\n", "Uart 16550 init: BAD ARGUMENT");
  else if(uart_p -> status == ALT_E_BAD_CLK)
    printf("%s\n", "Uart 16550 init: CLOCK REQUIRED");
  else
    printf("%s\n", "Uart 16550 init: unrecognized error");
}

void uart_config(UART * uart_p)
{
  printf("%s\n", "*** CONFIG MODULE STEPS ***");

  // Safe reset
  uart_p -> status = alt_16550_reset(& uart_p -> uart_handle);
  if(uart_p -> status == ALT_E_SUCCESS)
    printf("%s\n", "Uart 16550 reset: OK");
  else if(uart_p -> status == ALT_E_ERROR)
    printf("%s\n", "Uart 16550 reset: ERROR");
  else if(uart_p -> status == ALT_E_BAD_ARG)
    printf("%s\n", "Uart 16550 reset: BAD ARGUMENT");
  else
    printf("%s\n", "Uart 16550 reset: unrecognized error");

  // Set baurate and check
  uart_p -> status = alt_16550_baudrate_set(& uart_p -> uart_handle, BAUDRATE);
  if(uart_p -> status == ALT_E_SUCCESS)
    printf("%s %i %s\n", "Uart 16550 set baudrate:", BAUDRATE, "OK");
  else if(uart_p -> status == ALT_E_ERROR)
    printf("%s\n", "Uart 16550 set baudrate: ERROR");
  else if(uart_p -> status == ALT_E_BAD_ARG)
    printf("%s\n", "Uart 16550 set baudrate: BAD ARGUMENT");
  else if(uart_p -> status == ALT_E_ARG_RANGE)
    printf("%s\n", "Uart 16550 set baudrate: BAD BAUDRATE RANGE");
  else
    printf("%s\n", "Uart 16550 set baudrate: unrecognized error");

  uint32_t baudrate = 0;
  alt_16550_baudrate_get(& uart_p -> uart_handle, &baudrate);
  printf("Baudrate on register: %i\n", baudrate);

  // Enabling fifo for data
  // FIFO is automatically cleared
  uart_p -> status = alt_16550_fifo_enable(& uart_p -> uart_handle);
  if(uart_p -> status == ALT_E_SUCCESS)
    printf("%s\n", "Uart 16550 FIFO enable: OK");
  else if(uart_p -> status == ALT_E_ERROR)
    printf("%s\n", "Uart 16550 FIFO enable: ERROR");
  else if(uart_p -> status == ALT_E_BAD_ARG)
    printf("%s\n", "Uart 16550 FIFO enable: BAD ARGUMENT");
  else
    printf("%s\n", "Uart 16550 FIFO enable: unrecognized error");

  // Tx idle signal
  uart_p -> status = alt_16550_int_enable_tx(& uart_p -> uart_handle);
  if(uart_p -> status == ALT_E_SUCCESS)
    printf("%s\n", "Uart 16550 enabling FIFO tx interrupt: OK");
  else if(uart_p -> status == ALT_E_ERROR)
    printf("%s\n", "Uart 16550 enabling FIFO tx interrupt: ERROR");
  else if(uart_p -> status == ALT_E_BAD_ARG)
    printf("%s\n", "Uart 16550 enabling FIFO tx interrupt: BAD ARGUMENT");
  else
    printf("%s\n", "Uart 16550 enabling FIFO tx interrupt: unrecognized error");

  // Rx data signal and timeout
  uart_p -> status = alt_16550_int_enable_rx(& uart_p -> uart_handle);
  if(uart_p -> status == ALT_E_SUCCESS)
    printf("%s\n", "Uart 16550 enabling FIFO rx interrupt: OK");
  else if(uart_p -> status == ALT_E_ERROR)
    printf("%s\n", "Uart 16550 enabling FIFO rx interrupt: ERROR");
  else if(uart_p -> status == ALT_E_BAD_ARG)
    printf("%s\n", "Uart 16550 enabling FIFO rx interrupt: BAD ARGUMENT");
  else
    printf("%s\n", "Uart 16550 enabling FIFO rx interrupt: unrecognized error");


  // Tx trigger event
  uart_p -> status = alt_16550_fifo_trigger_set_tx(& uart_p -> uart_handle,
    ALT_16550_FIFO_TRIGGER_TX_EMPTY);
  if(uart_p -> status == ALT_E_SUCCESS)
    printf("%s\n", "SETTING FIFO TRIGGER TX EMPTY");
  else if(uart_p -> status == ALT_E_ERROR)
    printf("%s\n", "Uart 16550 fifo trigger: ERROR");
  else if(uart_p -> status == ALT_E_BAD_ARG)
    printf("%s\n", "Uart 16550 fifo trigger: BAD ARGUMENT");
  else
    printf("%s\n", "Uart 16550 fifo trigger: unrecognized error");

  // Rx trigger event
  uart_p -> status = alt_16550_fifo_trigger_set_rx(& uart_p -> uart_handle,
    ALT_16550_FIFO_TRIGGER_RX_ANY);
  if(uart_p -> status == ALT_E_SUCCESS)
    printf("%s\n", "SETTING FIFO RX TRIGGER ANY DATA");
  else if(uart_p -> status == ALT_E_ERROR)
    printf("%s\n", "Uart 16550 fifo rx trigger: ERROR");
  else if(uart_p -> status == ALT_E_BAD_ARG)
    printf("%s\n", "Uart 16550 fifo rx trigger: BAD ARGUMENT");
  else
    printf("%s\n", "Uart 16550 fifo rx trigger: unrecognized error");

  // Enable UART after config
  uart_p -> status = alt_16550_enable(& uart_p -> uart_handle);
  if(uart_p -> status == ALT_E_SUCCESS)
    printf("%s\n", "Uart 16550 enable: OK");
  else if(uart_p -> status == ALT_E_ERROR)
    printf("%s\n", "Uart 16550 enable: ERROR");
  else if(uart_p -> status == ALT_E_BAD_ARG)
    printf("%s\n", "Uart 16550 enable: BAD ARGUMENT");
  else
    printf("%s\n", "Uart 16550 enable: unrecognized error");
}

void uart_tx(UART * uart_p, char * buf, uint32_t len)
{
  // Uart transmission with self-made safe writing, to avoid buffer
  // overrun

  // // Uart writing safe ** NOT WORKING
  // status = alt_16550_fifo_write_safe(&uart, "hola", 4, true);
  // if(status == ALT_E_SUCCESS) printf("%s\n", "4 bytes added to FIFO");

  // Uart writing not safe ** WORKING
  uint8_t buf_aux_size = 100;

  char * buf_aux = (char*)malloc(buf_aux_size * sizeof(char));
  int bytes_faltantes = len;

  while(true) {
    if(check_tx_idle(uart_p)) {
    // esta rutina se ejecuta solo si el modulo está liberado
      if(bytes_faltantes < buf_aux_size) buf_aux_size = bytes_faltantes;

      memcpy(buf_aux, buf, buf_aux_size);
      alt_16550_fifo_write(& uart_p -> uart_handle, buf_aux, buf_aux_size);

      bytes_faltantes = bytes_faltantes - buf_aux_size;
      if(bytes_faltantes <= 0) break;
    }
  }
}

bool uart_rx(UART * uart_p)
{
  char * buf = (char*)malloc(10 * sizeof(char));

  if(check_rx_data(uart_p)) {
    alt_16550_fifo_read(& uart_p -> uart_handle, buf, 1);
    printf("%c", *buf);
  }
  return true;
}

bool check_tx_idle(UART * uart_p)
{
  // Get int status
  ALT_16550_INT_STATUS_t int_status;
  alt_16550_int_status_get(& uart_p -> uart_handle, &int_status);
  if (int_status == ALT_16550_INT_STATUS_TX_IDLE ||
      int_status == ALT_16550_INT_STATUS_NONE)
    return true;
  else
    return false;
}

bool check_rx_data(UART * uart_p)
{
  // Get int status
  ALT_16550_INT_STATUS_t int_status;
  alt_16550_int_status_get(& uart_p -> uart_handle, &int_status);
  if(int_status == ALT_16550_INT_STATUS_RX_DATA)
    return true;
  else
    return false;
}
