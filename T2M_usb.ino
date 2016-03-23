/***************************************************
T2MINI control software by John Ackermann N8UR.

USB serial functions
****************************************************/

void usb_setup() {
// initialize serial:
  Serial.begin(115200);
  delay(100);
// splash
  Serial.println("");
  Serial.println("TAPR T2-Mini Control Software ");
  Serial.println(version);
  Serial.println("Copyright 2016 John Ackermann   N8UR");
  Serial.println("Licensed under MIT license");
  Serial.println("");
  Serial.println("Command example: <2B> (arrow, boardnum, command, arrow)");
  Serial.println("Boardnum 1-8, 0 addresses all");
  Serial.println("Command: A = 1 MHz, B = 2.5 MHz, C = 5 MHz, D = 10 MHz");
  Serial.println("         S = arm for Sync (divider restarts after any ");
  Serial.println("         command on next sync input positive edge)");
  Serial.println("");
} // usb_setup()


boolean get_usb_data() {
  if ( getSerialString() ) {
    execute = cmd_parse(dataBuffer);
    return execute;
  }
} //get_usb_data

boolean getSerialString(){
    static byte dataBufferIndex = 0;
    boolean storeString;
    while(Serial.available()>0){
        char incomingbyte = Serial.read();
        if(incomingbyte==startChar){
            dataBufferIndex = 0;  //Initialize dataBufferIndex variable
            storeString = true;
        }
        if(storeString){
            // Abort if index outside buffer size
            if(dataBufferIndex==DATABUFFERSIZE){
                dataBufferIndex = 0;
                break;
            }
            if(incomingbyte==endChar){
                dataBuffer[dataBufferIndex] = 0; //null terminate the C string
                //Data string is complete.
                storeString = false;
                return true;
            }
    else{
                dataBuffer[dataBufferIndex++] = incomingbyte;
                dataBuffer[dataBufferIndex] = 0; //null terminate the C string
            }
        }
        else{
        }
    }

    //All data read; no valid string
    return false;
}

void usb_print_status(int boardnum, byte cmd_mask){
  Serial.print ("Board: ");
  Serial.print (boardnum);
  if (board == '0') { Serial.print(" (Select All)"); }
  Serial.print (" command: ");
  Serial.print (cmd);
  Serial.print("     Mask: "); 
  usb_print_bits(cmd_mask);   
  Serial.println("");     
}

void usb_print_bits(byte myByte){
 for (byte mask = 0x80; mask; mask >>= 1){
   if (mask  & myByte)
       Serial.print('1');
   else
       Serial.print('0');
   }
} // usb_printBits()

