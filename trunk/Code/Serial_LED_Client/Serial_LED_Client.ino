#include <C:/test.h> //Include VITAL RGB TILE FUNCTIONS!
unsigned char mode; //Global mode variable allows board to save/switch between different operating modes.

//Settings for strobe
char freq = 1000;
char duty = 1000;
char s_r = 255;
char s_g = 255;
char s_b = 255;


void status(){
  Serial.println("Status:");
}
void help(){
  Serial.println("Serial command shell v1.2");
  Serial.println("Copyright (c) 2012 Jorel Lalicki");
  Serial.println("Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:");
Serial.println("   • Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.");
Serial.println("   • Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.");
Serial.println("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ""AS IS"" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.");
Serial.println("\n\nCommand Reference: [* not yet fully implemented; ~ not yet implemented]\n");
Serial.println("help(); Displays the liscense and provides a command reference");
Serial.println("rcos(); Standard flash memory wasting function.");
Serial.println("reboot(); Resets variables to their initial values, and clears the bluetooth communication buffer");
Serial.println("set_rgb(char r, char g, char b); Instantly updates the PILED output color");
Serial.println("target_rgb(char r, char g, char b, unsigned int fade time); Fades smoothly into the specified color over the specified fade time\n");

Serial.println("* status(); Displays status information, such as ambient sound level, input voltage, settings for the strobe and rgb fade effects, etc");
Serial.println("* strobe(int frequency off, int frequency on, char r, char g, char b); Strobe with variable color, frequency, and duty cycle\n");

Serial.println("~rgb_fade(int time per primary color); A smooth rainbow fade with the option to change speed");
Serial.println("~audio_color(int sensitivity, int change speed); A classy effect that changes color with loud sounds, cycling from red to green to blue ");
}

void _set_rgb(char command[128], char command_length, char base_length){
  //parse out the individual strings
  char temp_num[4]; //3 chars, plus a null character to terminate it as a string for atoi()
  char color = 2;
  char pos;
  char temp = command_length - 3; //we want an index of the leftmost data value (not ")" or ";" )
  unsigned char r,g,b;
  while ( temp > base_length){
     temp_num[0] = '0';//clear the buffer for a single color
     temp_num[1] = '0';
     temp_num[2] = '0';
     temp_num[3] = 0; //null character termination
     pos = 2;
    while (command[temp] != ',' && command[temp] != '('){ //parse out separate colors
        temp_num[pos] = command[temp]; //data? I hope so!
        temp--;
        pos--;
        if (pos<-1) { //You forgot a comma, or put WAY too many digits...
           invalid_input();
           return;
        }
    }
    temp--;

    //the temp_num array should now contain a 3 digit padded null terminated ascii representation of a number
    if (color == 2) b = atoi(temp_num);
    if (color == 1) g = atoi(temp_num);
    if (color == 0) r = atoi(temp_num);
    color--; //now read in the next color   
  }
  set_rgb(r,g,b);
}
void _target_rgb(char command[128], char command_length, char base_length){
    //parse out the individual strings
  char temp_num[11]; //10 chars, plus a null character to terminate it as a string for atoi()
  char color = 3; //color here also includes the target time
  char pos;
  char temp = command_length - 3; //we want an index of the leftmost data value (not ")" or ";" )
  unsigned char r,g,b;
  unsigned int t; //store the target fade time
  
  while ( temp > base_length){
     temp_num[0] = '0';//clear the buffer for a single color
     temp_num[1] = '0';
     temp_num[2] = '0';
     temp_num[3] = '0';
     temp_num[4] = '0';
     temp_num[5] = '0';
     temp_num[6] = '0';
     temp_num[7] = '0';
     temp_num[8] = '0';
     temp_num[9] = '0';
     temp_num[10] = '0';
     temp_num[11] = 0; //null character termination
     pos = 10;
    while (command[temp] != ',' && command[temp] != '('){ //parse out separate colors
        temp_num[pos] = command[temp]; //data? I hope so!
        temp--;
        pos--;
        if (pos<-1) { //You forgot a comma, or put WAY too many digits...
           invalid_input();
           return;
        }
    }
    temp--;

    //the temp_num array should now contain a 3 digit padded null terminated ascii representation of a number
    if (color == 3) t = atoi(temp_num);
    if (color == 2) b = atoi(temp_num);
    if (color == 1) g = atoi(temp_num);
    if (color == 0) r = atoi(temp_num);
    color--; //now read in the next color   
  }
  target_rgb(r,g,b,t);  
}

void reboot(){
  setup();
}
 void invalid_input(){
   Serial.println("Error in input command. Type ""help();"" for available commands."); 
 }




