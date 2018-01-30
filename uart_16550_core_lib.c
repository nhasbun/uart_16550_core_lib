// Nicolas Hasbun A, 2018

// This is a wrapper library from the API delivered from Altera
// It's configured to make use of Tx + FIFO for now
// Rx isn't configured so far

#include "uart_16550_core_lib.h"

void uart_init(UART * uart_p, uint32_t baseadd)
{
  uart_p -> uart_device = ALT_16550_DEVICE_ALTERA_16550_UART;



  printf("%s\n", "");
  printf("%s\n", "");
  printf("%s\n", "");
  printf("%s\n", "");

  uart_p -> status = alt_16550_init(
    uart_p -> uart_device, (void*)baseadd, UART_FREQ, & uart_p -> uart_handle);
  check_error(uart_p, "INIT");
}

void uart_config(UART * uart_p, uint32_t baudrate)
{
  printf("%s\n", "*** CONFIG MODULE STEPS ***");

  // Safe reset
  uart_p -> status = alt_16550_reset(& uart_p -> uart_handle);
  check_error(uart_p, "RESET");

  // Set baurate and check
  uart_p -> status = alt_16550_baudrate_set(& uart_p -> uart_handle, baudrate);
  check_error(uart_p, "BAUDRATE");

  uint32_t baudrate_reg = 0;
  alt_16550_baudrate_get(& uart_p -> uart_handle, &baudrate_reg);
  printf("Baudrate on register: %i\n", baudrate_reg);

  // Enabling fifo for data
  // FIFO is automatically cleared
  uart_p -> status = alt_16550_fifo_enable(& uart_p -> uart_handle);
  check_error(uart_p, "FIFO ENABLE");

  // Tx trigger event
  uart_p -> status = alt_16550_fifo_trigger_set_tx(& uart_p -> uart_handle,
    ALT_16550_FIFO_TRIGGER_TX_ALMOST_EMPTY );
  check_error(uart_p, "TRIGGER SET TX");

  // Rx trigger event
  uart_p -> status = alt_16550_fifo_trigger_set_rx(& uart_p -> uart_handle,
    ALT_16550_FIFO_TRIGGER_RX_ANY);
  check_error(uart_p, "TRIGGER SET RX");

  // Tx idle signal
  uart_p -> status = alt_16550_int_enable_tx(& uart_p -> uart_handle);
  check_error(uart_p, "INT ENABLE TX");

  // Rx data signal and timeout
  uart_p -> status = alt_16550_int_enable_rx(& uart_p -> uart_handle);
  check_error(uart_p, "INT ENABLE RX");


  // Enable UART after config
  uart_p -> status = alt_16550_enable(& uart_p -> uart_handle);
  check_error(uart_p, "ENABLE UART");

}

void check_error(UART * uart_p, char * buf)
{
  ALT_STATUS_CODE status = uart_p -> status;

  if(status == ALT_E_SUCCESS)
    printf("%s %s %s\n", "Uart 16550", buf, ": OK");
  else if(status == ALT_E_ERROR)
    printf("%s %s %s\n", "Uart 16550", buf, ": ERROR");
  else if(status == ALT_E_BAD_ARG)
    printf("%s %s %s\n", "Uart 16550", buf, ": BAD ARGUMENT");
  else if(uart_p -> status == ALT_E_BAD_CLK)
    printf("%s %s %s\n", "Uart 16550", buf, ": CLOCK REQUIRED");
  else if(uart_p -> status == ALT_E_ARG_RANGE)
    printf("%s %s %s\n", "Uart 16550", buf, ": BAD RANGE");
  else
    printf("%s %s %s\n", "Uart 16550", buf, ": unrecognized error");
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
    if(bytes_faltantes < buf_aux_size) buf_aux_size = bytes_faltantes;

    memcpy(buf_aux, buf, buf_aux_size);
    alt_16550_fifo_write(& uart_p -> uart_handle, buf_aux, buf_aux_size);

    bytes_faltantes = bytes_faltantes - buf_aux_size;
    while(!check_tx_idle(uart_p));
    // esperar que la transmision finalice

    if(bytes_faltantes <= 0) break;

    // avance del puntero
    buf = buf + buf_aux_size;
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
  if (int_status == ALT_16550_INT_STATUS_TX_IDLE)
    return true;
  // else if (int_status == ALT_16550_INT_STATUS_NONE)
  //   return true;
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

void clear_uart_fifo(UART * uart_p)
{
  alt_16550_fifo_clear_all(& uart_p -> uart_handle);
}
