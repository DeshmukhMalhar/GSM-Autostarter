#define pindefs_h
#define SWREVISION "0.0.1"
#define dbg_rx 0
#define dbg_tx 1
#define u_in 2
#define v_in 3
#define w_in 4
#define rly_ON 5
#define rly_OFF 6
#define BUZZ 7
#define gsm_RX_L 8
#define gsm_TX_L 9
#define LED_GREEN 10
#define LED_RED 11
#define LED_GREEN2 12
#define LED 13
#define IN_POT A5
#define USR_SW1 20
#define USR_SW2 21

#define CONFIG (PINC & 0x0f) //reads config DIPsw and masks it

#define OUT(X) pinMode(X, OUTPUT)
#define IN(X) pinMode(X, INPUT)
#define IN_PU(X) pinMode(X, INPUT_PULLUP)

#define read(X) digitalRead(X)

#define ON(X) digitalWrite(X, 1)
#define OFF(X) digitalWrite(X, 0)

#define DEL_TIME_STD 6600
#define RELAY_HOLD 500
#define ERROR -1
#define GOOD 2

#define ERROR_FLASH_DLY 50
#define GOOD_FLASH_DLY 100
enum config_states
{
	NO_GSM_actualDelay = 0,
	NO_GSM_twiceDelay,
	GSM_actualDelay,
	GSM_twiceDelay,

	SETUP = 0x0a,

};

const String about = String("GSM-Autostarter \n") + String("Firmware Version: ") + String(SWREVISION) + String("\n") + String("BUILT on\n") + String(__DATE__) + String("\n") + String(__TIME__);