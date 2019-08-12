/*
*Team id: eYRC#161
*Authorlist: Nidesh D Shetty, Shashank Jain T M, Shivaraj K M, Shrinidhi Adiga
*Filename: Instrument
*Theme: Mocking Bot
*/

#include <Instrument.h>                                                


// function declarations
void read_key();
void string_generation(char note, char* instrument);
void play_audio_file();
void boot_switch_pin_config();
void init_devices();
void check_boot_press();
void uart0_init();
void uart_tx(char data);
void uart_tx_string(char *data);
void urat_transmit(char note, char instr);


/********************************************************
Function name  :  setup
Input          :  None
Output         :  None
Functionality  :  Declaration of pins as input or output
Example call   :  Called automatically  
********************************************************/

void setup() {

   DDRA= 0x03;
   DDRF= 0x00;
   tmrpcm.quality(1);
   tmrpcm.speakerPin = 11;
   PORTA= 0x00;
   DDRH =  0x00;
   DDRE = 0xFF;
   PORTE = 0x00;
   init_devices();
   
   if (!SD.begin(SD_ChipSelectPin)) {                                         // see if the card is present and can be initialized:
  
    return;                                                                   // don't do anything more if not;
  }
  else
  { PORTA=PORTA | 0x01;
	_delay_ms(500);
	 PORTA=PORTA & 0xFE;                                                  //led will blink indicating sd card is initialized; 
	_delay_ms(500);
  }
}

/********************************************************
Function name  :  loop
Input          :  None
Output         :  None
Functionality  :  all the sub functions required for this task are called in this functions 
Example call   :  Called automatically
********************************************************/

void loop() 
{
  int l= 0;
  char instr;
  check_boot_press();                    

  while(l<12 && switch_flag == 1)
  {
  x=0;
  delay(500);
  while(x==0){
    read_key();
  }
  if(end_key == 1){
	uart_tx_string("$");
	uart_tx_string("\n");
	
	while (1)
		tmrpcm.play("end.wav");

  }
  if(x==1){
  instrument = "Pia";
  instr = 'P';
  }
  else if(x==2){
	  instrument = "Tru";
	  instr = 'T';
  }
  urat_transmit(note,instr);
  
  string_generation(note,instrument);
  play_audio_file();
}
}

