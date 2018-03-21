// LIBRARY ZA NEXTION
#include <doxygen.h>
#include <NexButton.h>
#include <NexCheckbox.h>
#include <NexConfig.h>
#include <NexCrop.h>
#include <NexDualStateButton.h>
#include <NexGauge.h>
#include <NexGpio.h>
#include <NexHardware.h>
#include <NexHotspot.h>
#include <NexNumber.h>
#include <NexObject.h>
#include <NexPage.h>
#include <NexPicture.h>
#include <NexProgressBar.h>
#include <NexRadio.h>
#include <NexRtc.h>
#include <NexScrolltext.h>
#include <NexSlider.h>
#include <NexText.h>
#include <NexTimer.h>
#include <Nextion.h>
#include <NexTouch.h>
#include <NexUpload.h>
#include <NexVariable.h>
#include <NexWaveform.h>
#include <Arduino.h>
#include "Nextion.h"

// LIBRARY ZA SENZORE
#include <DallasTemperature.h>
#include <OneWire.h>

// DEFINIRANJE KOMPONENTI NA NEXTION-u
NexButton myButton0 = NexButton(0,10,"page0.b2"); // TIPKA ZA MANUALNO PALJENJE PUMPE GRIJANJA
NexButton myButton1 = NexButton(0,11,"page0.b3"); // TIPKA ZA MANUALNO GAŠENJE PUMPE GRIJANJA
NexButton myButton2 = NexButton(0,12,"page0.b4"); // TIPKA ZA MANUALNO PALJENJE PUMPE SOLARNOG GRIJANJA
NexButton myButton3 = NexButton(0,13,"page0.b5"); // TIPKA ZA MANUALNO GAŠENJE PUMPE SOLARNOG GRIJANJA
NexButton myButton4 = NexButton(2,4,"page2.b1"); // TIPKA ZA POVIŠAVANJE GRANICE PALJENJA PUMPE GRIJANJA (tempHigh)
NexButton myButton5 = NexButton(2,5,"page2.b2"); // TIPKA ZA SNIŽAVANJE GRANICE PALJENJA PUMPE GRIJANJA (tempHigh)
NexButton myButton6 = NexButton(2,22,"page2.b5"); // TIPKA ZA POVIŠAVANJE GRANICE GAŠENJA PUMPE GRIJANJA (tempLow)
NexButton myButton7 = NexButton(2,23,"page2.b6"); // TIPKA ZA SNIŽAVANJE GRANICE GAŠENJA PUMPE GRIJANJA (tempLow)
NexButton myButton8 = NexButton(2,18,"page2.b3"); // TIPKA ZA POVIŠAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifHigh)
NexButton myButton9 = NexButton(2,19,"page2.b4"); // TIPKA ZA SNIŽAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifHigh)
NexButton myButton10 = NexButton(2,26,"page2.b7"); // TIPKA ZA POVIŠAVANJE GRANICE GAŠENJA PUMPE SOLARNOG GRIJANJA (tempDifLow)
NexButton myButton11 = NexButton(2,27,"page2.b8"); // TIPKA ZA SNIŽAVANJE GRANICE GAŠENJA PUMPE SOLARNOG GRIJANJA (tempDifLow)

// INICIJALIZACIJA KOMPONENTI NEXTION-a
NexTouch *nex_listen_list[] = {
  &myButton0, &myButton1, &myButton2, &myButton3, &myButton4, &myButton5, &myButton6, &myButton7, &myButton8, &myButton9, &myButton10, &myButton11,
  NULL
};


// DEFINIRANJE PINOVA ZA SENZORE TEMPERATURE
OneWire oneWire1(2);
OneWire oneWire2(3);
OneWire oneWire3(4);
OneWire oneWire4(5);
OneWire oneWire5(6);
OneWire oneWire6(7);

// DEFINIRANJE ZA DALLAS TEMPERATURE LIBRARY
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);
DallasTemperature sensors4(&oneWire4);
DallasTemperature sensors5(&oneWire5);
DallasTemperature sensors6(&oneWire6);

// DEFINIRANJE VARIJABLI ZA POHRANJIVANJE TEMPERATURI SA SENZORA
int tempPec;
int tempPovrat;
int tempBojler1;
int tempBojler2;
int tempBojler3;
int tempSolar;
int tempRazlika;

// DEFINIRANJE PINOVA ZA RELEJE
int relayPump1 = 8; // PUMPA GRIJANJA
int relayPump2 = 9; // PUMPA SOLARNOG GRIJANJA
// int relayPump3 = 27; // PUMPA SANITARNE VODE (PRIPREMA)

// DEFINIRANJE PINA I STATE-a ZA BUZZER
int buzzerPin = 50;
int buzzerState = LOW;

