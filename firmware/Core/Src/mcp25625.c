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


	if (!MCP_confirm_attach()) return false;
	    if (length == 0) return true;
	    if (length > 13) return false;

	    uint8_t total = 2 + length;
	    uint8_t tx[15] = {0};
	    uint8_t rx[15] = {0};

	    tx[0] = MCP_CMD_READ;
	    tx[1] = address;


	    CS_low();
	    (void)HAL_SPI_TransmitReceive(mcp_hspi, tx, rx, total, HAL_MAX_DELAY);
	    CS_high();

	    for (uint8_t i = 0; i < length; i++) {
	        output[i] = rx[2 + i];
	    }
	    return true;
}




void MCP_write_byte (uint8_t address, uint8_t value)
{
	if (!MCP_confirm_attach()) {
			return;
		}
	
    uint8_t tx[3] = { MCP_CMD_WRITE, address, value };

    CS_low();
    (void)HAL_SPI_Transmit(mcp_hspi, tx, 3, HAL_MAX_DELAY);
    CS_high();
}






bool MCP_write_bytes(uint8_t address, uint8_t const *value, uint8_t length) {
	if (!MCP_confirm_attach()) return false;
		    if (length == 0) return true;
		    if (length > 13) return false;

		  uint8_t total = 2+length;
		  uint8_t tx[15] = {0};

		  tx[0]= MCP_CMD_WRITE;
		  tx[1] = address;

		  for ( int i =0; i <length; i++) {
			  tx[2+i] = value[i];
		  }


		  CS_low();
		  bool ok = (HAL_SPI_Transmit(mcp_hspi, tx, total, HAL_MAX_DELAY) == HAL_OK);
		  CS_high();

		  return ok;


}






void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
	
	if (!MCP_confirm_attach()) {
			return;
		}
    uint8_t tx[4] = { MCP_CMD_BIT_MODIFY, address, mask, data };

    CS_low();
    (void)HAL_SPI_Transmit(mcp_hspi, tx, 4, HAL_MAX_DELAY);
    CS_high();
}



static bool MCP_confirm_mode(uint8_t mode ) {
	uint8_t canstat = MCP_read_byte(MCP_CANSTAT);
	
	return (((canstat >>5) & 0x07) == (mode & 0x07));
	
}


void MCP_RTS( uint8_t rts_cmd) {


	CS_low();
	(void)HAL_SPI_Transmit(mcp_hspi, &rts_cmd, 1, HAL_MAX_DELAY);
	CS_high();
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
		if( !MCP_read_bytes(MCP_RXB0SIDH, rx_buffer_0, 13)) {
			return false;
		}
		 MCP_bit_modify(MCP_CANINTF, MCP_CANINTF_RX0IF, 0x00);

	     return true;
	     }
	 if ( canintf & MCP_CANINTF_RX1IF) {
	 	if (!MCP_read_bytes(MCP_RXB1SIDH, rx_buffer_1, 13)) {
	 			 return false;
	 		 }
	 		 MCP_bit_modify(MCP_CANINTF, MCP_CANINTF_RX1IF, 0x00);

	 	     return true;
	 	     }

	 return false;
}


bool MCP_sending_raw(const uint8_t * tx_buffer)  {

	uint8_t txb0ctrl = MCP_read_byte(MCP_TXB0CTRL);

	if ( (txb0ctrl & MCP_TXB0CTRL_TXREQ) ==0) {
		if(!MCP_write_bytes(MCP_TXB0SIDH, tx_buffer,13)) {
			return false;
		}
		MCP_RTS(MCP_CMD_RTS_TXB0);
		return true;
	}

	uint8_t txb1ctrl = MCP_read_byte(MCP_TXB1CTRL);

	if ( (txb1ctrl & MCP_TXB1CTRL_TXREQ) ==0) {
			if(!MCP_write_bytes(MCP_TXB1SIDH, tx_buffer,13)) {
				return false;
			}
			MCP_RTS(MCP_CMD_RTS_TXB1);
			return true;
		}

	uint8_t txb2ctrl = MCP_read_byte(MCP_TXB2CTRL);

	if ( (txb2ctrl & MCP_TXB2CTRL_TXREQ) ==0) {
			if(!MCP_write_bytes(MCP_TXB2SIDH, tx_buffer,13)) {
				return false;
			}

			MCP_RTS(MCP_CMD_RTS_TXB2);
			return true;
		}


	return false;
}


