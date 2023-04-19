ESP8266WebServer server(80);

void handleRoot() {
  /* Dynamically generate the LED toggle link, based on its current state (on or off)*/
  char ledText[80];
  
  if (ledState) {
    strcpy(ledText, "LED is on. <a href=\"/?led=0\">Turn it OFF!</a>");
  }

  else {
    strcpy(ledText, "LED is OFF. <a href=\"/?led=1\">Turn it ON!</a>");
  }
 
  ledState = digitalRead(ledPin);

  char html[1000];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  int brightness = analogRead(A0);
  brightness = (int)(brightness + 5) / 10; //converting the 0-1024 value to a (approximately) percentage value

  // Build an HTML page to display on the web-server root address
  snprintf ( html, 1000,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='10'/>\
    <title>ESP8266 WiFi Network</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: 1.5em; Color: #000000; }\
      h1 { Color: #AA0000; }\
    </style>\
  </head>\
  <body>\
    <h1>ESP8266 Wi-Fi Access Point and Web Server Demo</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Brightness: %d%</p>\
    <p>%s<p>\
    <p>This page refreshes every 10 seconds. Click <a href=\"javascript:window.location.reload();\">here</a> to refresh the page now.</p>\
  </body>\
</html>",

    hr, min % 60, sec % 60,
    brightness,
    ledText
  );
  server.send ( 200, "text/html", html );
  digitalWrite ( LED_BUILTIN, 1 );
}

void handleNotFound() {
  digitalWrite ( LED_BUILTIN, 0 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( LED_BUILTIN, 1 ); //turn the built in LED on pin DO of NodeMCU off
}