int signalPin1 = 2;
int noisePin1 = 3;
int signalPin2 = 4;
int noisePin2 = 5;
int coincidencePin = 6;

int index = 0;
char msg1[256] = "";
char msg2[256] = "";
char msg3[256] = "";

int signal1 = 0;
int noise1 = 0;
int signal2 = 0;
int noise2 = 0;
int coincidence = 0;

unsigned int counter = 0;

unsigned int signalLock1 = 0;
unsigned int noiseLock1 = 0;
unsigned int signalLock2 = 0;
unsigned int noiseLock2 = 0;

unsigned int coincidentLock = 0;

unsigned long signalCount1 = 0;
unsigned long noiseCount_record1 = 0;
unsigned long signalCount2 = 0;
unsigned long noiseCount_record2 = 0;
unsigned long coincidentCount = 0;
unsigned long timer = 0;


unsigned long noiseCount1 = 0;
unsigned long noiseCount2 = 0;


unsigned long prevtime = 0;

void coincident_trigger(){
  if (signal1 == 0 && signal2 == 0 && signalLock1 == 0 && signalLock2 == 0){
    coincidentLock = 1;
    coincidentCount++;
  } else if (signal1 == 1 && signal2 == 1 && signalLock1 == 1 && signalLock2 == 1){
    coincidentLock = 0;
  } else if (coincidence == 1){
    coincidentLock = 1;
    coincidentCount++;
  } else if (coincidence == 0){
    coincidentLock = 0;    
  }
}

void trigger_function1(){
  if (signal1 == 0 && signalLock1 == 0){
    signalLock1 = 1;
    signalCount1++;
  } else if (signal1 == 1 && signalLock1 == 1){
    signalLock1 = 0;
  }
  if (noise1 == 1 && noiseLock1 == 0){
    noiseLock1 = 1;
    noiseCount1++;
  } else if (noise1 == 0 && noiseLock1 == 1){
    noiseLock1 = 0;
  }
}

void trigger_function2(){
  if (signal2 == 0 && signalLock2 == 0){
    signalLock2 = 1;
    signalCount2++;
  } else if (signal2 == 1 && signalLock2 == 1){
    signalLock2 = 0;
  }
  if (noise2 == 1 && noiseLock2 == 0){
    noiseLock2 = 1;
    noiseCount2++;
  } else if (noise2 == 0 && noiseLock2 == 1){
    noiseLock2 = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("<< Arduino Program Initiated >>");
  pinMode(signalPin1, INPUT);
  digitalWrite(signalPin1, HIGH);
  pinMode(noisePin1, INPUT);
  digitalWrite(noisePin1, HIGH);
  pinMode(signalPin2, INPUT);
  digitalWrite(signalPin2, HIGH);
  pinMode(noisePin2, INPUT);
  digitalWrite(noisePin2, HIGH);
  pinMode(coincidencePin, INPUT);
  digitalWrite(coincidencePin, LOW);
  Serial.println("Pocket Geiger Setup Loaded...");
  prevtime = millis();
  Serial.println("!! DATA ACQUISITION INITIATED !!");
}

void loop() {
  signal1 = digitalRead(signalPin1);
  signal2 = digitalRead(signalPin2);
  noise1 = digitalRead(noisePin1);
  noise2 = digitalRead(noisePin2);
  coincidence = digitalRead(coincidencePin);

  counter = random();
  
  coincident_trigger();
  if (counter % 2 == 0){
    trigger_function1();
    trigger_function2();
  } else {
    trigger_function2();
    trigger_function1();
  }
  
  if (index == 10000){
    timer = millis();
    if (noiseCount1 == 0 && noiseCount2 == 0){
      Serial.print("[PG1]:");
      Serial.print(signalCount1);
      Serial.print(" , [PG2]:");
      Serial.print(signalCount2);
      Serial.print(" , Coincidence:");
      Serial.print(coincidentCount);
      Serial.print(" , Time:");
      Serial.println(timer - prevtime);
      //Serial.print(msg2);
      //Serial.println(msg3);
    } else if (noiseCount1 == 0 && noiseCount2 != 0){
      noiseCount_record1++;
    } else if (noiseCount1 != 0 && noiseCount2 == 0){
      noiseCount_record2++;
    } else {
      noiseCount_record1++;
      noiseCount_record2++;      
    }
    noiseCount1 = 0;
    noiseCount2 = 0;
    index = 0;    
  }
  index++;
}
