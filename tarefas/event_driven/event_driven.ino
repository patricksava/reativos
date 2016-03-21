#define BTNPIN 4
#define LEDPIN 12

#define TIMERS_MAX 2
#define BUTTONS_MAX 14

int timerReg;
int timerArr[TIMERS_MAX];
int timerBeginArr[TIMERS_MAX];
int btnPinR[BUTTONS_MAX];
int btnSttR[BUTTONS_MAX];

int state = 1;


void button_listen(int pin){
  btnPinR[pin] = 1;
  btnSttR[pin] = digitalRead(pin);
}

void button_unregister(int pin){
  btnPinR[pin] = 0;
  btnSttR[pin] = 0;
}

int timer_set(int ms){
  int timerId = timerReg % 2;
  timerArr[timerId] = millis() + ms;
  timerReg++;
  return timerId;
}

void timer_unset(int timerId){
  timerArr[timerId] = 0;
}

/* callbacks */
void button_changed(int pin, int v){
  digitalWrite(LEDPIN, v);
}

void timer_expired(int timerId){
  digitalWrite(LEDPIN, !state);
  state = !state;
  timer_set(1500);
}

void initialize() {
  pinMode(LEDPIN, OUTPUT);
  pinMode(BTNPIN,  INPUT);
  button_listen(BTNPIN);
  digitalWrite(LEDPIN, state);
  timer_set(1500);
  Serial.println("Initialized");
}

void setup() {
  Serial.begin(9600);
  timerReg = 0;
  for(int i = 0; i < TIMERS_MAX; i++) timerArr[i] = 0;
  for(int i = 0; i < TIMERS_MAX; i++) timerBeginArr[i] = 0;
  for(int i = 0; i < BUTTONS_MAX; i++) btnPinR[i] = 0;
  Serial.println("Setup ended");
  initialize();
}

void checkButtons() {
  //verifica os botoes
  for(int i = 0; i < BUTTONS_MAX; i++){
    /*Serial.print("Button ");
    Serial.print(i);
    Serial.print(" acvitated? ");
    Serial.println(btnPinR[i]);*/
    if(btnPinR[i]){
      int readState = digitalRead(i);
      Serial.print("Button state: ");
      Serial.println(readState);
      if(readState != btnSttR[i]){
        btnSttR[i] = readState;
        Serial.println("Callback called");
        Serial.print("Parameters: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(readState);
        button_changed(i, readState);
      }
    }
  }
}

void checkTimers() {
  //verifica os timers
  for(int i = 0; i < TIMERS_MAX; i++){
    if(timerArr[i] != 0 && timerArr[i] < millis()){
      timer_expired(i);
      timer_unset(i);
    }
  }
}

void loop(){
  checkButtons();
  checkTimers();
}
