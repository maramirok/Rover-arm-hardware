/*
 * mcp25625.h
 *
 *  Created on: Jan 10, 2026
 *      Author: marwankamal
 */

#ifndef INC_MCP25625_H_
#define INC_MCP25625_H_
#include "stm32g0xx_hal.h"
#include <stdint.h>

// MCP25625 SPI commands
#define MCP_CMD_RESET       0xC0
#define MCP_CMD_READ        0x03
#define MCP_CMD_WRITE       0x02
#define MCP_CMD_BIT_MODIFY  0x05
#define MCP_CMD_RX_STATUS   0xB0
#define MCP_CMD_READ_STATUS 0xA0
#define MCP_CMD_READ_RX_BUF_RXB0SIDH 0x90
#define MCP_CMD_READ_RX_BUF_RXB0D0 0x92
#define MCP_CMD_READ_RX_BUF_RXB1SIDH 0x94
#define MCP_CMD_READ_RX_BUF_RXB1D0 0x96

#define MCP_RXF0SIDH	0x00
#define MCP_RXF0SIDL	0x01
#define MCP_RXF0EID8	0x02
#define MCP_RXF0EID0	0x03
#define MCP_RXF1SIDH	0x04
#define MCP_RXF1SIDL	0x05
#define MCP_RXF1EID8	0x06
#define MCP_RXF1EID0	0x07
#define MCP_RXF2SIDH	0x08
#define MCP_RXF2SIDL	0x09
#define MCP_RXF2EID8	0x0A
#define MCP_RXF2EID0	0x0B
#define MCP_CANSTAT		0x0E
#define MCP_CANCTRL		0x0F

#define MCP_RXF3SIDH	0x10
#define MCP_RXF3SIDL	0x11
#define MCP_RXF3EID8	0x12
#define MCP_RXF3EID0	0x13
#define MCP_RXF4SIDH	0x14
#define MCP_RXF4SIDL	0x15
#define MCP_RXF4EID8	0x16
#define MCP_RXF4EID0	0x17
#define MCP_RXF5SIDH	0x18
#define MCP_RXF5SIDL	0x19
#define MCP_RXF5EID8	0x1A
#define MCP_RXF5EID0	0x1B
#define MCP_TEC		0x1C
#define MCP_REC		0x1D

#define MCP_RXM0SIDH	0x20
#define MCP_RXM0SIDL	0x21
#define MCP_RXM0EID8	0x22
#define MCP_RXM0EID0	0x23
#define MCP_RXM1SIDH	0x24
#define MCP_RXM1SIDL	0x25
#define MCP_RXM1EID8	0x26
#define MCP_RXM1EID0	0x27
#define MCP_CNF3		0x28
#define MCP_CNF2		0x29
#define MCP_CNF1		0x2A
#define MCP_CANINTE		0x2B
#define MCP_CANINTF		0x2C
#define MCP_EFLG		0x2D

#define MCP_TXB0CTRL	0x30
#define MCP_TXB1CTRL	0x40
#define MCP_TXB2CTRL	0x50
#define MCP_RXB0CTRL	0x60
#define MCP_RXB0SIDH	0x61
#define MCP_RXB1CTRL	0x70
#define MCP_RXB1SIDH	0x71


 // bit definitions //
#define MCP_CANSTAT_OPMOD_Pos 5
#define MCP_CANSTAT_OPMOD_Msk (0x7U << MCP_CANSTAT_OPMOD_Pos)

#define MCP_CANCTRL_REQOP_Pos 5
#define MCP_CANCTRL_REQOP_Msk (0x7U << MCP_CANCTRL_REQOP_Pos) 
#define MCP_CANCTRL_ABAT (1U << 4)
#define MCP_CANCTRL_OSM (1U << 3)
#define MCP_CANCTRL_CLKEN (1U << 2)
#define MCP_CANCTRL_CLKPR1 (1U << 1)
#define MCP_CANCTRL_CLKPRE0 (1U << 0)

#define MCP_CNF1_BRP_Pos 0
#define MCP_CNF1_BRP_Msk (0x3FU << MCP_CNF1_BRP_Pos)
#define MCP_CNF1_SJW_Pos 6
#define MCP_CNF1_SJW_Msk (0x3U  << MCP_CNF1_SJW_Pos)

#define MCP_CNF2_PRSEG_Pos 0
#define MCP_CNF2_PRSEG_Msk (0x7U << MCP_CNF2_PRSEG_Pos)
#define MCP_CNF2_PHSEG1_Pos 3
#define MCP_CNF2_PHSEG1_Msk (0x7U << MCP_CNF2_PHSEG1_Pos)

#define MCP_CNF3_PHSEG2_Pos 0
#define MCP_CNF3_PHSEG2_Msk (0x7U << MCP_CNF3_PHSEG2_Pos)


