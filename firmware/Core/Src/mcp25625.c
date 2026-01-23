#include "mcp25625.h"
#include "main.h"   // for CS_GPIO_Port, CS_Pin

static SPI_HandleTypeDef *mcp_hspi = NULL;

static inline void CS_LOW(void)  { HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET); }
static inline void CS_HIGH(void) { HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);   }

void MCP_Init(SPI_HandleTypeDef *hspi)
{
    mcp_hspi = hspi;
    CS_HIGH(); // idle deselected
}

void MCP_Reset(void)
{
    uint8_t cmd = MCP_CMD_RESET;

    CS_LOW();
    (void)HAL_SPI_Transmit(mcp_hspi, &cmd, 1, HAL_MAX_DELAY);
    CS_HIGH();

    HAL_Delay(2); // datasheet: allow oscillator/startup time
}

uint8_t MCP_ReadReg(uint8_t addr)
{
    uint8_t tx[3] = { MCP_CMD_READ, addr, 0x00 };
    uint8_t rx[3] = { 0 };

    CS_LOW();
    (void)HAL_SPI_TransmitReceive(mcp_hspi, tx, rx, 3, HAL_MAX_DELAY);
    CS_HIGH();

    return rx[2];
}

void MCP_WriteReg(uint8_t addr, uint8_t value)
{
    uint8_t tx[3] = { MCP_CMD_WRITE, addr, value };

    CS_LOW();
    (void)HAL_SPI_Transmit(mcp_hspi, tx, 3, HAL_MAX_DELAY);
    CS_HIGH();
}

void MCP_BitModify(uint8_t addr, uint8_t mask, uint8_t data)
{
    uint8_t tx[4] = { MCP_CMD_BIT_MODIFY, addr, mask, data };

    CS_LOW();
    (void)HAL_SPI_Transmit(mcp_hspi, tx, 4, HAL_MAX_DELAY);
    CS_HIGH();
}
/*
 * mcp25625.c
 *
 *  Created on: Jan 10, 2026
 *      Author: marwankamal
 */


