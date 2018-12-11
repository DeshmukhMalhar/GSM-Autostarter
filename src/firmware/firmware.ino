#include "pindefs.h"
#include <stdint.h>

//#include <SoftwareSerial.h>
//SoftwareSerial gsm_serial(gsm_RX_L, gsm_TX_L);

#define DEBUG_L1

volatile uint8_t flag_u, flag_v, flag_w;
volatile int32_t delVU, delWV, delUW, time_preU, time_preV, time_preW, delU, delV, delW;

int32_t delUW_shadow, delVU_shadow, delWV_shadow, last_phaseOK_stamp, TurnOnWait;

const int32_t timelimitHigh = DEL_TIME_STD + 2000, timelimitLow = DEL_TIME_STD - 2000;
bool d1Status, d2Status, d3Status, isitfinetoturnon = false;
int8_t count_sw2 = 0;
//remember that there are 120 degree phase shifts, thus when normalized to range of 0-90deg, we get order as YELLOW->RED->BLUE
//thus connection pattern is modified as follows
//u->YELLOW
//v->RED
//w->BLUE

ISR(INT0_vect)
{
	// if (!flag_u)
	// {
	time_preU = micros();
	flag_u = true;
	// if (flag_w)
	// {
	delUW = time_preU - time_preW;
	flag_w = false;
	//}
	// }
}
ISR(INT1_vect)
{
	// if (!flag_v)
	// {
	time_preV = micros();
	flag_v = true;
	// if (flag_u)
	// {
	delVU = time_preV - time_preU;
	flag_u = false;
	//}
	// }
}

ISR(PCINT2_vect)
{
	if ((PIND & PD4) == 0)
	{
		// if (!flag_w)
		// {
		time_preW = micros();
		flag_w = true;
		// if (flag_v)
		// {
		delWV = time_preW - time_preV;
		flag_v = false;
		//}
		//}
	}
}

bool checkPhases()
{
	d1Status = ((delUW_shadow <= timelimitHigh) && (delUW_shadow >= timelimitLow));
	d2Status = ((delVU_shadow <= timelimitHigh) && (delVU_shadow >= timelimitLow));
	d3Status = ((delWV_shadow <= timelimitHigh) && (delWV_shadow >= timelimitLow));

	return (d1Status && d2Status && d3Status);
}

void blink(uint8_t _io, int8_t code)
{
	switch (code)
	{
	case ERROR:
		flashError(_io);
		break;
	case GOOD:
		flashGood(_io);
		break;
	default:
		break;
	}
}
void flashError(uint8_t _io)
{
	ON(_io);
	delay(ERROR_FLASH_DLY);
	OFF(_io);
	delay(ERROR_FLASH_DLY);
	ON(_io);
	delay(ERROR_FLASH_DLY);
	OFF(_io);
	delay(ERROR_FLASH_DLY);
	ON(_io);
	delay(ERROR_FLASH_DLY);
	OFF(_io);
	delay(ERROR_FLASH_DLY);
}
void flashGood(uint8_t _io)
{
	ON(_io);
	delay(GOOD_FLASH_DLY);
	OFF(_io);
	delay(GOOD_FLASH_DLY);
	ON(_io);
	delay(GOOD_FLASH_DLY);
	OFF(_io);
	delay(GOOD_FLASH_DLY);
	ON(_io);
	delay(GOOD_FLASH_DLY);
	OFF(_io);
	delay(GOOD_FLASH_DLY);
}
void onSequence()
{
	if (checkPhases())
	{
		ON(rly_ON);
		delay(RELAY_HOLD);
		OFF(rly_ON);
		blink(LED_GREEN, GOOD);
	}
	else
	{
		blink(LED_RED, ERROR);
	}
}
void offSequence()
{
	ON(rly_OFF);
	delay(1000);
	OFF(rly_OFF);
	blink(LED_GREEN2, GOOD);
}

void setup()
{

	//external  interrupt setup
	cli();
	EICRA |= (1 << ISC11) | (1 << ISC01); //setting int0 and int1 for falling edge,
										  //falling edge gets us the positive peak of waveform
	EIMSK |= (1 << INT1) | (1 << INT0);   //enable into and int1
	PCICR |= (1 << PCIE2);				  //enable PCINT2 controlling PCINT23:16, masking others than PCINT20
	PCMSK2 |= (1 << PCINT20);
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
	Serial.println(about);
	delay(500);
	//gsm_serial.begin(9600);

	TurnOnWait = (((CONFIG == NO_GSM_actualDelay) || (CONFIG == GSM_actualDelay)) ? (analogRead(IN_POT) / 2) : (analogRead(IN_POT))) * 1000; //read the delay value from configuration pot,converting to milliseconds

#ifdef DEBUG_L1
	Serial.print("TurnOnWait:");
	Serial.println(TurnOnWait);
#endif
	isitfinetoturnon = true;
}
void loop()
{
	delUW_shadow = delUW;
	delVU_shadow = delVU;
	delWV_shadow = delWV;
	if (checkPhases())
	{
		last_phaseOK_stamp = millis();
	}

	if (((last_phaseOK_stamp <= (millis() - TurnOnWait)) && isitfinetoturnon) || read(USR_SW1))
	{
		onSequence();
	}

	if (read(USR_SW2))
	{
		count_sw2++;
		if (count_sw2 >= 2)
		{
			count_sw2 = 0;
			offSequence();
		}
	}

	//if (flag_u && flag_v && flag_w)
	// {
	// 	flag_u = false;
	// 	flag_v = false;
	// 	flag_w = false;

	// 	delUW = time_preW - time_preU;
	// 	delVU = time_preU - time_preV;
	// 	delWV = time_preV - time_preW;
	// }

#ifdef DEBUG_L1
	Serial.print("delUW: ");
	Serial.println(delUW);
	Serial.print("delVU: ");
	Serial.println(delVU);
	Serial.print("delWV: ");
	Serial.println(delWV);
	Serial.print("checkPhases()");
	Serial.println(checkPhases());

#ifdef DEBUG_L2
	Serial.print("time_preU:");
	Serial.println(time_preU);
	Serial.print("time_preV:");
	Serial.println(time_preV);
	Serial.print("time_preW:");
	Serial.println(time_preW);
#endif

#endif

	delay(200);
}
