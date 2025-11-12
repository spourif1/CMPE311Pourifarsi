const int led1 = 8;
const int led2 = 9;

unsigned long interval1 = 0; // interval for light1
unsigned long interval2 = 0; // interval for light 2
unsigned long prevMillis1 = 0; // prev interval light1
unsigned long prevMillis2 = 0; // prev interval light2
// light states
bool ledState1 = LOW;
bool ledState2 = LOW;

int step = 0; // 0 = ask for LED, 1 = ask for interval
int selectedLED = 0; // led input

void ledInterval(unsigned long*, unsigned long*);
void checkInterval(unsigned long*, unsigned long, bool*, int);

typedef void(*task)();

void setInterval() { ledInterval(&interval1, &interval2); }
void checkLED1() { checkInterval(&prevMillis1, interval1, &ledState1, led1); }
void checkLED2() { checkInterval(&prevMillis2, interval2, &ledState2, led2); }
task task_arr[] = {setInterval, checkLED1, checkLED2};


void setup() {
  // setup LEDs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  // initiate serial
  Serial.begin(9600);
  Serial.println("Arduino: What LED? (1 or 2)");



}

void loop() {
  for (int i = 0; i < 3; i++){
    task_arr[i]();
    if ( i == 3){
      i = 0;
    }
  }
}


void ledInterval(unsigned long* interval1, unsigned long* interval2){
  if (Serial.available()) {
  String input = Serial.readStringUntil('\n'); // once user hits enter, thats the input

  if (step == 0){
    selectedLED = input.toInt();
    Serial.println("Arduino: What interval (in msec)?"); // print out to ask for interval
    step = 1;
  }
  else if (step == 1){
    unsigned long newInterval = input.toInt();
    if (selectedLED == 1){
      *interval1 = newInterval;
    }
    else if (selectedLED == 2){
      *interval2 = newInterval;
    }
    step = 0;
    Serial.println("Arduino: What LED? (1 or 2)");
  }
  }
}


void checkInterval(unsigned long* prevMillis, unsigned long interval, bool* ledState, int lednum){
  if(millis() - *prevMillis >= interval){
    *prevMillis = millis();
    *ledState = !(*ledState);
    digitalWrite(lednum, *ledState);
  }
}



