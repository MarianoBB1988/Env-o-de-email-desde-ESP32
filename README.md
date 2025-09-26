# Envío de Correos con ESP32 y Google Apps Script

## Descripción

Este proyecto permite enviar correos electrónicos desde un ESP32 utilizando un **Google Apps Script** como Web App.  
El ESP32 lee por el **Serial Monitor** el destinatario, el asunto y el cuerpo del correo, y envía la información a través del script de Google.

---

## Requisitos

- ESP32
- Conexión WiFi
- Cuenta de Gmail
- Google Apps Script

---

## Configuración del Apps Script

1. Crear un nuevo proyecto en [Google Apps Script](https://script.google.com/).
2. Copiar el siguiente código en el script:

## Configuración del Apps Script

1. Crear un nuevo proyecto en [Google Apps Script](https://script.google.com/).
2. Copiar el siguiente código en el script:

```javascript
function doGet(e) {
  var destinatario = e.parameter.to;
  var asunto = e.parameter.subject;
  var cuerpo = e.parameter.body;

  if (destinatario && asunto && cuerpo) {
    GmailApp.sendEmail(destinatario, asunto, cuerpo);
    return ContentService.createTextOutput("Correo enviado a " + destinatario);
  } else {
    return ContentService.createTextOutput("Faltan parámetros. Uso: ?to=destinatario&subject=asunto&body=cuerpo");
  }
}

#include <WiFi.h>
#include <HTTPClient.h>

const char* WIFI_SSID = "RedWIFI";
const char* WIFI_PASS = "password";
const String url = "TU_URL_DE_APPS_SCRIPT_AQUI";

String inputLine = "";
String destinatario = "";
String asunto = "";
String cuerpo = "";
int paso = 0;

String urlEncode(const String &str) {
  String encoded = "";
  char c, code0, code1;
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

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.println("Ingresá el destinatario del correo:");
}

void loop() {
  if (Serial.available()) {
    inputLine = Serial.readStringUntil('\n');
    inputLine.trim();

    if (paso == 0) {
      destinatario = inputLine; paso = 1; Serial.println("Ingresá el asunto del correo:");
    } else if (paso == 1) {
      asunto = inputLine; paso = 2; Serial.println("Ingresá el cuerpo del correo:");
    } else if (paso == 2) {
      cuerpo = inputLine; paso = 0;
      enviarCorreo(destinatario, asunto, cuerpo);
      Serial.println("\nIngresá el destinatario del correo:");
    }
  }
}

void enviarCorreo(String to, String sub, String msg) {
  String requestUrl = url + "?to=" + urlEncode(to) + "&subject=" + urlEncode(sub) + "&body=" + urlEncode(msg);
  HTTPClient http;
  http.begin(requestUrl);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.println("Correo enviado correctamente (HTTP code " + String(httpCode) + ")");
  } else {
    Serial.println("Error en la solicitud HTTP");
  }
  http.end();
}

