#include <VirtualWire.h> // On inclus la librairie VirtualWire

// Brancher le récepteur sur le PIN 11

typedef struct {
  int serial;
  float temp;
  float hum;
  float felt;
} MySensor;

void setup()
{
    Serial.begin(115200); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("Début communication");
 
    vw_setup(2000); // initialisation de la librairie VirtualWire à 2000 bauds
    vw_rx_start();  // Activation de la partie réception de la librairie VirtualWire
}
 
void loop()
{
  MySensor message;
  byte taille_message = sizeof(MySensor);
  
  vw_wait_rx();
  
  if (vw_get_message((byte *) &message, &taille_message)) {
    // On copie le message, qu'il soit corrompu ou non

    Serial.print("{");
    Serial.print("\"sensor\": "); // Affiche le message
    Serial.print(message.serial);
    Serial.print(",");
    Serial.print("\"temp\": ");
    Serial.print(message.temp);
    Serial.print(",");
    Serial.print("\"hum\": ");
    Serial.print(message.hum);
    Serial.print(",");
    Serial.print("\"felt\": ");
    Serial.print(message.felt);
    Serial.print("}");
    Serial.println("");
    
  }
}
