/*silakan copy paste guna project NON KOMERSIAL anda
namun jangan lupa sertakan sumber sebagai bentuk apresiasi penulis 
ditulis oleh hendrik Politeknik Negeri ATK Yogyakarta pada tahun 2021*/

// Library yang digunakan
#define BLYNK_PRINT Serial
#include <LiquidCrystal_I2C.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Blynk.h>>
#include <UniversalTelegramBot.h>

#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>
#define BOT_TOKEN "1874729604:AAEH-BSmqte10n704okLykWwbCSuh2XrqNy"

int relayPin = 30; //relay on/off pemanas berdasar suhu
int mainRelayPin = 40; // relay catu daya ke pemanas

LiquidCrystal_I2C lcd(0x27,16,2);

//===================== Pengaturan koneksi ke wifi ===============================
char auth[] = "lrrS5lK5qWz0wPQsZj5ukYzPEsj41uMF"; //masukan token disini
char ssid[] = "propeople"; //masukan nama wifi disini 
char pass[] = "123456777"; //pasword wifimu
#define EspSerial Serial3
#define ESP8266_BAUD 115200  //serial untuk komunikasi berapa
ESP8266 wifi(&EspSerial);

//============== peletakan sensor suhu di pin 24 arduino mega ===============

#define ONE_WIRE_BUS 24 //pin sensor suhu di digital 
OneWire oneWire(ONE_WIRE_BUS);  

//============= menghubungkan ke library =================
DallasTemperature sensors(&oneWire);
float celcius; // tipe data yang digunakan float

void setup(void)
{
  lcd.init();
  lcd.backlight();
  pinMode(relayPin,OUTPUT);
  pinMode(mainRelayPin,OUTPUT); 
  digitalWrite(mainRelayPin, HIGH);
  delay(10);
  sensors.begin();  // perintah memulai sensor
  Serial.begin(115200); //menggunakan 2 serial karena board yang digunakan arduino mega + include esp 8622
  Serial3.begin(115200);
  delay(10);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  // Blynk.begin(auth, wifi, ssid, pass);                          //Reguler server
  Blynk.begin(auth, wifi, ssid, pass,"blynk-cloud.com", 8080);    //Local server

}

void loop(void)
{ 
  Blynk.run(); //perintah menjalankan Blynk
  sensors.requestTemperatures();  //mengirim perintah get temperatures
//print suhu dalam satuan Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  //Serial.print((char)176);//shows degrees character
  Serial.print(celcius); 
  Serial.println("Celcius  ");
  
  lcd.setCursor(0,0);
  lcd.print(sensors.getTempCByIndex(0));
  lcd.write(0xDF);
  lcd.print("C");
  
//===================opsi jika ingin ditampilkan dalam fahrenheit======================
  //print suhu dalam satuan Fahrenheit (jika ingin di print pada F)
  //Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  //Serial.print((char)176);//shows degrees character
 // Serial.println("F");
  delay(10); 

//==============perintah mengirim ke aplikasi blynk================
  
  Blynk.virtualWrite(V1, sensors.getTempCByIndex(0));

// ============ setting on/off relay ===========
  
  if(sensors.getTempCByIndex(0) >= 75){
    digitalWrite (relayPin, HIGH); //relay mati saat suhu mencapai target
    Serial.println("RELAY OFF!");
    lcd.setCursor(0,1);
    lcd.print("RELAY OFF!  ");
  }

  if(sensors.getTempCByIndex(0) <= 72){
    digitalWrite (relayPin, LOW);  //relay menyala saat suhu dibawah target
    Serial.println("RELAY ON!");
    lcd.setCursor(0,1);
    lcd.print("RELAY ON!      ");    
  }
 
//---------suhu air dijaga 72-75 derajad celcius-----------
}