uint8_t MCP_read_eflg(void) {
	return MCP_read_byte(MCP_EFLG);
}




uint8_t MCP_read_tec(void)   {
	return MCP_read_byte(MCP_TEC);
}



uint8_t MCP_read_rec(void) {
	return MCP_read_byte(MCP_REC);
}



void    MCP_clear_rx_overflow(void) {

	MCP_bit_modify(MCP_EFLG,MCP_EFLG_RX1OVR, 0x00);

	MCP_bit_modify(MCP_EFLG,MCP_EFLG_RX0OVR, 0x00);
}








uint32_t MCP_unpack_ext_id(uint8_t sidh, uint8_t sidl, uint8_t ex_id_8, uint8_t ex_id_0) {
	uint32_t ext_full_id = 0;

	id |= ((uint32_t)sidh << 21 );


}

static void mcp_pack_ext_id_29(uint32_t id, uint8_t raw[13])
{
    id &= 0x1FFFFFFF;

    raw[0] = (uint8_t)(id >> 21);

    raw[1] = (uint8_t)(
          (((id >> 18) & 0x07) << 5)
        | (1U << 3)                 // EXIDE = 1
        | ((id >> 16) & 0x03)
    );

    raw[2] = (uint8_t)(id >> 8);
    raw[3] = (uint8_t)(id >> 0);
}

static uint32_t mcp_unpack_ext_id_29(const uint8_t raw[13])
{
    uint32_t id = 0;
    id |= ((uint32_t)raw[0]) << 21;
    id |= ((uint32_t)((raw[1] >> 5) & 0x07)) << 18;
    id |= ((uint32_t)(raw[1] & 0x03)) << 16;
    id |= ((uint32_t)raw[2]) << 8;
    id |= ((uint32_t)raw[3]) << 0;
    return id & 0x1FFFFFFF;
}

static void mcp_pack_frame_to_raw(const CanFrame *f, uint8_t raw[13])
{
    mcp_pack_ext_id_29(f->id, raw);

    uint8_t dlc = f->dlc & 0x0F;
    if (dlc > 8) dlc = 8;
    raw[4] = dlc;

    for (uint8_t i = 0; i < 8; i++) {
        raw[5 + i] = f->data[i];
    }
}

static void mcp_unpack_raw_to_frame(const uint8_t raw[13], CanFrame *out)
{
    out->id = mcp_unpack_ext_id_29(raw);

    uint8_t dlc = raw[4] & 0x0F;
    if (dlc > 8) dlc = 8;
    out->dlc = dlc;

    for (uint8_t i = 0; i < 8; i++) {
        out->data[i] = raw[5 + i];
    }
}

// ---- PUBLIC (these ARE in your header) ----

bool MCP_send_frame(const CanFrame *frame)
{
    if (frame == NULL) return false;

    uint8_t raw[13] = {0};
    mcp_pack_frame_to_raw(frame, raw);

    return MCP_sending_raw(raw);
}

bool MCP_receive_frame(CanFrame *frame)
{
    if (frame == NULL) return false;

    uint8_t raw0[13] = {0};
    uint8_t raw1[13] = {0};

    if (!MCP_receive_raw(raw0, raw1)) {
        return false;
    }

    // Your MCP_receive_raw reads either RXB0 or RXB1 and clears the flag.
    // Detect which buffer got filled:
    bool raw0_used = (raw0[0] != 0) || (raw0[1] != 0) || (raw0[4] != 0);
    bool raw1_used = (raw1[0] != 0) || (raw1[1] != 0) || (raw1[4] != 0);

    if (raw0_used && !raw1_used) {
        mcp_unpack_raw_to_frame(raw0, frame);
        return true;
    }
    if (raw1_used && !raw0_used) {
        mcp_unpack_raw_to_frame(raw1, frame);
        return true;
    }

    // fallback: prefer raw0 if it looks non-empty
    if (raw0_used) {
        mcp_unpack_raw_to_frame(raw0, frame);
        return true;
    }
    if (raw1_used) {
        mcp_unpack_raw_to_frame(raw1, frame);
        return true;
    }

    return false;
}




