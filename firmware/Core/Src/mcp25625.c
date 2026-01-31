#include "mcp25625.h"
#include "stm32g0xx_hal.h"
#include <stdbool.h>

static SPI_HandleTypeDef *mcp_hspi = NULL;
static GPIO_TypeDef *mcp_cs_port= NULL;
static uint16_t mcp_cs_pin =0;





static inline bool MCP_confirm_attach(void) {
	return (mcp_hspi !=NULL && mcp_cs_port !=NULL);
}


static inline void CS_low(void) {
	HAL_GPIO_WritePin(mcp_cs_port, mcp_cs_pin, GPIO_PIN_RESET);
}




static inline void CS_high(void) {
	HAL_GPIO_WritePin(mcp_cs_port, mcp_cs_pin, GPIO_PIN_SET);
}

void MCP_attach(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin) {
	
	mcp_hspi = hspi;
	mcp_cs_port = cs_port;
	mcp_cs_pin = cs_pin;
	CS_high();
}



void MCP_reset(void)
{
    uint8_t cmd = MCP_CMD_RESET;

    CS_low();
    (void)HAL_SPI_Transmit(mcp_hspi, &cmd, 1, HAL_MAX_DELAY);
    CS_high();

    HAL_Delay(2); 
}

uint8_t MCP_read_byte(uint8_t address)
{
	
	if (!MCP_confirm_attach()) {
		return 0xFF;
	}

    uint8_t tx[3] = { MCP_CMD_READ, address, 0x00};
    uint8_t rx[3] = { 0 };


    CS_low();
    (void)HAL_SPI_TransmitReceive(mcp_hspi, tx, rx, 3, HAL_MAX_DELAY);
    CS_high();


    return rx[2];
}
bool MCP_read_bytes(uint8_t address, uint8_t *output, uint8_t length) {
	uint8_t total = 2+length;

	uint8_t tx[2+length];
	uint8_t rx[2+length];

	if (length > 5) {
		return false;
	}
	tx[0] = MCP_CMD_READ;
	tx[1] = address;

	for ( int i =0; i < length; i++) {
		tx[2+i] = 0x00;
	}

	CS_low();
	HAL_SPI_TransmitReceive(mcp_hspi, tx, rx, total , HAL_MAX_DELAY);
	CS_high();

	for ( int i=0; i< length;i++) {
		out[i] = rx[2+i];
	}


	return true;
}

void MCP_write_byte (uint8_t address, uint8_t value)
{
	if (!MCP_confirm_attach()) {
			return 0xFF;
		}
	
    uint8_t tx[3] = { MCP_CMD_WRITE, address, value };

    CS_low();
    (void)HAL_SPI_Transmit(mcp_hspi, tx, 3, HAL_MAX_DELAY);
    CS_high();
}

void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
	
	if (!MCP_confirm_attach()) {
			return 0xFF;
		}
    uint8_t tx[4] = { MCP_CMD_BIT_MODIFY, address, mask, data };

    CS_low();
    (void)HAL_SPI_Transmit(mcp_hspi, tx, 4, HAL_MAX_DELAY);
    CS_high();
}

static bool MCP_confirm_mode(uint8_t mode ) {
	uint8_t canstat = MCP_read_byte(MCP_CANSTAT);
	uint8_t canctrl = MCP_read_btye(MCP_CANCTRL);
	
	return (((canstat >>5) & 0x07) == (mode & 0x07));
	return (((canctrl >>5) & 0x07) == (mode & 0x07));
	
}

bool MCP_init(void) {
	
	if (!MCP_confirm_attach()) {
		return false;
	}
	
	
	MCP_reset();
	
	if (!MCP_confirm_mode(0b100)) {
		return false;
	}
	
	MCP_write_byte(MCP_CNF1, 0xC0);
	MCP_write_byte(MCP_CNF2, 0x9E);
	MCP_write_byte(MCP_CNF3, 0x03);
	
	MCP_bit_modify(MCP_CANCTRL, 0xE0, 0x00);
	
	HAL_Delay(2);
	
	if (!MCP_confirm_mode(0b000)) {
		return false;
	}
	
	return true;
	
	
}
// values used in these next values are the values of each mode found on p.53 of the mcp25625 datasheet
bool MCP_set_config_mode(void) {
	MCP_bit_modify(MCP_CANCTRL,MCP_CANCTRL_REQOP_Msk ,0x80);

	for ( int i =0; i < 1000; i++) {
		uint8_t canstat = MCP_read_byte(MCP_CANSTAT);
		if ( (canstat & MCP_CANCTRL_REQOP_Msk) == 0x80) {
			return true;
		}

	}
	return false;
}
bool MCP_set_normal_mode(void) {
	MCP_bit_modify(MCP_CANCTRL,MCP_CANCTRL_REQOP_Msk ,0x00);
	for ( int i =0; i < 1000; i++) {
			uint8_t canstat = MCP_read_byte(MCP_CANSTAT);
			if ( (canstat & MCP_CANCTRL_REQOP_Msk) == 0x00) {
				return true;
			}

		}
		return false;


}
bool MCP_set_sleep_mode(void) {

	MCP_bit_modify(MCP_CANCTRL,MCP_CANCTRL_REQOP_Msk ,0x20);
		for ( int i =0; i < 1000; i++) {
				uint8_t canstat = MCP_read_byte(MCP_CANSTAT);
				if ( (canstat & MCP_CANCTRL_REQOP_Msk) == 0x20) {
					return true;
				}

			}
			return false;
}
bool MCP_set_loopback_mode(void) {
	MCP_bit_modify(MCP_CANCTRL,MCP_CANCTRL_REQOP_Msk ,0x40);
			for ( int i =0; i < 1000; i++) {
					uint8_t canstat = MCP_read_byte(MCP_CANSTAT);
					if ( (canstat & MCP_CANCTRL_REQOP_Msk) == 0x40) {
						return true;
					}

				}
				return false;

}
bool MCP_set_listen_mode(void) {
	MCP_bit_modify(MCP_CANCTRL,MCP_CANCTRL_REQOP_Msk ,0x60);
			for ( int i =0; i < 1000; i++) {
					uint8_t canstat = MCP_read_byte(MCP_CANSTAT);
					if ( (canstat & MCP_CANCTRL_REQOP_Msk) == 0x60) {
						return true;
					}

				}
				return false;
}
bool MCP_message_available(void) {
	uint8_t canintf = MCP_read_byte(MCP_CANINTF);
    return (canintf & (MCP_CANINTF_RX0IF | MCP_CANINTF_RX1IF )) !=0;
}

bool MCP_receive_raw(uint8_t * rx_buffer_0, uint8_t * rx_buffer_1) {



		uint8_t canintf = MCP_read_byte(MCP_CANINTF);
	 if ( canintf & MCP_CANINTF_RX0IF) {
		 MCP_read_bytes(MCP_RXB0SIDH, rx_buffer_0, 13);
		 MCP_bit_modify(MCP_CANINTF, MCP_CANINTF_RX0IF, 0x00);

	     return true;
	     }
	 if ( canintf & MCP_CANINTF_RX1IF) {
	 		 MCP_read_bytes(MCP_RXB1SIDH, rx_buffer_1, 13);
	 		 MCP_bit_modify(MCP_CANINTF, MCP_CANINTF_RX1IF, 0x00);

	 	     return true;
	 	     }

	 return false;
}
