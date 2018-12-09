#include "pindefs.h"
//#include <SoftwareSerial.h>

//SoftwareSerial gsm_serial(gsm_RX_L, gsm_TX_L);

volatile uint8_t flag_u, flag_v, flag_w;
volatile int32_t delVU, delWV, delUW, time_preU, time_preV, time_preW;

//remember that there are 120 degree phase shifts, thus when normalised to range of 0-90deg, we get order as YELLOW->RED->BLUE
//thus connection pattern is modified as follows
//u->YELLOW
//v->RED
//w->BLUE

ISR(INT0_vect)
{
	if (!flag_u)
	{
		time_preU = micros();
		flag_u = true;
		if (flag_w)
		{
			delUW = time_preU - time_preW;
			flag_w = false;
		}
	}
}
ISR(INT1_vect)
{
	if (!flag_v)
	{
		time_preV = micros();
		flag_v = true;
		if (flag_u)
		{
			delVU = time_preV - time_preU;
			flag_u = false;
		}
	}
}

ISR(PCINT2_vect)
{
	if ((PIND & (1 << PD2)) == 0)
	{
		if (!flag_w)
		{
			time_preW = micros();
			flag_w = true;
			if (flag_v)
			{
				delWV = time_preW - time_preV;
				flag_v = false;
			}
		}
	}
}
void setup()
{
	cli();
	//external  interrupt setup
	EICRA |= (1 << ISC11) | (1 << ISC01); //setting int0 and int1 for falling edge,
										  //falling edge gets us the positive peak of waveform
	EIMSK |= (1 << INT1) | (1 << INT0);   //enable into and int1
	PCICR |= (1 << PCIE2);				  //enable PCINT2 controlling PCINT23:16, masking others than PCINT18
	PCMSK2 |= (1 << PCINT18);
	//interrupt configuration complete

	//IO configuration
	OUT(rly_OFF);
	OUT(rly_ON);

	IN_PU(u_in);
	IN_PU(v_in);
	IN_PU(w_in);

	IN_PU(USR_SW1);
	IN_PU(USR_SW2);
	DDRC = 0;
	PORTC = 0xff; //4 bit DIPsw PORTC IN_PU

	//io config complete
	sei(); //turning on global interrupts

	Serial.begin(9600);
	//gsm_serial.begin(9600);
}
void loop()
{
	Serial.print("delUW: ");
	Serial.println(delUW);
	Serial.print("delVU: ");
	Serial.println(delVU);
	Serial.print("delWV: ");
	Serial.println(delWV);
	delay(200);
}