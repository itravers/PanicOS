/**serial.c
 * Author: Isaac Assegai
 * Date  : 10/5/2017
 * Implements functions for communicating over serial ports
 */

#include <kernel/serial.h>

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor){
  outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
  outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
  outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

/** serial_configure_line:
  * Configures the line of the given serial port. The port is set
  * to have a deta length of 8 bits, no parity bits, one stop bit
  * and break control disabled.
  *
  * d: Enables (d = 1) or disables (d = 0) DLAB
  * b: If break control is enabled (b = 1) or disabled (b = 0)
  * prty: The number of parity bits to use
  * s: The number of stop bits to use (s = 0 equals 1, s = 1 equals 1.5 or 2)
  * dl: Describes the length of the data
  * @param com The Serial Port to Configure
  */
void serial_configure_line(unsigned short com){
  /* Bit:      | 7 | 6 | 5 4 3 | 2 | 1 0 |
   * Content   | d | b | prty  | s |  dl |
   * Value     | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03 */
  outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_configure_buffers:
  * Configures the buffers of the given serial port. 
  * lvl: How many bytes should be stored in the FIFO buffers
  * bs: If the buffers should be 16 or 64 bytes large
  * r: Reserved for future use
  * dma: How the serial port data should be accessed
  * clt: Clear the transmission FIFO buffer
  * clr: Clear the receiver FIFO buffer
  * e: If the FIFO buffer should be enabled or not
  */
void serial_configure_buffers(unsigned short com){
  /*
   * Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
   * Content: | lvl | bs | r | dma | clt | clr | e |
   * Value:   | 1 1 | 0  | 0 | 0   | 1   | 1   | 1 | = 0xC7 
   * 
   *  Enables FIFO
   *  Clear both receiver and transmission FIFO queues
   * Use 14 bytes as size of queue */
  outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

/** serial_configure_modem:
  * Configures the modem control register.
  * We want RTS and DTS to be 1 which means we are
  * Ready to send data.
  */
void serial_configure_modem(unsigned short com){
  /*
   * Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
   * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
   * Value:   | 0 | 0 | 0  | 0  | 0   | 0   | 1   | 1   | = 0x03 */
  outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int){
  /* 0x20 = 0010 0000 */ //The transmit fifo queue bit
  return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

