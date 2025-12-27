#ifndef MCP25625_H
#define MCP25625_H

#include <stdint.h>
#include <stdbool.h>
#include <main.h>

/*   chip select control */

#define MCP25625_CS_PORT GPIOA
#define MCP25625_CS_PIN GPIO_PIN_4

#define MCP25625_CS_LOW() HAL_GPIO_WritePin(MCP25625_CS_PORT, MCP25625_CS_PIN, GPIO_PIN_RESET)
#define MCP25625_CS_HIGH() HAL_GPIO_WritePin(MCP25625_CS_PORT, MCP25625_CS_PIN, GPIO_PIN_SET)

/*   spi instruction set from the datasheet */

#define MCP25625_RESET 0xc0
#define MCP25625_READ  0x03
#define MCP25625_READ_RXB0SIDH 0x90
#define MCP25625_READ_RXB0D0 0x92
#define MCP25625_READ_RXB1SIDH 0x94
#define MCP25625_READ_RXB1D0 0x96
#define MCP25625_WRITE 0x02
#define MCP25625_WRITE_TXB0SIDH 0x40
#define MCP25625_WRITE_TXB0D0 0x41
#define MCP25625_WRITE_TXB1SIDH 0x42
#define MCP25625_WRITE_TXB1D0 0x43
#define MCP25625_WRITE_TXB2SIDH 0x44
#define MCP25625_WRITE_TXB2D0 0x45
#define MCP25625_RX_STATUS 0xB0
#define MCP25625_READ_STATUS 0xA0
#define MCP25625_BIT_MODIFY 0x05
#define MCP25625_RTS_TX0 0x81
#define MCP25625_RTS_TX1 0x82
#define MCP25625_RTS_TX2 0x84
#define MCP25625_RTS_ALL 0x87


/* register map */

#define MCP2515_RXF0SIDH	0x00
#define MCP2515_RXF0SIDL	0x01
#define MCP2515_RXF0EID8	0x02
#define MCP2515_RXF0EID0	0x03
#define MCP2515_RXF1SIDH	0x04
#define MCP2515_RXF1SIDL	0x05
#define MCP2515_RXF1EID8	0x06
#define MCP2515_RXF1EID0	0x07
#define MCP2515_RXF2SIDH	0x08
#define MCP2515_RXF2SIDL	0x09
#define MCP2515_RXF2EID8	0x0A
#define MCP2515_RXF2EID0	0x0B
#define MCP2515_CANSTAT		0x0E
#define MCP2515_CANCTRL		0x0F
#define MCP2515_RXF3SIDH	0x10
#define MCP2515_RXF3SIDL	0x11
#define MCP2515_RXF3EID8	0x12
#define MCP2515_RXF3EID0	0x13
#define MCP2515_RXF4SIDH	0x14
#define MCP2515_RXF4SIDL	0x15
#define MCP2515_RXF4EID8	0x16
#define MCP2515_RXF4EID0	0x17
#define MCP2515_RXF5SIDH	0x18
#define MCP2515_RXF5SIDL	0x19
#define MCP2515_RXF5EID8	0x1A
#define MCP2515_RXF5EID0	0x1B
#define MCP2515_TEC		    0x1C
#define MCP2515_REC		    0x1D
#define MCP2515_RXM0SIDH	0x20
#define MCP2515_RXM0SIDL	0x21
#define MCP2515_RXM0EID8	0x22
#define MCP2515_RXM0EID0	0x23
#define MCP2515_RXM1SIDH	0x24
#define MCP2515_RXM1SIDL	0x25
#define MCP2515_RXM1EID8	0x26
#define MCP2515_RXM1EID0	0x27
#define MCP2515_CNF3		0x28
#define MCP2515_CNF2		0x29
#define MCP2515_CNF1		0x2A
#define MCP2515_CANINTE		0x2B
#define MCP2515_CANINTF		0x2C
#define MCP2515_EFLG		0x2D
#define MCP2515_TXB0CTRL	0x30
#define MCP2515_TXB1CTRL	0x40
#define MCP2515_TXB2CTRL	0x50
#define MCP2515_RXB0CTRL	0x60
#define MCP2515_RXB0SIDH	0x61
#define MCP2515_RXB1CTRL	0x70
#define MCP2515_RXB1SIDH	0x71

/* rx status instruction received message*/

#define MSG_IN_RXB0             0x01
#define MSG_IN_RXB1             0x02
#define MSG_IN_BOTH_BUFFERS     0x03

typedef union {
	
	struct {
		unsigned RX0IF :1;
		unsigned RX1IF :1;
		unsigned TXBOREQ :1;
		unsigned TX0IF :1;
		unsigned TXB1REQ :1;
		unsigned TX1IF :1;
		unsigned TXB2REQ :1;
		unsigned TX2IF :1;
		
	}; control_status;
} control_status_t;

typedef union {
	
	struct {
		unsigned message_type :2;
		unsigned filter_match :3;
		unsigned not_used_bit :1;
		unsigned rx_buffer : 2;
		
	}; control_rx_status;
} control_rx_status_t; 




