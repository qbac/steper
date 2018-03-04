#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Bounce2.h>
#define BUTTON_PIN 2
Bounce debouncer = Bounce();
// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)

//Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, D/C, CS, RST);
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
int step = 0;
int stepAvg = 0;
int savg = 0;
int timeAvg = 0;

int ts = 0;
int tm = 0;
int th = 0;
String tsS = "00";
String tmS = "00";
String thS = "00";
unsigned long time;
unsigned long lastTime;
int lastValue = HIGH;
void setup() {
display.begin();                          //uruchom ekran
display.setContrast(56);                  //ustaw kontrast
display.clearDisplay();                   //wyczyść bufor ekranu
// Konfiguracja pinu przycisku i aktywowanie rezystora podciągającego:
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  // Po konfiguracji przycisku, ustawienie działania funkcji Bounce :
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5); // interwał w ms
}

void loop() {
  czas();
  debouncer.update();
  // Odczyt zaktualizowanej wartości (value) :
  int value = debouncer.read();
  // W zależności od stanu włączenie lub wyłączenie diody LED :
  if ( value == LOW && lastValue == HIGH) {
    step ++;
    savg ++;
    display.clearDisplay();
  }
  display.setTextSize(1);                   //ustaw rozmiar czcionki na 1
  display.setTextColor(BLACK);              //ustaw kolor tekstu na czarny
  display.setCursor(0,0);                   //ustaw kursor w pozycji 3 kolumna 5 wiersz
  display.println("Ilosc:   "+String(step));                  //ustaw rozmiar czcionki na 1
  display.println("Sr/min : "+String(stepAvg));
  //display.println(String(th)+":"+String(tm)+":"+String(ts));
  display.println(thS+":"+tmS+":"+tsS);
   // Aktualizacja funkcji Bounce : 
  lastValue = value;
  display.display();
}

void czas(void){
  time = millis();
  
  if (time>=lastTime+1000) {
    if (ts<10){tsS = "0"+String(ts);} else {tsS = String(ts);}
    if (th<10){thS = "0"+String(th);} else {thS = String(th);}
    if (tm<10){tmS = "0"+String(tm);} else {tmS = String(tm);}
    ts++;
    lastTime = time;
    display.clearDisplay();
    timeAvg++;   
    }
    
    if ( tm == 60)    // pętla -wejście gdy spełniony zostaje warunek
      {        
         tm = 0;      // przypisz wartość 0
         th ++;      // zwiększ godziny   
      }
   if (th > 23)   // wywołanie instrukcji warunkowej -wejście gdy 
   {        // spełniony zostaje warunek
      th = 0;     // przypisz wartość 0
     }
   if (ts == 60)     // analogicznie jak wyżej   
    {
    ts = 0;
    tm ++;
    }

    if (timeAvg == 10){
      stepAvg = savg * 6;
      timeAvg = 0;
      savg = 0;
      }
}
