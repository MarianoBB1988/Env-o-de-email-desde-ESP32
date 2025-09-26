#include <WiFi.h>
#include <HTTPClient.h>

// --- WiFi ---
const char* WIFI_SSID = "RedWIFI";
const char* WIFI_PASS = "passwordWIFI";

// --- Apps Script URL ---
const String url = "URL de implementación en google";

// --- Variables para Serial ---
String inputLine = "";
String destinatario = "";
String asunto = "";
String cuerpo = "";
int paso = 0; // 0=destinatario, 1=asunto, 2=cuerpo

// --- Función para codificar parámetros en URL ---
String urlEncode(const String &str) {
  String encoded = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      encoded += '%';
      code0 = (c >> 4) & 0xF;
      code1 = c & 0xF;
      encoded += (code0 < 10 ? char('0' + code0) : char('A' + code0 - 10));
      encoded += (code1 < 10 ? char('0' + code1) : char('A' + code1 - 10));
    }
  }
  return encoded;
}

// --- Setup WiFi ---
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Conectando a WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.println("Ingresá el destinatario del correo:");
}

// --- Loop principal para leer Serial ---
void loop() {
  if (Serial.available()) {
    inputLine = Serial.readStringUntil('\n');
    inputLine.trim();

    if (paso == 0) {
      destinatario = inputLine;
      paso = 1;
      Serial.println("Ingresá el asunto del correo:");
    } 
    else if (paso == 1) {
      asunto = inputLine;
      paso = 2;
      Serial.println("Ingresá el cuerpo del correo:");
    } 
    else if (paso == 2) {
      cuerpo = inputLine;
      paso = 0;

      // Enviar correo
      enviarCorreo(destinatario, asunto, cuerpo);

      // Prepararse para siguiente correo
      Serial.println("\nIngresá el destinatario del correo:");
    }
  }
}

// --- Función para enviar correo vía Apps Script ---
void enviarCorreo(String to, String sub, String msg) {
  String requestUrl = url 
      + "?to=" + urlEncode(to)
      + "&subject=" + urlEncode(sub)
      + "&body=" + urlEncode(msg);

  HTTPClient http;
  http.begin(requestUrl);
  int httpCode = http.GET();
  if (httpCode > 0) {
    // En lugar de imprimir toda la página HTML, solo mostramos mensaje claro
    Serial.println("Correo enviado correctamente (HTTP code " + String(httpCode) + ")");
  } else {
    Serial.println("Error en la solicitud HTTP");
  }
  http.end();
}
