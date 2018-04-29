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
NexButton myButton0 = NexButton(0,5,"page0.b2"); // TIPKA ZA MANUALNO PALJENJE PUMPE GRIJANJA
NexButton myButton1 = NexButton(0,6,"page0.b3"); // TIPKA ZA MANUALNO GAŠENJE PUMPE GRIJANJA
NexButton myButton2 = NexButton(0,7,"page0.b4"); // TIPKA ZA MANUALNO PALJENJE PUMPE SOLARNOG GRIJANJA
NexButton myButton3 = NexButton(0,8,"page0.b5"); // TIPKA ZA MANUALNO GAŠENJE PUMPE SOLARNOG GRIJANJA
NexButton myButton4 = NexButton(2,3,"page2.b1"); // TIPKA ZA POVIŠAVANJE GRANICE PALJENJA PUMPE GRIJANJA (tempHigh)
NexButton myButton5 = NexButton(2,4,"page2.b2"); // TIPKA ZA SNIŽAVANJE GRANICE PALJENJA PUMPE GRIJANJA (tempHigh)
NexButton myButton6 = NexButton(2,9,"page2.b5"); // TIPKA ZA POVIŠAVANJE GRANICE GAŠENJA PUMPE GRIJANJA (tempLow)
NexButton myButton7 = NexButton(2,10,"page2.b6"); // TIPKA ZA SNIŽAVANJE GRANICE GAŠENJA PUMPE GRIJANJA (tempLow)
NexButton myButton8 = NexButton(2,6,"page2.b3"); // TIPKA ZA POVIŠAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifHigh)
NexButton myButton9 = NexButton(2,7,"page2.b4"); // TIPKA ZA SNIŽAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifHigh)
NexButton myButton10 = NexButton(2,12,"page2.b7"); // TIPKA ZA POVIŠAVANJE GRANICE GAŠENJA PUMPE SOLARNOG GRIJANJA (tempDifLow)
NexButton myButton11 = NexButton(2,13,"page2.b8"); // TIPKA ZA SNIŽAVANJE GRANICE GAŠENJA PUMPE SOLARNOG GRIJANJA (tempDifLow)
NexButton myButton12 = NexButton(3,3,"page3.b1"); // TIPKA ZA RESETIRANJE GRANIČNIH TEMPERATURA
NexButton myButton13 = NexButton(2,22,"page2.b10"); // TIPKA ZA REFRESH TEMPERATURA SERVICE
NexButton myButton14 = NexButton(0,22,"page0.b1"); // TIPKA ZA REFRESH TEMPERATURA HOME

NexNumber n0 = NexNumber(0, 9, "n0"); // TEKST TEMPERATURE U PEČI
NexNumber n1 = NexNumber(0, 10, "n1"); // TEKST TEMPERATURE POVRATA
NexNumber n2 = NexNumber(0, 12, "n2"); // TEKST TEMPERATURE BOJLER ZONA 1
NexNumber n3 = NexNumber(0, 14, "n3"); // TEKST TEMPERATURE BOJLER ZONA 2
NexNumber n4 = NexNumber(0, 16, "n4"); // TEKST TEMPERATURE BOJLER ZONA 3
NexNumber n5 = NexNumber(0, 18, "n5"); // TEKST TEMPERATURE SOLARNO

NexNumber n60 = NexNumber(2, 31, "n60"); // TEKST GRANICE PALJENJA PUMPE GRIJANJA
NexNumber n61 = NexNumber(2, 33, "n61"); // TEKST GRANICE GAŠENJA PUMPE GRIJANJA
NexNumber n62 = NexNumber(2, 35, "n62"); // TEKST GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA
NexNumber n63 = NexNumber(2, 36, "n63"); // TEKST GRANICE GAŠENJA PUMPE SOLARNOG GRIJANJA

NexPicture p1 = NexPicture(0, 20, "p1"); // SLIKA PUMPE 1
NexPicture p2 = NexPicture(0, 21, "p2"); // SLIKA PUMPE 2



