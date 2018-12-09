#define pindefs_h

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

#define USR_SW1 20
#define USR_SW2 21

#define CONFIG (PINC & 0x0f) //reads config DIPsw and masks it

#define OUT(X) pinMode(X, OUTPUT)
#define IN(X) pinMode(X, INPUT)
#define IN_PU(X) pinMode(X, INPUT_PULLUP)

#define ON(X) digitalWrite(X, 1)
#define OFF(X) digitalWrite(X, 0)

enum config_states
{
	normal,

};