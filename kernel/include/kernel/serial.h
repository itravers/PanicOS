/**serial.h
 * Author: Isaac Assegai
 * Date  : 10/5/2017
 * Defines an interface for working with the serial port
 */
#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdint.h>
#include <stdlib.h> //for inb()

/* The I/O ports */

/* All the I/O ports are calculated relative to the data port. This is because
 * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
 * order, but they start at different values.
 */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short, unsigned short);

/** serial_configure_line:
  * Configures the line of the given serial port. The port is set
  * to have a deta length of 8 bits, no parity bits, one stop bit
  * and break control disabled.
  *
  * @param com The Serial Port to Configure
  */
void serial_configure_line(unsigned short);

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
void serial_configure_buffers(unsigned short);

/** serial_configure_modem:
  * Configures the modem control register.
  * We want RTS and DTS to be 1 which means we are
  * Ready to send data.
  */
void serial_configure_modem(unsigned short);

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int);

/* serial_initialze:
 * Sets up the serial port
 */
void serial_initialize(unsigned int);

/* serial_write:
 * writes a character to the serial port
 * When serial_is_transmit_fifo_empty is true
 */
void serial_write(unsigned int, char);

/** serial_received:
 * returns true if data is waiting in the serial port
 * Waiting to be read. 
 */
int serial_received(unsigned short);


/** read_serial
 * Returns the last char in the serial read buffer 
 */
char read_serial(unsigned short);
#endif
