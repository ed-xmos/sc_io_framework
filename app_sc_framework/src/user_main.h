#ifndef _USER_MAIN_H_
#define _USER_MAIN_H_

#ifdef __XC__

#include "i2c.h"
#include <xs1.h>
#include <platform.h>

extern unsafe client interface i2c_master_if i_i2c_client;
extern void interface_saver(client interface i2c_master_if i);
extern void board_setup();

#endif
#endif