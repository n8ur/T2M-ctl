/***************************************************
T2Mini Control Software by John Ackermann N8UR.

Hardware write functions

****************************************************/

void board_setup() {
// set configuration pins to input pullup
  int i = 0;
  int j = 0;
  for (i = FIRST_PIN; i <=FIRST_PIN+23; i++) {
    pinMode(i, OUTPUT);
    if (j == 2 || j == 5 || j == 8 || j == 11 || j == 14 || j == 17 || j == 20 || j == 23) { // 3rd pin (ARM) is normally HIGH
      digitalWrite(i,HIGH);
    } else {
      digitalWrite(i,LOW);
   }
   j++;
  }
}

byte write_config(int boardnum, char cmd) {
  byte cmd_mask = 0;
  
  switch (boardnum) {
    case 1 : cmd_mask = cmd_mask_1;break;
    case 2 : cmd_mask = cmd_mask_2;break;
    case 3 : cmd_mask = cmd_mask_3;break;
    case 4 : cmd_mask = cmd_mask_4;break;
    case 5 : cmd_mask = cmd_mask_5;break;
    case 6 : cmd_mask = cmd_mask_6;break;
    case 7 : cmd_mask = cmd_mask_7;break;
    case 8 : cmd_mask = cmd_mask_8;break;
  } // case
  switch (cmd) {
    case 'A' : cmd_mask = 0x00;break;
    case 'B' : cmd_mask = 0x01;break;
    case 'C' : cmd_mask = 0x02;break;
    case 'D' : cmd_mask = 0x03;break;
    case 'S' : break; // just go on to arm_pulse
    
  } // switch
  
  switch (boardnum) {
    case 1 : cmd_mask_1 = cmd_mask; write_board(boardnum,cmd_mask_1);break;
    case 2 : cmd_mask_2 = cmd_mask; write_board(boardnum,cmd_mask_2);break;
    case 3 : cmd_mask_3 = cmd_mask; write_board(boardnum,cmd_mask_3);break;
    case 4 : cmd_mask_4 = cmd_mask; write_board(boardnum,cmd_mask_4);break;
    case 5 : cmd_mask_5 = cmd_mask; write_board(boardnum,cmd_mask_5);break;
    case 6 : cmd_mask_6 = cmd_mask; write_board(boardnum,cmd_mask_6);break;
    case 7 : cmd_mask_7 = cmd_mask; write_board(boardnum,cmd_mask_7);break;
    case 8 : cmd_mask_8 = cmd_mask; write_board(boardnum,cmd_mask_8);break; 
  } // switch
  
  arm_pulse(boardnum);

  return cmd_mask;
} // set_cfg

void write_board(int boardnum, byte cmd_mask) {
    
    int start_counter = ((boardnum - 1) * 3) + 26; // three lines per board, boards 1 through 8, starting at pin 26
    int counter = start_counter;
    
    for (byte mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
      if (counter <= start_counter + 2) { // only check 3 LSB
        if (cmd_mask & mask) { // if bitwise AND resolves to true   
          digitalWrite(counter,HIGH);
        } else {
          digitalWrite(counter,LOW);
        } // if/else
        counter++;
      } // if 
    } // for 
}  // write_board

// Lower ARM pin of selected board for 1.1 second, then raise.
// Divider will restart on next positive edge of SYNC input 
void arm_pulse (int boardnum) { 
   int arm_pin = ((boardnum - 1) * 3) + 28;   // 3 pins per board; board 1 ARM is pin 28
   delay(1);
   digitalWrite(arm_pin,LOW);
   delay(1100);
   digitalWrite(arm_pin,HIGH);
  }
