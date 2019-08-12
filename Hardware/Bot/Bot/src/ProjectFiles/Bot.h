/*
* Team ID: eYRC#161
* Author List: Nidesh D Shetty, Shashank Jain T M, Shivaraj K M, Shrinidhi Adiga
* Filename: Bot 
* Theme: Mocking Bot
*/

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "servo.h"


#define F_CPU 14745600

#define RX  (1<<4)
#define TX  (1<<3)
#define TE  (1<<5)
#define RE  (1<<7)

volatile unsigned char data;
int switch_flag = 0;
int flag = 0;



int a=0;
int pos = 0;
int tpos = 25;
int error3 = 2;
int error2 = 178;
int _1stKey=0;
int C=1;
int D=2;
int E=3;
int F=4;
int G=5;
int A=6;
int B=7;
int _LeftEncoderTicks = 0;
int start_position=25;
int stop_position=52;

char instr[16],notes_[16];
char instr_1[16];

///Serial data variables///
char instruments[16];
char notes[16] ;
char received_onsets[50];
////

float onset[8]; ///float onsets of an audio file

//// audio file data for piano////
char piano_note[10];
float piano_onset[10];
////

////audio file data for trumpet////
char trumpet_note[10];
float trumpet_onset[10];
////

int curr_note;
int next_note,hold_value;
int i,j,k=0,h=0;
char n;
unsigned long reaching_time,prev;
int s_pos =19;

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
Function name	:	control
Input			:	n,curr_note
Output			:	curr_note
Functionality	:	Contains the overall working of the striking mechanism
Example Call	:	var = control(x,y);
***********************************/

int control(char n, int curr_note)
{
	switch(n)
	{
		case 'A':
		{
			next_note = A;
			curr_note = calculation(curr_note,next_note);
			reaching_time=millis() - prev;
			stop_udf(50);
			onset_calculation_p(piano_onset[k]);
			hitme();
			k++;
			stop_udf(50);
			break;
		}
		case 'B':
		{
			next_note = B;
			curr_note = calculation(curr_note,next_note);
			reaching_time=millis() - prev;
			stop_udf(50);
			onset_calculation_p(piano_onset[k]);
			hitme();
			k++;
			stop_udf(50);
			break;
		}
		case 'C':
		{
			next_note = C;
			curr_note = calculation(curr_note,next_note);
			reaching_time=millis() - prev;
			stop_udf(50);
			onset_calculation_p(piano_onset[k]);
			hitme();
			k++;
			stop_udf(50);
			break;
		}
		case 'D':
		{
			next_note = D;
			curr_note = calculation(curr_note,next_note);
			reaching_time=millis() - prev ;
			stop_udf(50);
			onset_calculation_p(piano_onset[k]);
			hitme();
			k++;
			stop_udf(50);
			break;
		}
		case 'E':
		{
			next_note = E;
			curr_note = calculation(curr_note,next_note);
			reaching_time=millis() - prev;
			stop_udf(50);
			onset_calculation_p(piano_onset[k]);
			hitme();
			k++;
			stop_udf(50);
			break;
		}
		case 'F':
		{
			next_note = F;
			curr_note = calculation(curr_note,next_note);
			reaching_time=millis() - prev;
			stop_udf(50);
			onset_calculation_p(piano_onset[k]);
			hitme();
			k++;
			stop_udf(50);
			break;
		}
		case 'G':
		{
			next_note = G;
			curr_note = calculation(curr_note,next_note);
			reaching_time=millis() - prev;
			stop_udf(50);
			onset_calculation_p(piano_onset[k]);
			hitme();
			k++;
			stop_udf(50);
			break;
		}
		case '1':
		{
			next_note = _1stKey;
			curr_note = calculation(curr_note,next_note);
			reaching_time=millis() - prev;
			stop_udf(50);
			onset_calculation_p(piano_onset[k]);
			hitme();
			k++;
			stop_udf(30000);
			break;
		}
		default :
		{
			PORTH = 0x00;
			break;
		}
	}
	return curr_note;
}

////////////////////Trumpet functions////////

/**********************************
Function name	:	trumpet_striking
Input			:	None
Output			:	None
Functionality	:	Overall striking of the trumpet keys
Example Call	:	trumpet_striking();
***********************************/