void setup()
{
        // start serial port at 9600 bps:
        Serial.begin(9600);
	P1DIR |= BIT6;             // P1.2 to output
	P1SEL |= BIT6;             // P1.2 to TA0.1
	P2DIR |= BIT1;             // P2.1 to output
	P2SEL |= BIT1;             // P2.1 to TA1.1
	P2DIR |= BIT4;             // P2.4 to output
	P2SEL |= BIT4;             // P2.4 to TA1.2
 	//*************************
        //Set up timer
	CCTL1 = OUTMOD_7;// reset/set mode
	CCTL2 = OUTMOD_7;
	TA1CCTL1 = OUTMOD_7;
	TA1CCTL2 = OUTMOD_7;
	TACTL = TASSEL_2 + MC_1;           // SMCLK/8, upmode
	TA1CTL = TASSEL_2 + MC_1;
	CCR0 = period;        // period timer 0
	TA1CCR0 = period; //period timer 1
	CCR1 = 0;         // R
	CCR2 = 0;         // G
        mode = 0;
        establishContact();  // send the fun initialization data OUT; and wait for a keypress
        
      
}

void rcos(){
  //I might be an easter egg.... 
  Serial.println("          _____                    _____                   _______                   _____");          
  Serial.println("         /\\    \\                  /\\    \\                 /::\\    \\                 /\\    \\ ");         
  Serial.println("        /::\\    \\                /::\\    \\               /::::\\    \\               /::\\    \\ ");       
  Serial.println("       /::::\\    \\              /::::\\    \\             /::::::\\    \\             /::::\\    \\ ");       
  Serial.println("      /::::::\\    \\            /::::::\\    \\           /::::::::\\    \\           /::::::\\    \\ ");     
  Serial.println("     /:::/\\:::\\    \\          /:::/\\:::\\    \\         /:::/~~\\:::\\    \\         /:::/\\:::\\    \\ ");     
  Serial.println("    /:::/__\\:::\\    \\        /:::/  \\:::\\    \\       /:::/    \\:::\\    \\       /:::/__\\:::\\    \\ ");    
  Serial.println("   /::::\\   \\:::\\    \\      /:::/    \\:::\\    \\     /:::/    / \\:::\\    \\      \\:::\\   \\:::\\    \\ ");   
  Serial.println("  /::::::\\   \\:::\\    \\    /:::/    / \\:::\\    \\   /:::/____/   \\:::\\____\\   ___\\:::\\   \\:::\\    \\ ");  
  Serial.println(" /:::/\\:::\\   \\:::\\____\\  /:::/    /   \\:::\\    \\ |:::|    |     |:::|    | /\\   \\:::\\   \\:::\\    \\ "); 
  Serial.println("/:::/  \\:::\\   \\:::|    |/:::/____/     \\:::\\____\\|:::|____|     |:::|    |/::\\   \\:::\\   \\:::\\____\\ ");
  Serial.println("\\::/   |::::\\  /:::|____|\\:::\\    \\      \\::/    / \\:::\\    \\   /:::/    / \\:::\\   \\:::\\   \\::/    /");
  Serial.println(" \\/____|:::::\\/:::/    /  \\:::\\    \\      \\/____/   \\:::\\    \\ /:::/    /   \\:::\\   \\:::\\   \\/____/"); 
  Serial.println("       |:::::::::/    /    \\:::\\    \\                \\:::\\    /:::/    /     \\:::\\   \\:::\\    \\ ");     
  Serial.println("       |::|\\::::/    /      \\:::\\    \\                \\:::\\__/:::/    /       \\:::\\   \\:::\\____\\ ");    
  Serial.println("       |::| \\::/____/        \\:::\\    \\                \\::::::::/    /         \\:::\\  /:::/    /");    
  Serial.println("       |::|  ~|               \\:::\\    \\                \\::::::/    /           \\:::\\/:::/    /");     
  Serial.println("       |::|   |                \\:::\\    \\                \\::::/    /             \\::::::/    /");      
  Serial.println("       \\::|   |                 \\:::\\____\\                \\::/____/               \\::::/    /");       
  Serial.println("        \\:|   |                  \\::/    /                 ~~                      \\::/    /");        
  Serial.println("         \\|___|                   \\/____/                                           \\/____/");                                                                                                           
}



