/*
 * mcp25625.h
 *
 *  Created on: Jan 10, 2026
 *      Author: marwankamal
 */

#ifndef INC_MCP25625_H_
#define INC_MCP25625_H_
#pragma once
#include "stm32g0xx_hal.h"
#include <stdint.h>

// MCP25625 SPI commands
#define MCP_CMD_RESET      0xC0
#define MCP_CMD_READ       0x03
#define MCP_CMD_WRITE      0x02
#define MCP_CMD_BIT_MODIFY 0x05

// Some useful MCP register addresses
#define MCP_CANSTAT        0x0E
#define MCP_CANCTRL        0x0F

void     MCP_Init(SPI_HandleTypeDef *hspi);
void     MCP_Reset(void);
uint8_t  MCP_ReadReg(uint8_t addr);
void     MCP_WriteReg(uint8_t addr, uint8_t value);
void     MCP_BitModify(uint8_t addr, uint8_t mask, uint8_t data);


#endif /* INC_MCP25625_H_ */
