#include "test.h" //Include VITAL RGB TILE FUNCTIONS!
#define mode_max 6//Define the maximum number of modes, so the pushbuttons will iterate through them all properly. Mode 0 is direct bluetooth command, and Mode 1 is the default start-up mode (rgb mixer). 
unsigned char mode = 1; //Global mode variable allows board to save/switch between different operating modes.


//Settings for strobe
char s_r = 255;
char s_g = 255;
char s_b = 255;

//setting for sound activated colors
char ccolor = 0;
int avg = 1023; //variable for keeping a running average of the sound levels. Start at 1023 to prevent accidental triggering in a low volume situation

//setting for rainbow fade
// global variables
unsigned int rbow;     //0-360
unsigned int relative; //0-59
unsigned char rr, rg, rb;

//setting for clap activated white light
boolean clapper = 0;


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
  
  //!!! THIS IS A BLOCKING COMMAND!!!
 
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
  set_rgb(0,0,0);
  s_r = 255;
  s_g = 255;
  s_b = 255;
  avg = 1023;//reset the sound average.
  setup();
}


 void invalid_input(){
   Serial.println("Error in input command. Type ""help();"" for available commands."); 
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
  Serial.println("Copyright 2012 LIB3 Inc.");
  Serial.println("__________________________________________\n\n");
  Serial.print('>');
}



//dec_mode and inc_mode serve as the ISR functions for the two pushbuttons
void dec_mode(){
  mode--;
  if (mode == 0){
    mode = mode_max; //if we cycle to the end, continue at other end
  }
}

void inc_mode(){
  mode++;
  if (mode > mode_max){
    mode = 1;
  }  
}


void setup()
{
       setup_piled();
       mode = 1;          //Defaults to RGB mixer mode. 
       establishContact();  // send the fun initialization data OUT     
}