#define MCP_CANINTE_MERRE (1U << 7)
#define MCP_CANINTE_WAKIE (1U << 6)
#define MCP_CANINTE_ERRIE (1U << 5)
#define MCP_CANINTE_TX2IE (1U << 4)
#define MCP_CANINTE_TX1IE (1U << 3)
#define MCP_CANINTE_TX0IE (1U << 2)
#define MCP_CANINTE_RX1IE (1U << 1)
#define MCP_CANINTE_RX0IE (1U << 0) 

#define MCP_CANINTF_MERREF (1U << 7)
#define MCP_CANINTF_WAKIEF (1U << 6)
#define MCP_CANINTF_ERRIF (1U << 5)
#define MCP_CANINTF_TX2IF (1U << 4)
#define MCP_CANINTF_TX1IF (1U << 3)
#define MCP_CANINTF_TX0IF (1U << 2)
#define MCP_CANINTF_RX1IF (1U << 1)
#define MCP_CANINTF_RX0IF (1U << 0) 

#define MCP_EFLG_RX1OVR (1U << 7)
#define MCP_EFLG_RX0OVR (1U << 6)
#define MCP_EFLG_TXBO (1U << 5)
#define MCP_EFLG_TXEP (1U << 4)
#define MCP_EFLG_RXEP (1U << 3)
#define MCP_EFLG_TXWAR (1U << 2)
#define MCP_EFLG_RXWAR (1U << 1)
#define MCP_EFLG_EWARN (1U << 0)

#define MCP_TXB0CTRL_ABTF (1U<< 6)
#define MCP_TXB0CTRL_MLOA (1U<< 5)
#define MCP_TXB0CTRL_TXERR (1U<< 4)
#define MCP_TXB0CTRL_TXREQ (1U<< 3)
#define MCP_TXB0CTRL_TXP1 (1U<< 1)
#define MCP_TXB0CTRL_TXP0 (1U<< 0)

#define MCP_TXB1CTRL_ABTF (1U<< 6)
#define MCP_TXB1CTRL_MLOA (1U<< 5)
#define MCP_TXB1CTRL_TXERR (1U<< 4)
#define MCP_TXB1CTRL_TXREQ (1U<< 3)
#define MCP_TXB1CTRL_TXP1 (1U<< 1)
#define MCP_TXB1CTRL_TXP0 (1U<< 0)

#define MCP_TXB2CTRL_ABTF (1U<< 6)
#define MCP_TXB2CTRL_MLOA (1U<< 5)
#define MCP_TXB2CTRL_TXERR (1U<< 4)
#define MCP_TXB2CTRL_TXREQ (1U<< 3)
#define MCP_TXB2CTRL_TXP1 (1U<< 1)
#define MCP_TXB2CTRL_TXP0 (1U<< 0)

#define MCP_RXB0CTRL_RXM1 (1U<< 6)
#define MCP_RXB0CTRL_RXM0 (1U<< 5)
#define MCP_RXB0CTRL_RXRTR (1U<< 3)
#define MCP_RXB0CTRL_BUKT (1U<< 2)
#define MCP_RXB0CTRL_BUKT1 (1U<< 1)
#define MCP_RXB0CTRL_FILHIT0 (1U<< 0)

#define MCP_RXB1CTRL_RXM1 (1U<< 6)
#define MCP_RXB1CTRL_RXM0 (1U<< 5)
#define MCP_RXB1CTRL_RXRTR (1U<< 3)
#define MCP_RXB1CTRL_FILHIT2 (1U<< 2)
#define MCP_RXB1CTRL_FILHIT1 (1U<< 1)
#define MCP_RXB1CTRL_FILHIT0 (1U<< 0)




void MCP_set_config_mode(void);
void MCP_set_normal_mode(void);
void MCP_set_sleep_mode(void);
void MCP_set_loopback_mode(void);
void MCP_set_listen_mode(void);
void MCP_reset(void);
uint8_t MCP_read_byte(uint8_t address);
void MCP_write_byte(uint8_t address, uint8_t value);
void MCP_read_RXB0(uint8_t buff[13]);
void MCP_read_RXB1(uint8_t buff[13]);
void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void MCP_attach(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, unint16_t cs_pin);
static inline bool MCP_confirm_attach(void);
static bool MCP_confirm_mode(uint8_t mode);
bool MCP_init(void);


#define MCP_NORMAL_MODE 0x0
#define MCP_SLEEP_MODE 0x1
#define MCP_LOOPBACK_MODE 0x2
#define MCP_LISTEN_MODE 0x3
#define MCP_CONFIG_MODE 0x4



 
#endif /* INC_MCP25625_H_ */