void trumpet_striking(){
	reaching_time=millis() - prev;
	switch (trumpet_note[a])
	{
		case 'C' :
		{
			onset_calculation_t(trumpet_onset[h]);
			hit_trumpet(1,0,0);
			h++;
			break;
		}
		case 'D' :
		{
			onset_calculation_t(trumpet_onset[h]);
			hit_trumpet(0,1,0);
			h++;
			break;
		}
		case 'E' :
		{
			onset_calculation_t(trumpet_onset[h]);
			hit_trumpet(0,0,1);
			h++;
			break;
		}
		case 'F' :
		{
			onset_calculation_t(trumpet_onset[h]);
			hit_trumpet(1,1,0);
			h++;
			break;
		}
		case 'G' :
		{
			onset_calculation_t(trumpet_onset[h]);
			hit_trumpet(0,1,1);
			h++;
			break;
		}
		case 'A' :
		{
			onset_calculation_t(trumpet_onset[h]);
			hit_trumpet(1,0,1);
			h++;
			break;
		}
		case 'B' :
		{
			onset_calculation_t(trumpet_onset[h]);
			hit_trumpet(1,1,1);
			h++;
			break;
		}
	}
	a++;
}


/**********************************
Function name	:	rotationf
Input			:	None
Output			:	None
Functionality	:	Forward rotation of DC motor
Example Call	:	rotationf();
***********************************/

void rotationf()
{
	_LeftEncoderTicks = 0;
	while(_LeftEncoderTicks<=30)
	{
		PORTH = 0x01;
	}
}
/**********************************
Function name	:	rotationb
Input			:	None
Output			:	None
Functionality	:	Backward rotation of DC motor
Example Call	:	rotationb();
***********************************/

void rotationb()
{
	_LeftEncoderTicks = 0;
	while(_LeftEncoderTicks<=31)
	{
		PORTH = 0x02;
	}
}

/**********************************
Function name	:	stop_udf
Input			:	delay_udf
Output			:	None
Functionality	:	Provides a delay
Example Call	:	stop_udf();
***********************************/

void stop_udf(int delay_udf)
{
	PORTH = 0x00;
	delay(delay_udf);
}

/**********************************
Function name	:	calculation
Input			:	curr_note,next_note
Output			:	curr_note
Functionality	:	Analyzes the note to be played and calculates difference between current note and next note
Example Call	:	var=calculation(x,y);
***********************************/

int calculation(int curr_note ,int next_note)
{
	int diff=0;
	diff = next_note - curr_note;
	if(diff > 0)
	{
		for(j= 0 ; j <diff ; j++)
		{
			rotationf();
		}
	}
	else if(diff < 0)
	{
		for(j = 0 ; j <abs(diff) ; j++)
		{
			rotationb();
		}
	}
	else
	{
		stop_udf(5000);
	}
	curr_note = next_note;
	return curr_note;
}

/**********************************
Function name	:	hitme
Input			:	None
Output			:	None
Functionality	:	Rotation of the servo for striking the key
Example Call	:	hitme();
***********************************/

void hitme()
{
	for(pos = 19; pos >= 10; pos -= 3)
	{
		servo_1(pos);
		delay(5);
	}
	delay(600);
	for(pos = 10; pos <= 19; pos +=3)
	{
		servo_1(pos);
		delay(5);
	}
}
/**********************************
Function name	:	hit_trumpet
Input			:	None
Output			:	None
Functionality	:	Controls the rotation of the servos for striking the trumpet key 
Example Call	:	hit_trumpet();
***********************************/

