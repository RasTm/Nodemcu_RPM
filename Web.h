#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<html>
  <head>
    <title>ESP8266 WiFi Network</title>
    <style>
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: 1.5em; Color: #000000; }
      h1 { Color: #AA0000; }
      #colorPreview { width: 100px; height: 100px; }
      #tgt { width: 100vw; }
      .crange { border-collapse: collapse; }
      .crange-cell { padding: 0; }
    </style>
    <script>
      function createPicker(el, elFinal, h, s, l) {
        function changeH(h, inputs, ranges) {
          inputs[0].value = h;
          colorRangeS(h, ranges);
          colorRangeL(h, inputs[1].value, ranges);
          colorFinal(inputs);
        }
        function changeS(s, inputs, ranges) {
          inputs[1].value = s;
          colorRangeL(inputs[0].value, s, ranges);
          colorFinal(inputs);
        }
        function changeL(l, inputs, ranges) {
          inputs[2].value = l;
          colorFinal(inputs);
        }
        function colorRangeH(ranges) {
          _tds = ranges[0].querySelectorAll("td");
          for (let h=0; h<360; h++) {
            _tds[h].style.backgroundColor = `hsl(${h},100%,50%)`;
          }
        }
        function colorRangeS(h, ranges) {
          _tds = ranges[1].querySelectorAll("td");
          for (let s=0; s<100; s++) {
            _tds[s].style.backgroundColor = `hsl(${h},${s}%,50%)`;
          }
        }
        function colorRangeL(h, s, ranges) {
          _tds = ranges[2].querySelectorAll("td");
          for (let l=0; l<100; l++) {
            _tds[l].style.backgroundColor = `hsl(${h},${s}%,${l}%)`;
          }
        }
        function colorFinal(inputs) {
          elFinal.style.backgroundColor = `hsl(${inputs[0].value},${inputs[1].value}%,${inputs[2].value}%)`;
        }
        var labels = [];
        var inputs = [];
        var ranges = [];
        var containers = [];
        for (let i=0; i<3; i++) {
          labels[i] = document.createElement("SPAN");
          labels[i].innerText = ["H:","S:","L:"][i];
          inputs[i] = document.createElement("INPUT");
          inputs[i].name = ["h","s","l"][i];
          inputs[i].className = "crange-input";
          inputs[i].type = "number";
          inputs[i].min = 0;
          inputs[i].max = [360,100,100][i];
          inputs[i].value = [h,s,l][i];
          ranges[i] = document.createElement("TABLE");
          ranges[i].className = "crange";
          ranges[i].style.width = "100%";
          ranges[i].style.maxWidth = "100%";
          ranges[i].innerHTML = "<tbody><tr></tr></tbody>";
          inputs[i].onchange = [
            function(e){changeH(e.target.value, inputs, ranges)},
            function(e){changeS(e.target.value, inputs, ranges)},
            function(e){changeL(e.target.value, inputs, ranges)}
          ][i];
          let _tr = ranges[i].querySelector("tr");
          for (let j=0; j<[360,100,100][i]; j++) {
            let _td = document.createElement("TD");
            _td.className = "crange-cell";
            _td.onclick = function(){[changeH,changeS,changeL][i](j, inputs, ranges)};
            _tr.appendChild(_td);
          }
          containers[i] = document.createElement("DIV");
          containers[i].style.display = "flex";
          containers[i].style.width = "100%";
          containers[i].style.maxWidth = "100%";
          containers[i].appendChild(labels[i]);
          containers[i].appendChild(inputs[i]);
          containers[i].appendChild(ranges[i]);
          el.appendChild(containers[i]);
        }
        colorRangeH(ranges);
        colorRangeS(h, ranges);
        colorRangeL(h, s, ranges);
        colorFinal(inputs);
      }
    </script>
  </head>
  <body onload="createPicker(document.body.querySelector('#tgt'), document.body.querySelector('#colorPreview'), %HUE%, %SATURATION%, %LIGHTNESS%)">
    <iframe name="sub" id="sub" style="display:none;"></iframe>
    <p>Uptime: %TIME%</p>
    <div id="colorPreview"></div>
    <form action="/apply" target="sub">
      <div id="tgt"></div>
      <input type="submit" value="Apply">
    </form>
  </body>
</html>
)rawliteral";

static String processRoot(const String& var) {
	if (var == "TIME") {
		int sec = millis() / 1000;
		int min = sec / 60;
		int hr = min / 60;
		char t[10];

		snprintf(t, 10, "%02d:%02d:%02d", hr, min % 60, sec % 60);
		return String(t);
	} else if (var == "HUE") {
		return String(hue);
	} else if (var == "SATURATION") {
		return String(saturation);
	} else if (var == "LIGHTNESS") {
		return String(lightness);
	}
	return String("");
}

void handleRoot(AsyncWebServerRequest *req) {
  req->send_P ( 200, "text/html", index_html, processRoot );
}

void handleApply(AsyncWebServerRequest *req) {
  if (req->hasParam("h")) {
    AsyncWebParameter *h = req->getParam("h");
    hue = h->value().toInt();
    if (hue < 0) hue = 0;
    if (hue > 360) hue = 360;
    hue = map(hue, 0, 360, 0, 255);
  }
  if (req->hasParam("s")) {
    AsyncWebParameter *s = req->getParam("s");
    saturation = s->value().toInt();
    if (saturation < 0) saturation = 0;
    if (saturation > 100) saturation = 100;
  }
  if (req->hasParam("l")) {
    AsyncWebParameter *l = req->getParam("l");
    lightness = l->value().toInt();
    if (lightness < 0) lightness= 0;
    if (lightness > 100) lightness = 100;
  }
  req->send ( 200, "text/plain", "200" );
}

void handleNotFound(AsyncWebServerRequest *req) {
  String message = "File Not Found\n\n";
  message += "URL: ";
  message += req->url();
  message += "\nMethod: ";
  message += ( req->method() == HTTP_GET ) ? "GET" : "POST";
  message += "\n";

  req->send ( 404, "text/plain", message.c_str() );
}
