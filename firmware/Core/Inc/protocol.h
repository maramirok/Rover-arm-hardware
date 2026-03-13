#ifndef INC_PROTOCOL_H_
#define INC_PROTOCOL_H_

#include <stdint.h>

// base id for compat board

#define board_base_id 0b00111000010000000000000001100


// value for each instruction type


#define MEI_id_type 0b00
#define AEI_id_type 0b01
#define status_id_type 0b10
#define ctrl_id_type 0b11

// id for each instruction

#define force_stop_id_bit 0b01
#define resume_id_bit 0b10
#define error_id_bit 0b11
#define motor_1_id_bit 0b1001
#define motor_2_id_bit 0b1010
#define motor_3_id_bit 0b1011
#define motor_4_id_bit 0b1100
#define motor_5_id_bit 0b1101

// temporary max speed value

#define MAX_RADS 100.0f

// making each id

#define MAKE_ID(type, id)  (board_base_id | ((type)<<11) | ((id)<<6))




// final id for each instruction

#define FORCE_STOP_ID MAKE_ID(ctrl_id_type, force_stop_id_bit)
#define RESUME_ID MAKE_ID(ctrl_id_type, resume_id_bit)
#define ERROR_ID MAKE_ID(status_id_type, error_id_bit)
#define MOTOR_1_ID MAKE_ID(ctrl_id_type, motor_1_id_bit)
#define MOTOR_2_ID MAKE_ID(ctrl_id_type, motor_2_id_bit)
#define MOTOR_3_ID MAKE_ID(ctrl_id_type, motor_3_id_bit)
#define MOTOR_4_ID MAKE_ID(ctrl_id_type, motor_4_id_bit)
#define MOTOR_5_ID MAKE_ID(ctrl_id_type, motor_5_id_bit)



void stop_motors(void);

#endif