void hit_trumpet(int l,int m,int r)
{
	// Variables l, m, r can take values 0 or 1 thus representing the keys to be struck or not 
	
	if (l==1 && m==0 && r==0)// note "C"
	{
		for (pos = start_position; pos <=stop_position; pos += 2)
		{
			servo_2(pos);
			servo_3(error2-tpos);
			servo_4(tpos-error3);
			delay(10);
		}
		delay(350);
		for (pos =stop_position; pos >=start_position; pos -= 2)
		{ 
			servo_2(pos);
			servo_3(error2-tpos);
			servo_4(tpos-error3); 
			delay(10);
		}
	}
	else if (l==0 && m==1 && r==0)// note "D"
	{
		for (pos = start_position; pos <=stop_position; pos += 2)
		{
			servo_2(tpos);
			servo_3(error2-pos);
			servo_4(tpos-error3);
			
			delay(10);
		}
		delay(400);
		for (pos =stop_position; pos >=start_position; pos -= 2)
		{ 
			servo_2(tpos);
			servo_3(error2-pos);
			servo_4(tpos-error3); 
			delay(10);
		}
	}
	else if(l==0 && m==0 && r==1)// note "E"
	{
		for (pos = start_position; pos <=stop_position; pos += 2)
		{
			servo_2(tpos);
			servo_3(error2-tpos);
			servo_4(pos-error3);
			delay(10);
		}
		delay(350);
		for (pos =stop_position; pos >=start_position; pos -= 2)
		{ 
			servo_2(tpos);
			servo_3(error2-tpos);
			servo_4(pos-error3); 
			delay(10);
		}
	}
	else if(l==1 && m==1 && r==0)// note "F"
	{
		for (pos = start_position; pos <=stop_position; pos += 2)
		{
			servo_2(pos);
			servo_3(error2-pos);
			servo_4(tpos-error3);
			delay(10);
		}
		delay(350);
		for (pos =stop_position; pos >=start_position; pos -= 2)
		{ 
			servo_2(pos);
			servo_3(error2-pos);
			servo_4(tpos-error3); 
			delay(10);
		}
	}
	else if(l==0 && m==1 && r==1)// note "G"
	{
		for (pos = start_position; pos <=stop_position; pos += 2)
		{
			servo_2(tpos);
			servo_3(error2-pos);
			servo_4(pos-error3);
			delay(10);
		}
		delay(350);
		for (pos =stop_position; pos >=start_position; pos -= 2)
		{ 
			servo_2(tpos);
			servo_3(error2-pos);
			servo_4(pos-error3); 
			delay(10);
		}
	}
	else if(l==1 && m==0 && r==1)// note "A"
	{
		for (pos = start_position; pos <=stop_position; pos += 2)
		{
			servo_2(pos);
			servo_3(error2-tpos);
			servo_4(pos-error3);
			delay(10);
		}
		delay(350);
		for (pos =stop_position; pos >=start_position; pos -= 2)
		{ 
			servo_2(pos);
			servo_3(error2-tpos);
			servo_4(pos-error3);
			delay(10);
		}
	}
	else if(l==1 && m==1 && r==1)// note "B"
	{
		for (pos = start_position; pos <=stop_position; pos += 2)
		{
			servo_2(pos);
			servo_3(error2-pos);
			servo_4(pos-error3);
			delay(10);
		}
		delay(350);
		for (pos =stop_position; pos >=start_position; pos -= 2)
		{ 
			servo_2(pos);
			servo_3(error2-pos);
			servo_4(pos-6); 
			delay(10);
		}
	}
}

/**********************************
Function name	:	positive_dir
Input			:	none
Output			:	None
Functionality	:	Increments the count in the interrupt
Example Call	:	positive_dir();
***********************************/

void positive_dir()
{
	_LeftEncoderTicks+=1;
}


//// UART functions.../////

/**********************************
Function name	:	uart0_init
Input			:	None
Output			:	None
Functionality	:	Initializing the board for Serial Communication
Example Call	:	uart0_init();
***********************************/

void uart0_init()
{
	UCSR0B = 0x00;							//disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x5F; 							//9600BPS at 14745600Hz
	UBRR0H = 0x00;
	UCSR0B = 0x98;
	UCSR0C = 3<<1;							//setting 8-bit character and 1 stop bit
	UCSR0B = RX | TX;
}

/**********************************
Function name	:	uart_tx
Input			:	data
Output			:	None
Functionality	:	transmission of data from Atmega 2560 to serial port
Example Call	:	uart_tx(data);
***********************************/

void uart_tx(char data)
{
	while(!(UCSR0A & TE));						//waiting to transmit
	UDR0 = data;
}

/**********************************
Function name	:	Interrupt Service Routine
Input			:	None
Output			:	None
Functionality	:	The interrupt is called when the data is received in the UDR register
Example Call	:	Called automatically
***********************************/

ISR(USART0_RX_vect)
{
	data = UDR0;
}

/**********************************
Function name	:	uart_rx
Input			:	None
Output			:	None
Functionality	:	Receives data from serial port to Atmega 2560
Example Call	:	uart_rx();
***********************************/

char uart_rx()
{
	while(!(UCSR0A & RE));						//waiting to receive
	return UDR0;
}

/**********************************
Function name	:	serial_communication
Input			:	None
Output			:	None
Functionality	:	Communicates between Atmega 2560 and serial port 
Example Call	:	serial_communication();
***********************************/

void serial_communication()
{
	uart0_init();
	char data1;
	char data2;
	char data3;
	int i=0;

	for(i=0;i<16;i++){
		data1=uart_rx();	//Receives type of instrument from Serial port and stores in data1 
		instruments[i] = data1;
	}

	for(i=0 ; i<16;i++)
	{
		uart_tx(instruments[i]);	//Transmits data from Atmega 2560 to Serial port for verification
	}
	for(i=0;i<40;i++){
		data2=uart_rx();
		received_onsets[i] = data2;	//Receives the onsets of audio file and stores in data2
	}
	for(i=0 ; i<40;i++)
	{
		uart_tx(received_onsets[i]);	//Transmits data from Atmega 2560 to Serial port for verification
	}
	for(i=0;i<16;i++){
		data3=uart_rx();	//Receives the notes of audio file and stores in data3
		notes[i] = data3;
	}
	for(i=0 ; i<16;i++)
	{
		uart_tx(notes[i]);	//Transmits data from Atmega 2560 to Serial port for verification
	}
	PORTA = 0X00;
	flag = 1;
}




