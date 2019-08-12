

#include <Arduino.h>  
#define F_CPU 14745600                                             //defining frequency
#include <avr/io.h>                                                     //enabling the registers of micro controller
#include <util/delay.h>
#include <avr/interrupt.h>
#include <SD.h>                                                          // need to include the SD library
#define SD_ChipSelectPin 53                                              // #define SD_ChipSelectPin 53
#include <TMRpcm.h>                                                      // library to play music file on board(pulse coded modulation)
#include <SPI.h>

void uart0_init();
void uart_tx_string(char *data);
void uart_tx(char data);


// defining keys
#define RX_EN		4
#define TX_EN		3
#define UDR_EMPTY	5

TMRpcm tmrpcm;                                                             // Create an object for use in this sketch

// declarations
char note;
String mystring;
int x=0;
char* instrument;
char string[12];
char* filename;
int end_key = 0;
char sharp_note[8]={'N','N','N','N','Y','N','N','N'};                           //Configuration table given for both piano and trumpet
char octave[8]= {'5','4','3','6','5','6','5','4'};
float strike_time,prev;
int i=0,n=0;
int switch_flag = 0;

///// Trumpet sensor pins //////////

int	L = 17; //ph0   
int M = 6; //ph3
int R = 8; //ph5
   


// For debuging purpose
 
//int L_led = 5; // pe3
// int M_led = 2;  //pe4
// int R_led = 3;  //pe5  


/********************************************************
Function name  :  read_key
Input          :  None
Output         :  None
Functionality  :  Identification of the keys struck
Example call   :  read_key();
********************************************************/

void read_key()
{
	int l=0,m=0,r=0;
	int i =1;
	int del;
	for(i=1;i<2;i++){
	    l= digitalRead(L);
		m =digitalRead(M);
		r= digitalRead(R);
		del = 100 * i;
		delay(del);
	}
	if(PINF==0x02)
	{
		note='C';
		x =1;
	}
	else if(PINF==0x04)
	{
		note='D';
		x =1;
	}
	else if(PINF==0x08)
	{
		note='E';
		x =1;
	}
	else if(PINF==0x10)
	{
		note='F';
		x =1;
	}
	else if(PINF==0x20)
	{
		note='G';
		x =1;
	}
	else if(PINF==0x40)
	{
		note='A';
		x =1;
	}
	else if(PINF==0x80)
	{
		note='B';
		x =1;
	}
	else if(PINF==0x01)
	{
		end_key = 1;
		x =1;
	}
	else if(l==1 && m==0 && r==0){
		note = 'C';
		x = 2;
		PORTE = 0x08;
		
	}
	else if(l==0 && m==1 && r==0){
		note = 'D';
		x = 2;
		PORTE = 0x10;
	}
	else if(l==0 && m==0 && r==1){
		note = 'E';
		x = 2;
	PORTE = 0x20;
	}
	else if(l==1 && m==1 && r==0){
		note = 'F';
		x = 2;
PORTE = 0x18;
	}
	else if(l==0 && m==1 && r==1){
		note = 'G';
		x = 2;
PORTE = 0x30;
	}
	else if(l==1 && m==0 && r==1){
		note = 'A';
		x = 2;
PORTE =0x28;
	}
	else if(l==1 && m==1 && r==1){
		note = 'B';
		x = 2;
PORTE = 0x38;
	}
	else
	{
		x=0;
	}
	
}

/********************************************************
Function name  :  string_generation
Input          :  note, *instrument
Output         :  None
Functionality  :  Generates string to search the audio file in the SD card
Example call   :  string_generation();
********************************************************/

void string_generation(char note ,char* instrument)
{
	for(int h=0;h<12;h++){
		string[h] = '\0';
	}
	int j=0,k=0;
	string[j] = note;
	j++;
	
	if(sharp_note[i] == 'Y' ){
		string[j] = '#';
		j++;
	}
	string[j] = octave[i];
	j++;
	string[j] = '_';
	j++;
	for(k=0;k<3;k++){
		string[j] = instrument[k];
		j++;
	}
	string[j] = '.';
	j++;
	string[j] = 'w';
	j++;
	string[j] = 'a';
	j++;
	string[j] = 'v';
	i++;
	

}