void loop()
{
  //Variables for serial parsing
char command[32]; //command buffer
int command_length = 0; //length of command (ending with a ;)
char command_temp = 0;
int base_length = 0;
  //Check mode, and do mode-specific stuff\
  
  while (1){
    
  int i;
  {
    switch (mode){
          case 0:{ // Direct bluetooth command mode. Do nothing - just wait for commands
            break;
          }
          case 1:
            set_rgb(analogRead(F1)/4, analogRead(F2)/4, analogRead(F3)/4);
            break;
          case 2: //the Clapper (named after Zach Clapper, creator of the first PILED Android app)
            {
            int threshold = analogRead(F1); //set the threshold from the F1 potentiometer. This is the level against which the mic signal is compared to detect transients
            int temp = analogRead(MIC);//value from 0-1023 indicating sound level. Room noise is around 100-150 with the air conditioner on... :D
      
      	    if (temp>threshold){ //if level is over threshold, it's a beat!
             //Clap on, clap off...
             if (clapper) {
               target_rgb((char)0,(char)0,(char)0,(unsigned int)200);
               clapper = 0;
             }
             else{
               target_rgb((char)255,(char)255,(char)255,(unsigned int)200);
               clapper = 1;
             }
            }
            break;
            }
          case 3:{
            int margin = analogRead(F1) / 16;//set the margin from the F1 potentiometer. This is the difference in the average level and a transient to trigger a beat.
            int temp = analogRead(MIC);//value from 0-1023 indicating sound level. Room noise is around 100-150 with the air conditioner on... :D
            avg  = (temp + 3*avg)/4; //commence with super simple running exponential averaging!  
      	    if (temp>(avg + margin)){ //if level is over average plus margin, it's a beat! (most of the time)   
              //Cycle through the colors
              if (ccolor == 2){
                 target_rgb((char)255,(char)0,(char)0,(unsigned int)10);
                 ccolor = 0;
               }              
               else if (ccolor == 1){
                 target_rgb((char)0,(char)255,(char)0,(unsigned int)10);
                 ccolor= 2;
               }
               else{
                  target_rgb((char)0,(char)0,(char)255,(unsigned int)10);
                  ccolor = 1;
               }
            }
            break;
          }
          case 4: //Color temperature fader F1
          {
            unsigned int ctemp = (analogRead(F1) / 8)-64;
            unsigned int bright = analogRead(F2)/4;
            set_rgb((bright*(191-ctemp))/256,(bright*(191))/256,(bright*(150+ctemp))/256);
            break;
          }
  	  case 5:
           {
            //strobe - with variable frequency, duty cycle, and color
            int ontime = analogRead(F1);
            int offtime = analogRead(F2);
            
	    sleep(offtime);//this is your off time...
            set_rgb(s_r,s_g,s_b); //Set the colors ON as requested...
	    sleep(ontime);//this is your on cycle time...
	    set_rgb(0,0,0);
            break;
           }
          case 6:
          {
            sleep(analogRead(F1)/64);
            rbow++;
            rbow %= 360;
            char category = rbow / 60;
            int relative = rbow % 60;
            switch(category){
              case 0:
                rr = 255;
                 rb = 0;
                 rg = relative * 17 / 4;
                           break;
              case 1:
                  rg = 255;
                  rb = 0;
                  rr = 255 - relative * 17 / 4;
                  break;
              case 2:
                  rg = 255;
                  rr = 0;
                  rb = relative * 17 / 4;
                  break;
              case 3:
                  rb = 255;
                  rr = 0;
                  rg = 255 - relative * 17 / 4;
                  break;
              case 4:
                  rb = 255;
                  rg = 0;
                  rr = relative * 17 / 4;
                  break;
              case 5:
                  rr = 255;
                  rg = 0;
                  rb = 255 - relative * 17 / 4;
                  break;
              }
              set_rgb(rr,rg,rb);
			  break;
          }     
    }
  }
  
  
   
  //check for serial stuff... Max length is 32 chars
  
    if (Serial.available() != 0){ //if there is a character in the serial buffer, check it!
      while (command_temp != ';'){
        command_temp = Serial.read();
        if (command_temp == ' ' || command_temp <= 31 || command_temp == '/r' || command_temp == '/n' ){
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
      if (command_temp == ';'){
        //We have a new command (perhaps!)
        //Now check which commands were requested! set_rgb, target_rgb, rgbmix, reboot, strobe, clapper

    
          if (!memcmp("target_rgb",command,10)){
            mode = 0;
             _target_rgb(command, command_length, base_length);
             //Prepare the console for a new command!
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              Serial.write('\n>');
          }
          else if (!memcmp("set_rgb",command,7)){
            mode = 0;
            _set_rgb(command, command_length, base_length);
              //Prepare the console for a new command!
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              Serial.write('\n>');
          }
          else if (!memcmp("rgbmix",command,6)){
            mode = 1; //set the mode to manual RGB faders
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              Serial.write('\n>');
          }
          else if (!memcmp("clapper",command,7)){
            mode = 2; //set the mode to clap activated
              //Prepare the console for a new command!
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              Serial.write('\n>');
          }
          
          else if (!memcmp("sound",command,5)){
            mode = 3; //set the mode to sound activated
              //Prepare the console for a new command!
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              Serial.write('\n>');
          }
          else if (!memcmp("color_temp",command,10)){
            mode = 4; //set the mode to manual color temperature control
              //Prepare the console for a new command!
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              Serial.write('\n>');
          }
          else if (!memcmp("rainbow",command,7)){
            mode = 6; //set the mode to demo rainbow mode
              //Prepare the console for a new command!
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              Serial.write('\n>');
          }
          else if (!memcmp("strobe",command,6)){
            mode = 5; //set the mode to STROBE MODE (manual settings)
              //Prepare the console for a new command!
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              Serial.write('\n>');
          }
          else if (!memcmp("reboot",command,6)){
              reboot();
              //Prepare the console for a new command!
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              
          }   
          else {
            invalid_input();
              //Prepare the console for a new command!
              command_length = 0; //length of command (ending with a ;)
              command_temp = 0;
              base_length = 0;
              Serial.write('\n>');
          }                  
      }    
    }  
  }
}