// DEFINIRANJE VARIJABLI GRANIČNIH TEMPERATURA
int tempHigh = 70; // GRANICA PALJENJA PUMPE GRIJANJA
int tempLow = 65; // GRANICA GAŠENJA PUMPE GRIJANJA
int tempDifHigh = 7; // GRANICA PALJENJA PUMPE SOLARNOG GRIJANJA
int tempDifLow = 2; // GRANICA GAŠENJA PUMPE SOLARNOG GRIJANJA
int tempDanger = 90; // SIGURNOSNA GRANICA TEMPERATURE PEĆI

// DEFINIRANJE VARIJEBLI ZA MANUALNO PALJENJE / GAŠENJE PUMPI
int pump1 = 0;
int pump2 = 0;

// DEFINIRANJE VARIJABLE ZA PALJENJE / GAŠENJE PUMPE SOLARNOG GRIJANJA
int solar = 0;

// DEFINIRANJE VARIJABLI ZA INTERVALNO PALJENJE BUZZERA
unsigned long previousMillis = 0;
const long interval = 1000; // VRIJEME INTERVALA BUZZERA

// FUNKCIJE ZA UPRAVLJANJE VARIJABLI POMOĆU NEXTION-a
  // MANUALNO PALJENJE PUMPE GRIJANJA
  void ButtonRelease0 (void *ptr) {
    digitalWrite(8,LOW);
    pump1 = 1;
  }
  // MANUALNO GAŠENJE PUMPE GRIJANJA
  void ButtonRelease1 (void *ptr) {
   digitalWrite(8,HIGH);
   pump1 = 0;
  }
  // MANUALNO PALJENJE PUMPE SOLARNOG GRIJANJA
  void ButtonRelease2 (void *ptr) {
    digitalWrite(9,LOW);
    pump2 = 1;
  }
  // MANUALNO GAŠENJE PUMPE SOLARNOG GRIJANJA
  void ButtonRelease3 (void *ptr) {
    digitalWrite(9,HIGH);
    pump2 = 0;
  }
  // POVIŠAVANJE GRANICE PALJENJA PUMPE GRIJANJA (tempHigh)
  void ButtonRelease4 (void *ptr) {
  if(tempHigh >= 85) {
  } else {
    tempHigh = tempHigh + 5;
    Serial.println("tempHigh is: ");
    Serial.println(tempHigh);
  }
 }

 // SNIŽAVANJE GRANICE PALJENJA PUMPE GRIJANJA (tempHigh)
  void ButtonRelease5 (void *ptr) {
  if(tempHigh <= 65) {
  } else {
    tempHigh = tempHigh - 5;
    Serial.println("tempHigh is: ");
    Serial.println(tempHigh);
  }
 }

 // POVIŠAVANJE GRANICE GAŠENJA PUMPE GRIJANJA (tempLow)
  void ButtonRelease6 (void *ptr) {
  if(tempLow >= 75) {
  } else {
    tempLow = tempLow + 5;
    Serial.println("tempLow is: ");
    Serial.println(tempLow);
  }
 }

 // SNIŽAVANJE GRANICE GAŠENJA PUMPE GRIJANJA (tempLow)
  void ButtonRelease7 (void *ptr) {
  if(tempLow <= 55) {
  } else {
    tempLow = tempLow - 5;
    Serial.println("tempLow is: ");
    Serial.println(tempLow);
  }
 }

 // POVIŠAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifHigh)
  void ButtonRelease8 (void *ptr) {
  if(tempDifHigh >= 75) { // PITAJ NIKOLU ZA GRANICU
  } else {
    tempDifHigh = tempDifHigh + 1;
    Serial.println("tempDifHigh is: ");
    Serial.println(tempDifHigh);
  }
 }

 // SNIŽAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifHigh)
  void ButtonRelease9 (void *ptr) {
  if(tempDifHigh < 0) {
  } else {
    tempDifHigh = tempDifHigh - 1;
    Serial.println("tempDifHigh is: ");
    Serial.println(tempDifHigh);
  }
 }

  // POVIŠAVANJE GRANICE GAŠENJA PUMPE SOLARNOG GRIJANJA (tempDifLow)
  void ButtonRelease10 (void *ptr) {
  if(tempDifLow >= 75) { // PITAJ NIKOLU ZA GRANICU
  } else {
    tempDifLow = tempDifLow + 1;
    Serial.println("tempDifLow is: ");
    Serial.println(tempDifLow);
  }
 }

 // SNIŽAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifLow)
  void ButtonRelease11 (void *ptr) {
  if(tempDifLow <= 0) {
  } else {
    tempDifLow = tempDifLow - 1;
    Serial.println("tempDifLow is: ");
    Serial.println(tempDifLow);
  }
 }