// INICIJALIZACIJA KOMPONENTI NEXTION-a
NexTouch *nex_listen_list[] = {
  &myButton0, &myButton1, &myButton2, &myButton3, &myButton4, &myButton5, &myButton6, &myButton7, &myButton8, &myButton9, &myButton10, &myButton11, &myButton12, &myButton13, &myButton14, &n0, &n1, &n2, &n3, &n4, &n5, &n60, &n61, &n62, &n63, &p1, &p2,  
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

// DEFINIRANJE VARIJABLI ZA POHRANJIVANJE PRETHODNE TEMPERATURE SA SENZORA
int tempPecOld;
int tempSolarOld;

// DEFINIRANJE PINOVA ZA RELEJE
int relayPump1 = 8; // PUMPA GRIJANJA
int relayPump2 = 9; // PUMPA SOLARNOG GRIJANJA
// int relayPump3 = 27; // PUMPA SANITARNE VODE (PRIPREMA)

// DEFINIRANJE PINA I STATE-a ZA BUZZER
int buzzerPin = 50;
int buzzerState = LOW;

// DEFINIRANJE VARIJABLI GRANIČNIH TEMPERATURA
int tempHigh = 68; // GRANICA PALJENJA PUMPE GRIJANJA
int tempLow = 56; // GRANICA GAŠENJA PUMPE GRIJANJA
int tempDifHigh = 20; // GRANICA PALJENJA PUMPE SOLARNOG GRIJANJA
int tempDifLow = 7; // GRANICA GAŠENJA PUMPE SOLARNOG GRIJANJA
int tempDanger = 100; // SIGURNOSNA GRANICA TEMPERATURE PEĆI
int tempDangerSolar = 100; // SIGURNOSNA GRANICA TEMPERATURE SOLARNOG KOLEKTORA

// DEFINIRANJE VARIJEBLI ZA MANUALNO PALJENJE / GAŠENJE PUMPI
int pump1 = 0;
int pump2 = 0;

// DEFINIRANJE VARIJABLE ZA PALJENJE / GAŠENJE PUMPI
int pec = 0;
int solar = 0;

// DEFINIRANJE VARIJABLI ZA INTERVALNO PALJENJE SENZORA PEČI I SOLARNOG
unsigned long previousMillis = 0;
const long interval = 3000; // VRIJEME INTERVALA SENZORA

// DEFINIRANJE VARIJABLI ZA INTERVALNO PALJENJE BUZZERA
int buzz = 0;
unsigned long previousMillis2 = 0;
const long interval2 = 3000; // VRIJEME INTERVALA BUZZERA

// DEFINIRANJE VARIJABLI ZA INTERVALNO PALJENJE SENZORA POVRATA I BOJLERA
unsigned long previousMillis3 = 0;
const long interval3 = 30000; // VRIJEME INTERVALA SENZORA

// DEFINIRANJE VARIJABLI ZA INTERVALE ZA ON/OFF
unsigned long previousMillis4 = 0;
const long interval4 = 1000; // VRIJEME INTERVALA

// DEFINIRANJE VARIJABLI ZA PALJENJE PUMPI U SLUČAJU KVARA SENZORA
int error1 = 0;
int error2 = 0;

int a = 0; // VARIJABLA ZA INICIJALNO DOBIVANJE TEMPERATURA
int x = 0; // VARIJABLA ZA ON/OFF PUMPE GRIJANJA
int y = 0; // VARIJABLA ZA ON/OFF PUMPE SOLARNOG GRIJANJA

// FUNKCIJE ZA UPRAVLJANJE VARIJABLI POMOĆU NEXTION-a
  // MANUALNO PALJENJE PUMPE GRIJANJA
  void ButtonRelease0 (void *ptr) {
    digitalWrite(8,LOW);
    pump1 = 1;
    x = 1;
  }
  // MANUALNO GAŠENJE PUMPE GRIJANJA
  void ButtonRelease1 (void *ptr) {
    digitalWrite(8,HIGH);
    pump1 = 0;
    x = 0;
  }
  // MANUALNO PALJENJE PUMPE SOLARNOG GRIJANJA
  void ButtonRelease2 (void *ptr) {
    digitalWrite(9,LOW);
    pump2 = 1;
    y = 1;
  }
  // MANUALNO GAŠENJE PUMPE SOLARNOG GRIJANJA
  void ButtonRelease3 (void *ptr) {
    digitalWrite(9,HIGH);
    pump2 = 0;
    y = 0;
  }
  // POVIŠAVANJE GRANICE PALJENJA PUMPE GRIJANJA (tempHigh)
  void ButtonRelease4 (void *ptr) {
  if(tempHigh >= 85) {
  } else {
    tempHigh = tempHigh + 2;
    n60.setValue(tempHigh);
    /*
    Serial.println("tempHigh is: ");
    Serial.println(tempHigh);
    */
  }
 }

 // SNIŽAVANJE GRANICE PALJENJA PUMPE GRIJANJA (tempHigh)
  void ButtonRelease5 (void *ptr) {
  if(tempHigh <= 65) {
  } else {
    tempHigh = tempHigh - 2;
    n60.setValue(tempHigh);
    /*
    Serial.println("tempHigh is: ");
    Serial.println(tempHigh);
    */
  }
 }

 // POVIŠAVANJE GRANICE GAŠENJA PUMPE GRIJANJA (tempLow)
  void ButtonRelease6 (void *ptr) {
  if(tempLow >= 75) {
  } else {
    tempLow = tempLow + 2;
    n61.setValue(tempLow);
    /*
    Serial.println("tempLow is: ");
    Serial.println(tempLow);
    */
  }
 }

 // SNIŽAVANJE GRANICE GAŠENJA PUMPE GRIJANJA (tempLow)
  void ButtonRelease7 (void *ptr) {
  if(tempLow <= 55) {
  } else {
    tempLow = tempLow - 2;
    n61.setValue(tempLow);
    /*
    Serial.println("tempLow is: ");
    Serial.println(tempLow);
    */
  }
 }

 // POVIŠAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifHigh)
  void ButtonRelease8 (void *ptr) {
  if(tempDifHigh >= 75) { // PITAJ NIKOLU ZA GRANICU
  } else {
    tempDifHigh = tempDifHigh + 1;
    n62.setValue(tempDifHigh);
    /*
    Serial.println("tempDifHigh is: ");
    Serial.println(tempDifHigh);
    */
  }
 }

 // SNIŽAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifHigh)
  void ButtonRelease9 (void *ptr) {
  if(tempDifHigh < 0) {
  } else {
    tempDifHigh = tempDifHigh - 1;
    n62.setValue(tempDifHigh);
    /*
    Serial.println("tempDifHigh is: ");
    Serial.println(tempDifHigh);
    */
  }
 }

  // POVIŠAVANJE GRANICE GAŠENJA PUMPE SOLARNOG GRIJANJA (tempDifLow)
  void ButtonRelease10 (void *ptr) {
  if(tempDifLow >= 75) { // PITAJ NIKOLU ZA GRANICU
  } else {
    tempDifLow = tempDifLow + 1;
    n63.setValue(tempDifLow);
    /*
    Serial.println("tempDifLow is: ");
    Serial.println(tempDifLow);
    */
  }
 }

 // SNIŽAVANJE GRANICE PALJENJA PUMPE SOLARNOG GRIJANJA (tempDifLow)
  void ButtonRelease11 (void *ptr) {
  if(tempDifLow <= 0) {
  } else {
    tempDifLow = tempDifLow - 1;
    n63.setValue(tempDifLow);
    /*
    Serial.println("tempDifLow is: ");
    Serial.println(tempDifLow);
    */
  }
 }

 // RESETIRANJE GRANIČNIH TEMPERATURA
  void ButtonRelease12 (void *ptr) {
    tempHigh = 68; // GRANICA PALJENJA PUMPE GRIJANJA
    tempLow = 56; // GRANICA GAŠENJA PUMPE GRIJANJA
    tempDifHigh = 20; // GRANICA PALJENJA PUMPE SOLARNOG GRIJANJA
    tempDifLow = 7; // GRANICA GAŠENJA PUMPE SOLARNOG GRIJANJA
    /*
    Serial.println(tempHigh);
    Serial.println(tempLow);
    Serial.println(tempDifHigh);
    Serial.println(tempDifLow);
    */
 }

 // UPDATE GRANIČNIH TEMPERATURA
  void ButtonRelease13 (void *ptr) {
    n60.setValue(tempHigh);
    n61.setValue(tempLow);
    n62.setValue(tempDifHigh);
    n63.setValue(tempDifLow);
 }

 // UPDATE TEMPERATURA HOME
  void ButtonRelease14 (void *ptr) {
    n0.setValue(tempPec);
    n1.setValue(tempPovrat);
    n2.setValue(tempBojler1);
    n3.setValue(tempBojler2);
    n4.setValue(tempBojler3);
    n5.setValue(tempSolar);
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
  // Serial.begin(9600);

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
   myButton12.attachPop(ButtonRelease12, &myButton12);
   myButton13.attachPop(ButtonRelease13, &myButton13);
   myButton14.attachPop(ButtonRelease14, &myButton14);
}

void loop() {
  //INCIJALIZACIJA NEXTION LOOP-a
  nexLoop(nex_listen_list);

 if (a == 0){
  // PRVO DOBIVANJE TEMPERATURE
  sensors2.requestTemperatures();
  sensors3.requestTemperatures();
  sensors4.requestTemperatures();
  sensors5.requestTemperatures();

  // POHRANJIVANJE TEMPERATURA U VARIJABLE
  tempPovrat = sensors2.getTempCByIndex(0); // POVRAT
  tempBojler3 = sensors3.getTempCByIndex(0); // BOJLER ZONA 1
  tempBojler2 = sensors4.getTempCByIndex(0); // BOJLER ZONA 2
  tempBojler1 = sensors5.getTempCByIndex(0); // BOJLER ZONA 3  
    
  // ISPISIVANJE U SERIAL MONITOR ZA DEBUGGING
  /*
  Serial.println("Temperatura povrata: ");
  Serial.println(tempPovrat);
  Serial.println("Temperatura bojler zona 1: ");
  Serial.println(tempBojler1);
  Serial.println("Temperatura bojler zona 2: ");
  Serial.println(tempBojler2);
  Serial.println("Temperatura bojler zona 3: ");
  Serial.println(tempBojler3);
  */
    
  // ISPISIVANJE TEMPERATURA NA NEXTION
  n1.setValue(tempPovrat);
  n2.setValue(tempBojler1);
  n3.setValue(tempBojler2);
  n4.setValue(tempBojler3);
  a = 1;
 }

  unsigned long currentMillis = millis();

    if(currentMillis - previousMillis3 >= interval3) {
      previousMillis3 = currentMillis;
      // DOBIVANJE TEMPERATURI SA SENZORA
      // Serial.println("Requesting temperatures!");
      sensors2.requestTemperatures();
      sensors3.requestTemperatures();
      sensors4.requestTemperatures();
      sensors5.requestTemperatures();

      // POHRANJIVANJE TEMPERATURA U VARIJABLE
      tempPovrat = sensors2.getTempCByIndex(0);
      tempBojler3 = sensors3.getTempCByIndex(0);
      tempBojler2 = sensors4.getTempCByIndex(0);
      tempBojler1 = sensors5.getTempCByIndex(0);   
    
      // ISPISIVANJE U SERIAL MONITOR ZA DEBUGGING
      /*
      Serial.println("Temperatura povrata: ");
      Serial.println(tempPovrat);
      Serial.println("Temperatura bojler zona 1: ");
      Serial.println(tempBojler1);
      Serial.println("Temperatura bojler zona 2: ");
      Serial.println(tempBojler2);
      Serial.println("Temperatura bojler zona 3: ");
      Serial.println(tempBojler3);
      */
    
      // ISPISIVANJE TEMPERATURA NA NEXTION
      n1.setValue(tempPovrat);
      n2.setValue(tempBojler1);
      n3.setValue(tempBojler2);
      n4.setValue(tempBojler3);
    }

    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // POHRANJIVANJE STARIH TEMPERATURA U VARIJABLE
      tempPecOld = tempPec;
      tempSolarOld = tempSolar;
      // DOBIVANJE TEMPERATURI SA SENZORA
      // Serial.println("Requesting temperatures!");
      sensors1.requestTemperatures();
      sensors6.requestTemperatures();
          
      // POHRANJIVANJE TEMPERATURA U VARIJABLE
      tempPec = sensors1.getTempCByIndex(0);
      tempSolar = sensors6.getTempCByIndex(0);
      
      // ISPISIVANJE U SERIAL MONITOR ZA DEBUGGING
      /*
      Serial.println("Temperature u peci: ");
      Serial.println(tempPec);
      Serial.println("Temperatura solarnog: ");
      Serial.println(tempSolar);
      */

      // ISPISIVANJE TEMPERATURA NA NEXTION
      if(tempPec != tempPecOld) {
        n0.setValue(tempPec);
      }
      if(tempSolar != tempSolarOld) {
        n5.setValue(tempSolar);
      }
    }

    if(currentMillis - previousMillis4 >= interval4) {
      previousMillis4 = currentMillis;
      if(x == 0) {
      p1.setPic(6);
     } else {
      p1.setPic(5);
     }
     if(y == 0) {
      p2.setPic(6);
     } else {
      p2.setPic(5);
     }
   }

  // PALJENJE BUZZERA I PUMPE GRIJANJA KOD PREVISOKE TEMPERATURE
  if(tempPec >= tempDanger) {
    digitalWrite(relayPump1, LOW);
    x = 1;
    buzz = 1;
    pec = 1;
  }

  // GAŠENJE BUZZERA KOD PREVISOKE TEMPERATURE PECI
  if(pec == 1) {
      if(tempPec < tempDanger) {
      x = 0;
      pec = 0;
      buzz = 0;
    }
  }
  
  // PALJENJE BUZZERA I PUMPE SOLARNOG GRIJANJA KOD PREVISOKE TEMPERATURE
  if(tempSolar >= tempDangerSolar) {
    digitalWrite(relayPump2, LOW);
    y = 1;
    buzz = 1;
  }

  // GAŠENJE BUZZERA I PUMPE SOLARNOG GRIJANJA KOD PREVISOKE TEMPERATURE
  if(solar == 1) {
      if(tempSolar < tempDangerSolar) {
      digitalWrite(relayPump2, HIGH);
      y = 0;
      buzz = 0;
    }
  }
  
  // PALJENJE BUZZERA NA INTERVALE
  if(buzz == 1) {
    if (currentMillis - previousMillis2 >= interval2) {
      previousMillis2 = currentMillis;
      if (buzzerState == LOW) {
        buzzerState = HIGH;
      } else {
        buzzerState = LOW;
      }
      digitalWrite(buzzerPin, buzzerState);
    }
  }
  
  // IZRAČUNAVANJE RAZLIKE TEMPERATURA I PROMJENA STANJA VARIJABLE
  tempRazlika = tempSolar - tempBojler2;
  /*
  Serial.println("Razlika temperatura: ");
  Serial.println(tempRazlika);
  */

  if(tempRazlika >= tempDifHigh) {
    solar = 1;
  } else if(tempRazlika <= tempDifLow) {
    solar = 0;
  }

  // PALJENJE / GAŠENJE PUMPE GRIJANJA
  if(pump1 == 0) {
      if(tempPec >= tempHigh) {
      digitalWrite(relayPump1, LOW);
      x = 1;
    } else if(tempPec <= tempLow) {
      digitalWrite(relayPump1, HIGH);
      x = 0;
    }
  }
  
  // PALJENJE / GAŠENJE PUMPE SOLARNOG GRIJANJA
  if(pump2 == 0) {
     if(solar == 1) {
      digitalWrite(relayPump2, LOW);
      y = 1;
    } else if(solar == 0) {
      digitalWrite(relayPump2,HIGH);
      y = 0;
    }
  }

  // PALJENJE PUMPE GRIJANJA U SLUČAJU KVARA SENZORA
  if(tempPec == -127) {
    digitalWrite(relayPump1, LOW);
    x = 1;
    pump1 = 1;
    error1 = 1;
  } else if(tempPec > -127 && error1 == 1) {
    error1 = 0;
    pump1 = 0;
  }
  
  // PALJENJE PUMPE SOLARNOG GRIJANJA U SLUČAJU KVARA SENZORA
  if(tempSolar == -127) {
    digitalWrite(relayPump2, LOW);
    y = 1;
    pump2 = 1;
    error2 = 1;
  } else if(tempSolar > -127 && error2 == 1) {
    error2 = 0;
    pump2 = 0;
  }

}