/********************************************************
Function name  :  play_audio_file
Input          :  none
Output         :  None
Functionality  :  plays the respective audio file
Example call   :  play_audio_file();
********************************************************/

void play_audio_file()
{
	tmrpcm.play(string);
}

/********************************************************
Function name  :  urat_transmit
Input          :  note, instr
Output         :  None
Functionality  :  transmits the note and the instrument struck serially to the python console
Example call   :  urat_transmit();
********************************************************/

void urat_transmit(char note, char instr){
	int m =0;
	char Note[6];
	//char Onset[10];
	if(instr == 'P'){
		uart_tx_string("Piano");
		uart_tx_string("\n");
	}
	else if(instr =='T'){
		uart_tx_string("Trumpet");
		uart_tx_string("\n");
	}
	Note[m] = note;
	m++;
	if(sharp_note[n] == 'Y'){
		Note[m] = '#';
		m++;
		Note[m] = octave[n];
	}
	else{
		Note[m] = octave[n];
		m++;
		Note[m] = ' ';
	}
	n++;
	uart_tx_string(Note);
	uart_tx_string("\n");
}

/********************************************************
Function name  :  boot_switch_pin_config
Input          :  none
Output         :  None
Functionality  :  configures the boot switch to start the task when pressed
Example call   :  boot_switch_pin_config();
********************************************************/

void boot_switch_pin_config()
{
	DDRD  = DDRD & 0xBF;		                                           // set PD.6 as input
	PORTD = PORTD | 0x40;		                                           // set PD.6 HIGH to enable the internal pull-up
}

/********************************************************
Function name  :  init_devices
Input          :  none
Output         :  None
Functionality  :  function to initialize the boot and uasrt0
Example call   :  init_devices();
********************************************************/

void init_devices()
{
	boot_switch_pin_config();
	
	cli();
	
	uart0_init();
	
	sei();
}

/********************************************************
Function name  :  check_boot_press
Input          :  none
Output         :  None
Functionality  :  check the boot key status and send '#' key to start time in python console
Example call   :  check_boot_press();
********************************************************/

void check_boot_press()
{
	
	if ((PIND & 0x40) == 0x40)                                             // boot switch not pressed
	{
		switch_flag = 0;
	}
	
	else
	{
		switch_flag = 1;                                                    // if boot switch pressed
		uart_tx_string("#");                                                // send '#' to Python script
		uart_tx_string("\n");                                               // this is indicated as Start of Task
		PORTA=PORTA | 0x02;
	}
}


/********************************************************
Function name  :  uart0_init
Input          :  none
Output         :  None
Functionality  :  initialization of serial communication
Example call   :  uart0_init();
********************************************************/

void uart0_init()
{
	UCSR0B = 0x00;                                                           // disable while setting baud rate
	
	UBRR0  = 95;                                                             // for F_CPU = 14.7456 MHz and baud rate = 9600, UBBR = 95
	
	UCSR0C = UCSR0C | (1 << UCSZ01) | (1 << UCSZ00);
	
	UCSR0B = UCSR0B | (1 << RX_EN) | (1 << TX_EN);
}

/********************************************************
Function name  :  uart_tx
Input          :  data
Output         :  None
Functionality  :  sends a character byte
Example call   :  uart_tx();
********************************************************/

void uart_tx(char data)
{
	// waiting to transmit
	while(!(UCSR0A & (1 << UDR_EMPTY)));
	UDR0 = data;
}

/********************************************************
Function name  :  uart_tx_string
Input          :  data
Output         :  None
Functionality  :  sends a string
Example call   :  uart_tx_string();
********************************************************/

void uart_tx_string(char *data)
{
	while (*data != '\0')
	{
		uart_tx(*data);
		data++;
	}
}