void setup() {
  // INICIJALIZACIJA PINOVA ZA RELEJE I GASENJE RELEJA
  pinMode(relayPump1, OUTPUT);
  pinMode(relayPump2, OUTPUT);
  digitalWrite(relayPump1, HIGH);
  digitalWrite(relayPump2, HIGH);

  // INICIJALIZACIJA PINA ZA BUZZER
  pinMode(buzzerPin, OUTPUT);

  // INICIJALIZACIJA SENZORA
  sensors1.begin();
  sensors2.begin();
  sensors3.begin();
  sensors4.begin();
  sensors5.begin();
  sensors6.begin();

  // INICIJALIZACIJA SERIJSKOG PORTA ZA DEBUGGING
  Serial.begin(9600);

  // INICIJALIZACIJA NEXTION-a
  nexInit();
   myButton0.attachPop(ButtonRelease0, &myButton0);
   myButton1.attachPop(ButtonRelease1, &myButton1);
   myButton2.attachPop(ButtonRelease2, &myButton2);
   myButton3.attachPop(ButtonRelease3, &myButton3);
   myButton4.attachPop(ButtonRelease4, &myButton4);
   myButton5.attachPop(ButtonRelease5, &myButton5);
   myButton6.attachPop(ButtonRelease6, &myButton6);
   myButton7.attachPop(ButtonRelease7, &myButton7);
   myButton8.attachPop(ButtonRelease8, &myButton8);
   myButton9.attachPop(ButtonRelease9, &myButton9);
   myButton10.attachPop(ButtonRelease10, &myButton10);
   myButton11.attachPop(ButtonRelease11, &myButton11);
}

void loop() {
  //INCIJALIZACIJA NEXTION LOOP-a
  nexLoop(nex_listen_list);
  
  // DOBIVANJE TEMPERATURI SA SENZORA
  Serial.println("Requesting temperatures!");
  sensors1.requestTemperatures();
  sensors2.requestTemperatures();
  sensors3.requestTemperatures();
  sensors4.requestTemperatures();
  sensors5.requestTemperatures();
  sensors6.requestTemperatures();

  // POHRANJIVANJE TEMPERATURA U VARIJABLE
  tempPec = sensors1.getTempCByIndex(0);
  tempPovrat = sensors2.getTempCByIndex(0);
  tempBojler1 = sensors3.getTempCByIndex(0);
  tempBojler2 = sensors4.getTempCByIndex(0);
  tempBojler3 = sensors5.getTempCByIndex(0);
  tempSolar = sensors6.getTempCByIndex(0);

  // ISPISIVANJE U SERIAL MONITOR ZA DEBUGGING
  /*
  Serial.println("Temperature u peci: ");
  Serial.println(tempPec);
  Serial.println("Temperatura povrata: ");
  Serial.println(tempPovrat);
  Serial.println("Temperatura bojler zona 1: ");
  Serial.println(tempBojler1);
  Serial.println("Temperatura bojler zona 2: ");
  Serial.println(tempBojler2);
  Serial.println("Temperatura bojler zona 3: ");
  Serial.println(tempBojler3);
  Serial.println("Temperatura solarnog: ");
  Serial.println(tempSolar);

  */
  // PALJENJE BUZZERA I PUMPE KOD PREVISOKE TEMPERATURE
  if(tempPec >= tempDanger) {
    digitalWrite(relayPump1, LOW); // PROVJERITI DAl JE HIGH ILI LOW

    // PALJENJE BUZZERA NA INTERVALE
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if(buzzerState == LOW) {
        buzzerState = HIGH;
      } else {
        buzzerState = LOW;
      }
      digitalWrite(buzzerPin, buzzerState);
    }
  }

  // IZRAČUNAVANJE RAZLIKE TEMPERATURA I PROMJENA STANJA VARIJABLE
  tempRazlika = tempSolar - tempBojler2;
  Serial.println("Razlika temperatura: ");
  Serial.println(tempRazlika);

  if(tempRazlika >= tempDifHigh) {
    solar = 1;
  } else if(tempRazlika <= tempDifLow) {
    solar = 0;
  }

  // PALJENJE / GAŠENJE PUMPE GRIJANJA
  if(pump1 == 0) {
      if(tempPec >= tempHigh) {
      digitalWrite(relayPump1, LOW); // PROVJERITI DAL JE HIGH ILI LOW
    } else if(tempPec <= tempLow) {
      digitalWrite(relayPump1, HIGH);
    }
  }
  

  // PALJENJE / GAŠENJE PUMPE SOLARNOG GRIJANJA
  if(pump2 == 0) {
     if(solar == 1) {
      digitalWrite(relayPump2, LOW); // PROVJERITI DAL JE HIGH ILI LOW
    } else if(solar == 0) {
      digitalWrite(relayPump2,HIGH);
    }
  }
}
