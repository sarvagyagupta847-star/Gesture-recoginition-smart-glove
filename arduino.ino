#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mp3Serial(2, 3);   // Nano D2 = RX, D3 = TX
DFRobotDFPlayerMini dfplayer;

// Flex sensor pins
#define FLEX1 A0
#define FLEX2 A2
#define FLEX3 A3

// Moving Average settings
#define N 5   // number of samples

int f1[N] = {0}, f2[N] = {0}, f3[N] = {0};
int indexMA = 0;

// Store last played audio
int lastPlayed = 0;

// Function to calculate average
int getAverage(int arr[]) {
  int sum = 0;
  for (int i = 0; i < N; i++) {
    sum += arr[i];
  }
  return sum / N;
}

void setup() {
  Serial.begin(9600);
  mp3Serial.begin(9600);

  pinMode(FLEX1, INPUT);
  pinMode(FLEX2, INPUT);
  pinMode(FLEX3, INPUT);

  Serial.println("Starting DFPlayer + Flex Sensors...");
  delay(3000);

  if (!dfplayer.begin(mp3Serial)) {
    Serial.println("DFPlayer NOT detected");
    while (true);
  }

  Serial.println("DFPlayer detected");
  dfplayer.volume(30);
  delay(1000);
}

void loop() {
  // Read raw values
  int raw1 = analogRead(FLEX1);
  int raw2 = analogRead(FLEX2);
  int raw3 = analogRead(FLEX3);

  // Store values in arrays
  f1[indexMA] = raw1;
  f2[indexMA] = raw2;
  f3[indexMA] = raw3;

  // Update index
  indexMA = (indexMA + 1) % N;

  // Get filtered values
  int flex1 = getAverage(f1);
  int flex2 = getAverage(f2);
  int flex3 = getAverage(f3);

  Serial.print("F1: ");
  Serial.print(flex1);
  Serial.print("   F2: ");
  Serial.print(flex2);
  Serial.print("   F3: ");
  Serial.println(flex3);

  // Condition for 0001.mp3
  if (flex1 >= 40 && flex1 <= 90 &&
      flex2 >= 50 && flex2 <= 80 &&
      flex3 >= 0 && flex3 <= 10) {

    if (lastPlayed != 1) {
      Serial.println("Playing 0001.mp3");
      dfplayer.play(1);
      lastPlayed = 1;
      delay(500);
    }
  }

  // Condition for 0002.mp3
  else if (flex1 >= 60 && flex1 <= 120 &&
           flex2 >= 0 && flex2 <= 0 &&
           flex3 >= 0 && flex3 <= 10) {

    if (lastPlayed != 2) {
      Serial.println("Playing 0002.mp3");
      dfplayer.play(2);
      lastPlayed = 2;
      delay(3000);
    }
  }
  else if (flex1 >= 60 && flex1 <= 110 &&
           flex2 >= 40 && flex2 <= 80 &&
           flex3 >= 60 && flex3 <= 100) {

    if (lastPlayed != 3) {
      Serial.println("Playing 0003.mp3");
      dfplayer.play(3);
      lastPlayed = 3;
      delay(500);
    }
  }

  else {
    lastPlayed = 0;
  }

  delay(200);  // reduced delay for smoother response
}