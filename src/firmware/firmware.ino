#include "pindefs.h"
#include <SoftwareSerial.h>
SoftwareSerial gsm_serial(gsm_RX_L, gsm_TX_L);

void setup()
{
	Serial.begin(9600);
	gsm_serial.begin(9600);

	OUT(rly_OFF);
	OUT(rly_ON);

	IN_PU(u_in);
	IN_PU(v_in);
	IN_PU(w_in);
	IN_PU(USR_SW1);
	IN_PU(USR_SW2);
}