# Pré-requis
 - Librairie **VirtualWire** sur l'IDE Arduino (utiliser le gestionnaire de librairie embarqué).
 - python-serial (**apt install python-serial**)

# Fichiers
- dht.ino ⇒ code à mettre sur l'arduino qui possède le sensor
- recepteur.ino ⇒ code à mettre sur l'arduino qui reçoit les données
- sensors.list ⇒ liste des devices dans domoticz
- send_to_domoticz.py ⇒ passerelle entre l'arduino et domoticz
- rules.d/99-arduino.rules ⇒ règle udev pour fixer le nom du device recepteur

## Configuration
- Dans le fichier dht.ino
> // Brancher l'émetteur sur le PIN 12 // Brancher le DHT sur le PIN 2 -
> Si un autre pin, configurer DHTPIN
> 
> #define SENSOR_NUMBER 1000            // serial du sensor
> #define DHTTYPE DHT11                 // type de sensor - DHT11/DHT21/DHT22
> #define DHTPIN 2                      // pin sur l'arduino
> #define MESURE_DELAY 2000             // en ms - délai entre les mesures
> #define NMESURE 3                     // nombre de mesure pour constituer la moyenne
> #define SEND_FELTTEMP 1               // 0: désactivé | 1:activé - Envoi de la T° ressentie

- Dans le fichier recepteur.ino (aucune modif)
> // Brancher le récepteur sur le PIN 11

- Udev
Récupérer le serial de la carte arduino branchée au raspberry:
> root@raspberrypi:~/domotic# lsusb -v |  grep -A 1 Arduino | grep
 iSerial   iSerial               220 64932343638351813201

Modifier le fichier rules.d/99-arduino.rules:
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{iSerial}=="64932343638351813201", SYMLINK+="arduino1"

Placer le fichier 99-arduino.rules dans /etc/udev/rules.d

- Dans le fichier send_to_domoticz.py
> domoticz_proto = "https"
domoticz_host = "domos.zerros.lan"
sensors_list = "sensors.list"
TTY="arduino1"

- Dans le fichier sensors.list (ATTENTION ce fichier doit être dans le même répertoire que send_to_domoticz.py).
> \# id_sensors;idx_temp;idx_hum
> 1000;126;127

## Lancement du programme

Exécuter le fichier send_to_domoticz.py:
> nohup python send_to_domoticz.py > /dev/null 2>&1
