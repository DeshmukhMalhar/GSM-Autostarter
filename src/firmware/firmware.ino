#include "pindefs.h"
#include <SoftwareSerial.h>
SoftwareSerial gsm_serial(gsm_RX_L, gsm_TX_L);

void setup()
{
	Serial.begin(9600);
	gsm_serial.begin(9600);

	OUT(rly_OFF);
	OUT(rly_ON);

	IN(u_in);
	IN(v_in);
	IN(w_in);
}