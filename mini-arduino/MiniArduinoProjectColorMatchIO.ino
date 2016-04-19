#define LED_R 0
#define LED_B 1
#define LED_G 2
#define BTN_R 3
#define BTN_G 4
#define BTN_B 5

#define LED_OFF_STATE   0
#define LED_RED_STATE   1
#define LED_GREEN_STATE 3
#define LED_BLUE_STATE  2

#define BUZZ 6

#define DISP_7SEG_A 7
#define DISP_7SEG_B 8
#define DISP_7SEG_C 9
#define DISP_7SEG_D 10
#define DISP_7SEG_E 11
#define DISP_7SEG_F 12
#define DISP_7SEG_G 13

#define INIC_BLINK_RATE 1000

unsigned long ms = 0;
int ledState = LED_OFF_STATE;
int btnBlock = 1;
int combo = 0;
int blinkRate = INIC_BLINK_RATE;
int gamePaused = 1;

// the setup routine runs once when you press reset:
void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  pinMode(BUZZ, OUTPUT);
  
  pinMode(BTN_R, INPUT);
  pinMode(BTN_G, INPUT);
  pinMode(BTN_B, INPUT);
  
  pinMode(DISP_7SEG_A, OUTPUT);
  pinMode(DISP_7SEG_B, OUTPUT);
  pinMode(DISP_7SEG_C, OUTPUT);
  pinMode(DISP_7SEG_D, OUTPUT);
  pinMode(DISP_7SEG_E, OUTPUT);
  pinMode(DISP_7SEG_F, OUTPUT);
  pinMode(DISP_7SEG_G, OUTPUT);
  
  ms = millis();
}

void changeLedState(){
  digitalWrite(LED_R, ledState == LED_RED_STATE);
  digitalWrite(LED_G, ledState == LED_GREEN_STATE);
  digitalWrite(LED_B, ledState == LED_BLUE_STATE);
}

void beepBuzzer(int f){
  analogWrite(BUZZ, f);
  delay(200);
  analogWrite(BUZZ, 0);
}

void comboDisplay(){
  int d, i;
  switch(combo%10) {
    case 0: d = 0x3F; break;
    case 1: d = 0x06; break;
    case 2: d = 0x5B; break;
    case 3: d = 0x4F; break;
    case 4: d = 0x66; break;
    case 5: d = 0x6D; break;
    case 6: d = 0x7D; break;
    case 7: d = 0x07; break;
    case 8: d = 0x7F; break;
    case 9: d = 0x67; break;
    case -1: d = 0x40; break;
    default: d = 0x00; break;
  } 
  
  for(i = 0; i < 7; i++)
    digitalWrite(DISP_7SEG_A + i, !(0x01 & (d >> i)));
  
}

void pollButton(int btnPin, int ledNewState){
  if(digitalRead(btnPin)){
    btnBlock = 1;
    if(ledState == ledNewState){
      combo++;
      blinkRate = max(333, blinkRate - 100);
      ledState = LED_OFF_STATE;
      changeLedState();
    } else {
      finishGame();
    }
  }  
}

void blinkAllLeds(){
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
}

void finishGame(){
  beepBuzzer(440);
  blinkRate = INIC_BLINK_RATE;
  blinkAllLeds();
  gamePaused = 1;
}

void gameRestart(){
  combo = 0;
  blinkRate = INIC_BLINK_RATE;
  ms = millis();
  gamePaused = 0;
  
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned long now = millis();
  if(!gamePaused){
    if(now - ms >= blinkRate){
      if(!btnBlock) {
        finishGame();
      } else {
        ledState = random(1, 4);
        changeLedState();
        btnBlock = 0;
        ms = millis();
      }
    }
    
    if(!btnBlock){
      pollButton(BTN_R, LED_RED_STATE);
      pollButton(BTN_G, LED_GREEN_STATE);
      pollButton(BTN_B, LED_BLUE_STATE);
    }
    
    comboDisplay();
  } else {
    blinkAllLeds();
    if(digitalRead(BTN_R) && digitalRead(BTN_G) && digitalRead(BTN_B)){
      gameRestart();
    }
  }
}