void effects_loop(){
  
  int i;
  while (Serial.available() == 0) //Loop until some serial data is recieved.
 // Serial.println("effective!");
  {
    switch (mode){
          case 0: // Direct bluetooth command mode. Do nothing - just wait for commands
          {
            //Serial.println("zero");
            break;
          }
  	  case 4://strobe - with variable frequency, duty cycle, and color
  	{
			for(i = 0; i < freq; ++i){
				sleep(1000);//this is your frequency...
		        }
                            set_rgb(s_r,s_g,s_b); //Set the colors ON as requested...
                            Serial.println("strobin");
                        for(i = 0; i < duty; ++i){
		  	        sleep(1000);//this is your duty cycle time...
		  	}
		        set_rgb(0,0,0);
                        break;
		 
        }
    }
  }
}

void strobe(char command[128], char command_length, char base_length){
 // Serial.println(command_length);
    //parse out the individual strings
  char temp_num[7]; //6 chars, plus a null character to terminate it as a string for atoi()
  char color = 4; //starts reading at the 4th argument
  char pos;
  char temp = command_length - 3; //we want an index of the leftmost data value (not ")" or ";" )
  while ( temp > base_length){
     temp_num[0] = '0';//clear the buffer for a single color
     temp_num[1] = '0';
     temp_num[2] = '0';
     temp_num[3] = '0';
     temp_num[4] = '0';
     temp_num[5] = '0';
     temp_num[6] = 0; //null character termination
     pos = 5;
    while (command[temp] != ',' && command[temp] != '('){ //parse out separate colors
        temp_num[pos] = command[temp]; //data? I hope so!
        temp--;
        pos--;
        if (pos<-1) { //You forgot a comma, or put WAY too many digits...
           invalid_input();
           return;
        }
    }
    temp--;

    //the temp_num array should now contain a 3 digit padded null terminated ascii representation of a number
    if (color == 4) s_b = atoi(temp_num);
    if (color == 3) s_g = atoi(temp_num);
    if (color == 2) s_r = atoi(temp_num);
    if (color == 1) duty = atoi(temp_num);
    if (color == 0) freq = atoi(temp_num);
    color--; //now read in the next color   
  }
  mode = 4; //set the mode to STROBE MODE
 // Serial.println("strobe data set");  
}




void loop()
{
    Serial.write('>');
    effects_loop(); //Go to the effects/mode loop, and stay there until new serial data is recieved.
    char command[128]; //command buffer
    int command_length = 0; //length of command (ending with a ;)
    char command_temp = 0;
    int base_length = 0;
    
    while (command_temp != ';'){
      while (Serial.available() < 1){} //Wait for a full 4 bytes of command instruction
      command_temp = Serial.read();
      if (command_temp == ' ' || command_temp <= 31  ){
          //character is spacing/formatting, and should be ignored. Do nothing.
      }
      else { //character is valid, and should be treated as such...
      
        command[command_length] = command_temp;
          ++command_length; //update the recorded length of the command so far
        if (command_temp == '('){
          //This is the end of the base command, and the start of arguments
          base_length = command_length - 1;
        }
      }
    }

  //read in two more lines, just for the lulz. Also, so that the strobe works (or else the LF/CR would trigger a "new command".
  while (Serial.available() < 2){}
  if (Serial.read() != '\r' &&  Serial.read() != '\n') Serial.println("Please use CR+LF terminated lines");
  //Now check which commands were requested! status, help, set_rgb, target_rgb, rcos, reboot, strobe
    
   //Serial.println(command); //debugging output
    
   if (!memcmp("status",command,6)){
        status();
    }
   else if (!memcmp("help",command,4)){
       help();
    }
    else if (!memcmp("target_rgb",command,10)){
      mode = 0;
       _target_rgb(command, command_length, base_length);
    }
   else if (!memcmp("rcos",command,4)){
     rcos();
    }  
    else if (!memcmp("set_rgb",command,7)){
      mode = 0;
      _set_rgb(command, command_length, base_length);
    }
    else if (!memcmp("reboot",command,6)){
        reboot();
    }   
    else if (!memcmp("strobe",command,6)){
        strobe(command, command_length, base_length);
    }
    else {
      invalid_input();
    }
}


void establishContact() {
  Serial.println(" _______  ___   ___      _______  ______"); 
  Serial.println("|       ||   | |   |    |       ||      |");
  Serial.println("|    _  ||   | |   |    |    ___||  _    |");
  Serial.println("|   |_| ||   | |   |    |   |___ | | |   |");
  Serial.println("|    ___||   | |   |___ |    ___|| |_|   |");
  Serial.println("|   |    |   | |       ||   |___ |       |");
  Serial.println("|___|    |___| |_______||_______||______|");
  Serial.println("\nSerial command shell v1.2");
  Serial.println("Copyright 2012 Jorel Lalicki");
  Serial.println("__________________________________________\n\n");
}


