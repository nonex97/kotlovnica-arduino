#include <DallasTemperature.h>
#include <OneWire.h>

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
int relayPump1 = 23; // PUMPA GRIJANJA
int relayPump2 = 25; // PUMPA SOLARNOG GRIJANJA
// int relayPump3 = 27; // PUMPA SANITARNE VODE (PRIPREMA)

// DEFINIRANJE PINA ZA BUZZER
int buzzerPin = 50;

// DEFINIRANJE VARIJABLI GRANIČNIH TEMPERATURA
int tempHigh = 70; // GRANICA PALJENJA PUMPE GRIJANJA
int tempLow = 65; // GRANICA GAŠENJA PUMPE GRIJANJA
int tempDif = 7; // GRANICA PALJENJA PUMPE SOLARNOG GRIJANJA

// DEFINIRANJE VARIJABLE ZA PALJENJE / GAŠENJE PUMPE SOLARNOG GRIJANJA
int solar = 0;

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
}

void loop() {
  // DOBIVANJE TEMPERATURI SA SENZORA
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

  // IZRAČUNAVANJE RAZLIKE TEMPERATURA I PROMJENA STANJA VARIJABLE
  tempRazlika = tempSolar - tempBojler2; // PROVJERI S NIKOLOM

  if(tempRazlika >= tempDif) {
    solar = 1;
  } else if(tempRazlika <= 0) {
    solar = 0;
  }

  // PALJENJE / GAŠENJE PUMPE GRIJANJA
  if(tempPec >= tempHigh) {
    digitalWrite(relayPump1, LOW); // PROVJERITI DAL JE HIGH ILI LOW
  } else if(tempPec <= tempLow) {
    digitalWrite(relayPump1, HIGH);
  }

  // PALJENJE / GAŠENJE PUMPE SOLARNOG GRIJANJA
  if(solar==1) {
    digitalWrite(relayPump2, LOW); // PROVJERITI DAL JE HIGH ILI LOW
  } else if(solar==0) {
    digitalWrite(relayPump2,HIGH);
  }

}