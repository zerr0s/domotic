#include <VirtualWire.h>
#include <DHT.h>

// Brancher l'émetteur sur le PIN 12
// Brancher le DHT sur le PIN 2 - Si un autre pin, configurer DHTPIN

#define SENSOR_NUMBER 1000            // serial du sensor
#define DHTTYPE DHT11                 // type de sensor - DHT11/DHT21/DHT22
#define DHTPIN 2                      // pin sur l'arduino
#define MESURE_DELAY 2000             // en ms - délai entre les mesures
#define NMESURE 3                     // nombre de mesure pour constituer la moyenne
#define SEND_FELTTEMP 1               // 0: désactivé | 1:activé - Envoi de la T° ressentie

enum Types { TEMP, HUM, FELT };
DHT dht(DHTPIN, DHTTYPE);

typedef struct {
  int serial;
  float temp;
  float hum;
  float felt;
} MySensor;

void setup() {
  Serial.begin(115200);
  vw_setup(2000);
  vw_rx_start();
  dht.begin();
}

void loop() {
  delay(MESURE_DELAY);

  // récupère les data
  float humidity = getData(HUM);
  float temperature = getData(TEMP);
  float feltTemp = getData(FELT);
  
  // en cas d'erreur
  if (isnan(humidity) || isnan(temperature) || isnan(feltTemp)) {
    Serial.println("Capteur HS ?");
    return;
  }

  String result = "{";
  result += "\"Temperature\": ";
  result += temperature;
  result += ", ";
  result += "\"Humidity\": ";
  result += humidity;
  if (SEND_FELTTEMP == 1) {
    result += ", ";
    float felt = dht.computeHeatIndex(feltTemp, humidity);
    result += "\"Felt\": ";
    result += dht.convertFtoC(felt);
  }
  result += "}";
  Serial.println(result);

  MySensor message;
  message.serial = SENSOR_NUMBER;
  message.temp = temperature;
  message.hum = humidity;
  if (SEND_FELTTEMP == 1) {
    float felt = dht.computeHeatIndex(feltTemp, humidity);
    message.felt = dht.convertFtoC(felt);
  }

  Serial.print("Envoie...");
  vw_send((byte *) &message, sizeof(message));
  vw_wait_tx();
  Serial.println("Done !");
}

float getData(Types type) {
  float average = 0;
  float readTemp = 0;
  float readFeltTemp = 0;
  float readHum = 0;
  switch (type) {
    case TEMP:
      for (int i=1; i <= NMESURE; i++) {
        readTemp += dht.readTemperature();
        delay(250);
      }
      average = readTemp / NMESURE;
      break;
    case FELT:
      for (int i=1; i <= NMESURE; i++) {
        readFeltTemp += dht.readTemperature(true);
        delay(250);
      }
      average = readFeltTemp / NMESURE;
      break;
    case HUM:
      for (int i=1; i <= NMESURE; i++) {
        readHum += dht.readHumidity();
        delay(250);
      }
      average = readHum / NMESURE;
      break;
  }
  return average;
}
