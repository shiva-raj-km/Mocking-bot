
/*
* Team ID: eYRC#161
* Author List: Nidesh D Shetty, Shashank Jain T M, Shivaraj K M, Shrinidhi Adiga
* Filename: Bot 
* Theme: Mocking Bot
*/

#include "Bot.h"


void array_segmentation();
int control(char n, int curr_note);
void rotationf();
void rotationb();
void stop_udf(int delay_udf);
int calculation(int curr_note, int next_note);
void hitme();
void positive_dir();
void serial_communication();
void setting_pins();
void check_boot_press();
void boot_switch_pin_config();
void hit_trumpet(int l,int m,int r);
void trumpet_striking();
void onset_calculation_p(float onset_p);
void onset_calculation_t(float onset_t);

/**********************************
Function name	:	setup
Input   		:	None
Output      	:	None
Functionality	:	Declaration of the pins as input or output
Example Call	:	Called automatically
***********************************/

void setup() {
  DDRE  = DDRE & 0XEF;  //pe4
  DDRH = DDRE | 0x03;  // pin ph0 and ph1 motor   servo 4 ph3
  DDRA = 0xFF;
  PORTA = 0X00;
  delay(500);
  attachInterrupt(0,positive_dir,RISING);
  port_init();
  timer1_init();
  timer4_init();
  curr_note = 0;
  servo_1(s_pos);
  servo_2(tpos);
  servo_3(error2 - tpos);
  servo_4(tpos - error3);
  boot_switch_pin_config();
 PORTA = 0x02;
 while(flag == 0){
 serial_communication();
 }
array_segmentation();
}

/**********************************
Function name	:	loop
Input			:	None
Output			:	None
Functionality	:	Striking of the piano key
Example Call	:	Called automatically
***********************************/

void loop(){
	int i=0,j=0,k=0;
	
	check_boot_press();
	
	if(switch_flag == 1)
	{
		prev =millis();
		PORTA = 0x00;
		while(1){
			reaching_time=millis() - prev;
			for (i=0;i<8;i++)
			{
				if(instr[i] == 'P'){
				n= piano_note[j];
				curr_note = control(n,curr_note);
				j++;
				}
				else if(instr[i] == 'T'){
					trumpet_striking();
				}
			}
			n= '1';
			curr_note = control(n,curr_note);
		}
	}
}