/**********************************
Function name	:	array_segmentation
Input			:	None
Output			:	None
Functionality	:	Conversion of the received string to the desired format
Example Call	:	array_segmentation();
***********************************/

void array_segmentation(){
	int i=0,j=0,k=0,l=0,m=0;
	char sig_onset[5];
	char result[5];

	while(i < strlen(instruments)){
		if(instruments[i] != ','){
			instr[j] = instruments[i];	//The instrument string received is converted and individual data is stored in the array.
			uart_tx(instr[j]);
			j++;
		}
		i++;
	}
	while(i<strlen(instr)){
		instr_1[i] = instr[i];
	}
	i=0;j=0;
	while(i < strlen(notes)){
		if(notes[i] != ','){
			notes_[j] = notes[i];	//The note string received is converted and individual data is stored in the array.
			j++;
		}
		i++;
	}
	i=0;j=0;
	while(i < strlen(received_onsets))
	{
		j=0;
		while(received_onsets[i] != ',' && i < strlen(received_onsets))
		{
			sig_onset[j] = received_onsets[i];	//The string received is converted and individual data is stored in the array.
			j++;
			i++;
		}
		i++;
		onset[k] = atof(sig_onset);	// The individual onsets present in the character data type are stored as float in the onset array 
		k++;
	}
	for(k=0;k<8;k++){
		uart_tx(instr_1[k]);
	}
	j=0;i=0,k=0;
	
	while(i<16){
		if(instr[i] =='P'){		//Detects whether the instrument is piano or trumpet 
			uart_tx('p');
			piano_note[j] = notes_[i];
			piano_onset[j] = onset[i];
			j++;
		}
		if(instr[i] == 'T'){
			uart_tx('t');
			trumpet_note[k] = notes_[i];
			trumpet_onset[k] = onset[i];
			k++;
		}
		i++;
	}
	piano_onset[j] = 12.0 ;		//Onset of the end key which changes according to audio file
	piano_note[j] = '1';
	
}

///////////////

/**********************************
Function name	:	onset_calculation_p
Input			:	onset_p
Output			:	None
Functionality	:	Calculate the waiting time for the striking mechanism of piano to strike at a given onset time
Example Call	:	onset_calculation_p();
***********************************/

void onset_calculation_p(float onset_p){
	if (onset_p>(reaching_time/1000))
	{
		hold_value = 1000 * abs(onset_p- reaching_time/1000) - 1500;	//Calculates the difference between the actual onset and the present time of arrival and provides a delay accordingly
		if(hold_value<0)
		hold_value=0;
		stop_udf(hold_value);
		
	}
}

/**********************************
Function name	:	onset_calculation_t
Input			:	onset_t
Output			:	None
Functionality	:	Calculate the waiting time for the striking mechanism of trumpet to strike at a given onset time
Example Call	:	onset_calculation_t();
***********************************/

void onset_calculation_t(float onset_t){
	if (onset_t>(reaching_time/1000))
	{
		hold_value = 1000 * abs(onset_t- reaching_time/1000)-500;		//Calculates the difference between the actual onset and the present time of arrival and provides a delay accordingly
		if(hold_value<0)
		hold_value=0;
		stop_udf(hold_value);
		
	}
}

//////// boot key functions////

/**********************************
Function name	:	boot_switch_pin_config
Input			:	None
Output			:	None
Functionality	:	Configuration of boot key
Example Call	:	boot_switch_pin_config();
***********************************/

void boot_switch_pin_config()
{
	DDRD  = DDRD & 0xBF;		// set PD.6 as input
	PORTD = PORTD | 0x40;		// set PD.6 HIGH to enable the internal pull-up
}


// initialize all devices

/**********************************
Function name	:	check_boot_press
Input			:	None
Output			:	None
Functionality	:	Checks if the boot key is pressed
Example Call	:	check_boot_press();
***********************************/

void check_boot_press()
{
	//Initially boot switch not pressed
	if ((PIND & 0x40) == 0x40)
	{
		switch_flag = 0;
	}
	// if boot switch pressed
	// send '#' to Python script
	// this is indicated as Start of Task
	else
	{
		switch_flag = 1;
	}
}


