//
//  AMAD SALMON
//  25/12/2019
//
//  Maison Connectée
//  Programme pour créér une interface web

#include <ESP8266WiFi.h>


const char WiFiAPPSK[] = "polytech"; // Mot de passe du WiFi

// Pins
const int LED_PIN = 5; // LED integrée
const int ANALOG_PIN = A0; 
const int DIGITAL_PIN = 12; 

WiFiServer server(80);

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
}

void loop() 
{
  // On vérifie qu'une connexion a eu lieu
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Lire la première ligne de la requête
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  
  int val = -1; 
                
  if (req.indexOf("/led/0") != -1)
    val = 0; // LED à LOW
  else if (req.indexOf("/led/1") != -1)
    val = 1; // LED à HIGH
  else if (req.indexOf("/read") != -1)
    val = -2; // Imprimer la lecture des pins
 

  // connfiguration du GPIO5 par rapport à la requête
  if (val >= 0)
    digitalWrite(LED_PIN, val);

  client.flush();

  // RÉPONSE -------------------------------
  String s = "HTTP/1.1 200 OK\r\n"; // requete HTTP OK
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
 
  if (val >= 0)
  { // Affichage de l'état de la LED
    s += "La LED est ";
    s += (val)?"ON":"OFF";
  }
  else if (val == -2)
  { // Pour afficher la lecture des pins
    s += "Analog Pin = ";
    s += String(analogRead(ANALOG_PIN));
    s += "<br>"; 
    s += "Digital Pin 12 = ";
    s += String(digitalRead(DIGITAL_PIN));
  }
  else
  {
    s += "Requête invalide.<br>  /led/1, /led/0, ou /read.";
  }
  s += "</html>\n";

  // Envoie de la réponse au client
  client.print(s);
  delay(1);
  Serial.println("Client déconnecté");

}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // ajout des 2 derniers caractères de l'adresse MAC 
  // pour avoir un nom de réseau unique à chaque fois
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "AmadSalmon " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void initHardware()
{
  Serial.begin(115200);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}
