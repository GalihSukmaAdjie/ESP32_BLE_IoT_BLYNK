#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#include <DHTesp.h>
#include <Blynk.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Cc95LmkKzQVEx2JI0L4DW3nPpLDXsYIl";

WidgetTerminal terminal (V0);
BLYNK_WRITE(V0)
{
  terminal.clear();
  terminal.println("BLYNK Version " BLYNK_VERSION " : SELAMAT DATANG");
  terminal.println("-------------------------");
  if (String("hai") == param.asStr())
  {
    terminal.println("Hallo, saya benjamin engel, sebutkan nama anda.");
  }
  else if (String("tidak") == param.asStr())
  {
    terminal.println("Oke, Terimakasih");
  }
  else if (String("iya") == param.asStr())
  {
    terminal.println("Berikut yang dapat saya bantu :");
    terminal.println("(tulis dengan sesuai)");
    terminal.println("server");
    terminal.println("wifi");
  }
  else if (String("wifi") == param.asStr())
  {
    terminal.println("modul ini sedang menggunakan bluetooth");
  }
  else if (String("server") == param.asStr())
  {
    terminal.println("server Blynk");
    terminal.println(auth);
  }
  else {
    terminal.print("Hallo ");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.print(", ada yang bisa saya bentu (ya/tidak)");
    terminal.println();
  }
  terminal.flush();
}

BlynkTimer timer;

DHTesp dht;
#define dhtPin 13
#define dhtType DHT22

#define redPin 16
#define greenPin 4
#define bluePin 2

#define lampuPin 12

int red, green, blue;
int mred, mgreen, mblue;

BLYNK_WRITE(V1)
{
  red = param.asInt();
}
BLYNK_WRITE(V2)
{
  green = param.asInt();
}
BLYNK_WRITE(V3)
{
  blue = param.asInt();
}
int freq = 20000;
int redChannel = 0;
int greenChannel = 1;
int blueChannel = 2;
int resolution = 10;

void setup()
{
  // Debug console
  Serial.begin(9600);

  Serial.println("Waiting for connections...");

  Blynk.setDeviceName("Blynk");

  Blynk.begin(auth);

  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);

  ledcAttachPin(redPin, redChannel);
  ledcAttachPin(greenPin, greenChannel);
  ledcAttachPin(bluePin, blueChannel);

  dht.setup(dhtPin,DHTesp::dhtType);
  timer.setInterval(1000L, sendSensor);

  pinMode(lampuPin, OUTPUT);
}

void sendSensor()
{
  float suhu = dht.getTemperature();
  float kelembaban = dht.getHumidity();

  if(isnan(suhu) || isnan (kelembaban))
  {
    Serial.println("gagal membaca sensor");
    return;
  }
  Blynk.virtualWrite(V4, suhu);
  Blynk.virtualWrite(V5, kelembaban);
}

void tulisLed()
{
  mred = map(red,0,100,0,1024);
  mgreen = map(green,0,100,0,1024);
  mblue = map(blue,0,100,0,1024);

  ledcWrite(redChannel, mred);
  ledcWrite(greenChannel, mgreen);
  ledcWrite(blueChannel, mblue);
}

void loop()
{
  Blynk.run();
  tulisLed();
  timer.run();
}
