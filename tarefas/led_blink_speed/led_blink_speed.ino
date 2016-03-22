#define LED_PIN 8
#define BTN1_PIN 2
#define BTN2_PIN 4
 
unsigned long timems, btn1, btn2;
int ledState, btn1State, btn2State;
unsigned long interval;
 
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
 
  timems = 0; //ms
  btn1 = btn2 = 0;
  ledState = btn1State = btn2State = 0;
  interval = 1000; //ms
}
 
void loop() {
  unsigned long tms = millis();
  if(tms >= timems + interval){
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    timems = tms;
  }
 
  int btn1s = digitalRead(BTN1_PIN);
  int btn2s = digitalRead(BTN2_PIN);
 
  if(btn1s != btn1State && tms-btn1 > 20){
    interval += 100;
    btn1State = btn1s;
    btn1 = tms;
  }
 
  if(btn2s != btn2State && tms-btn2 > 20){
    interval -= 100;
    if(interval < 0) interval = 0;
    btn2State = btn2s;
    btn2 = tms;
  }
 
  if(btn1s && btn2s && (btn1-btn2 <= 500 || btn2-btn1 <= 500)){
    digitalWrite(LED_PIN, LOW);
    while(1);
  }
 
}
