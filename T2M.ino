/***************************************************
T2Mini Control Software by John Ackermann N8UR.
Controls up to 8 TAPR T2-Mini PPS divider boards using
T2Mini-SHIELD.

NOTE:  Ribbon cable from T2Mini-SHIELD is "twisted".

On shield end, red stripe is on pin 10; cable comes over
the top of the double-row header, with stripe to left.

On T2-Mini, cable comes over the output end of the
board, with red stripe on right.

NOTE NOTE: This could have been written much more efficiently.
The idea of using one byte to retain state for each board came from
another project where there were 8 element to track.  And there were
fewer boards.  It would have been better to use an array of bytes or
something, but since this is a one-off, I'm not taking the time now
to re-architect.
****************************************************/

char version[] = "v0.20 -- 23 March 2016";

// BOF preprocessor bug prevent - insert me on top of your arduino-code
// From: http://www.a-control.de/arduino-fehler/?lang=en
#if 1
__asm volatile ("nop");
#endif

#define FIRST_PIN 26

// uncomment to enable Wiznet W5100 ethernet shield
// #define USE_W5100
#ifdef USE_W5100
  #include <SPI.h>
  #include <EthernetUdp.h>
  #include <Dhcp.h>
  #include <Dns.h>
  #include <util.h>
  #include <Ethernet.h>
  #include <EthernetServer.h>
  #include <EthernetClient.h>

  // variable
  EthernetServer server = EthernetServer(23);
  EthernetClient client = 0;
  
  // If using ethernet, set network stuff here:
  byte mac[] = { 0xDE, 0xAD, 0x00, 0xAC, 0x00, 0x01 };
  IPAddress ip(10,73,3,178);
  IPAddress gateway(10,73,1,1);
  IPAddress subnet(255,255,0,0);
  boolean newclient = 1;
  
  // what to echo to the telnet port, if anything
  // 0 = no echo, 1 = echo raw command, 2 = interpreted output
  int telnet_verbose = 2; 

#endif  // USE_W5100
//*******************************************************

char board = ' '; // char 0 - 8 representing T2M board
int boardnum = 0; // int version of same to make some things easier
char cmd = ' '; // command -- A, B, C, D, S, R

boolean execute = 0;
int i = 0;
int j = 0;

// used for temporary storage of mask
byte cmd_mask = 0;

// these hold the current status for each board; bit 3 is normally HIGH
byte cmd_mask_1 = 0x04;
byte cmd_mask_2 = 0x04;
byte cmd_mask_3= 0x04;
byte cmd_mask_4 = 0x04;
byte cmd_mask_5 = 0x04;
byte cmd_mask_6 = 0x04;
byte cmd_mask_7= 0x04;
byte cmd_mask_8 = 0x04;

// setup for serial input
const char startChar = '<';
const char endChar = '>';

// size of serial command string; includes "<" and ">" delimiters
#define DATABUFFERSIZE 4
char dataBuffer[DATABUFFERSIZE+1]; //Add 1 for NULL terminator

void setup(void) {

  usb_setup();
  board_setup();

  #ifdef USE_W5100
    w5100_setup();
  #endif

} // setup

void loop() {
  board = ' ';
  boardnum = 0;
  cmd = ' ';
   
  if (!execute) {
    #ifdef USE_W5100
      get_w5100_data();
    #endif
    get_usb_data();
 }

 if (execute) {  
   if (boardnum == 0) {  // set all boards to this command
     for (i = 1; i <=8; i++) {
       cmd_mask = write_config(i, cmd);
       usb_print_status(i, cmd_mask);
     }
   } else {
     cmd_mask = write_config(boardnum, cmd);
     usb_print_status(boardnum, cmd_mask);
   }
  execute = 0;
  } // execute 

} // of loop

