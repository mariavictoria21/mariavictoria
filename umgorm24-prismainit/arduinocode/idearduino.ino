#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define DHTPIN 4     // Pin GPIO donde está conectado el pin de datos del DHT11
#define DHTTYPE DHT11 // Tipo de sensor DHT que estás utilizando (DHT11 o DHT22)

const char* ssid = "TIGO11A8D18"; //estoy en la red del trabajo cambiar.
const char* password = "EF1A8D18"; //estoy en la red del trabajo cambiar.
const char* serverUrl = "http://192.168.100.22:3001/datos"; // Cambia 'tu_servidor' por la dirección IPV4 o dominio de tu servidor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al WiFi...espere unos segundos");
  }

  Serial.println("Conectado al WiFi");
  dht.begin();
}

void loop() {
  // Lectura del sensor
  float temperatura = dht.readTemperature(); // Leer temperatura en grados Celsius
  float humedad = dht.readHumidity();        // Leer humedad relativa en porcentaje

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error al leer el sensor DHT11");
    return;
  }

  if (Serial.available() > 0) {
    char receivedChar = Serial.read(); // Leer el caracter recibido por el puerto serial
    
    if (receivedChar == 'f' || receivedChar == 'F') {
      // Crear un objeto JSON para enviar al servidor
      DynamicJsonDocument doc(200);
      doc["nombrecompleto"] = "maria victoria";
      doc["correouniversidad"] = "malvarador14@miumg.edu.gt";
      doc["carne"] = "0902-21-4354";
      doc["Latitud"] = "10.12345";
      doc["Longitud"] = "-84.12345";
      doc["Humedad"] = humedad;
      doc["Temperatura"] = temperatura;

      // Serializar el JSON a una cadena
      String jsonString;
      serializeJson(doc, jsonString);

      // Realizar la solicitud HTTP POST, en caso de error verificar el ipv4
      HTTPClient http;
      http.begin(serverUrl);                  // Especificar la URL del endpoint,
      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(jsonString);

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        String response = http.getString();
        Serial.println(response);
      } else {
        Serial.print("Error en la solicitud HTTP: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
  }

  // Esperar un corto tiempo antes de verificar de nuevo la tecla presionada
  delay(100);
}