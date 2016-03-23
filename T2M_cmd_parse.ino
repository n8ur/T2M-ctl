/***************************************************
T2Mini control software by John Ackermann N8UR.

Command parser serial functions
****************************************************/

int cmd_parse(char dataBuffer[DATABUFFERSIZE+1]) {
    board = toupper(dataBuffer[1]);
    boardnum = atoi(&dataBuffer[1]);
    cmd = toupper(dataBuffer[2]);
    execute = 1;
    // first param is board, 1 - 8; 0 addresses all
    if (!(board >= '0' && board <= '8')) {execute = 0;} // board check

    // Second is command -- A, B, C, D, S
    if (!(((cmd >= 'A') && (cmd <= 'D')) || (cmd == 'S'))) {execute = 0;} // cmd check
    
    return execute;
} // cmd_parse

