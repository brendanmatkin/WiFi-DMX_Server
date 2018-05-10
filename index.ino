static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html><html><head><meta name = "viewport" content = "width = device-width, initial-scale = 1.0"><title>DMX Server</title><style>body{background-color:#FFF;font-family:Arial,Helvetica,Sans-Serif;color:#020202}.container{width:75%;margin:0 auto}.slider{-webkit-appearance:none;width:100%;height:15px;background:#e0e0e0;outline:none;opacity:0.7;border-radius:3px;-webkit-transition: .2s;transition:opacity .2s}.slider:hover{opacity:1}.slider::-webkit-slider-thumb{-webkit-appearance:none;appearance:none;width:32px;height:32px;border-radius:50%;background:#101010;cursor:pointer}.slider::-moz-range-thumb{width:30px;height:30px;border-radius:50%;background:#101010;cursor:pointer}#col{}.fields{padding:1px;width:100px;font-size:16px;margin:0}input[type="color"]{-webkit-appearance:none;border:none;width:50px;height:50px;cursor:pointer;display:block;margin:10px 0px}input[type="color"]::-webkit-color-swatch-wrapper{padding:0}input[type="color"]::-webkit-color-swatch{border:none}</style></head><body><div class = "container"> <header><h1>LED Control</h1> </header><h3>SPEED:</h3> <input class="enabled slider" id="r" type="range" min="0" max="255" step="1" oninput="sendRGB();" value="0"><h3>DELAY:</h3> <input class="enabled slider" id="g" type="range" min="0" max="255" step="1" oninput="sendRGB();" value="0"><h3>COLOR:</h3> <input class="enabled slider" id="b" type="range" min="0" max="255" step="1" oninput="sendRGB();" value="0"><h3>COLOR PICKER:</h3> <input class = "color-pick" id="col" type="color" value="#ff0000" oninput="sendRGB();"><h3>WHITE:</h3> <input class="enabled slider" id="w" type="range" min="0" max="255" step="1" oninput="sendW();" value="0"><h3>NUM DEVICES:</h3> <input class="fields" id="numDev" type="number" name="num" min="1" max="40" placeholder="devices" oninput="submitDevices();"><p style="margin:8px 0px"> <button id="rainbow" class="button" style="background-color:#EEE" onclick="rainbowEffect();">Rainbow</button></p></div> <script>var rainbowEnable=false;var connection=new WebSocket('ws://'+location.hostname+':81/',['arduino']);connection.onopen=function(){connection.send('Connect '+new Date());};connection.onerror=function(error){console.log('WebSocket Error ',error);};connection.onmessage=function(e){console.log('Server: ',e.data);};connection.onclose=function(){console.log('WebSocket connection closed');};function sendSDC(){} function sendRGB(){var col=document.getElementById('col').value;console.log('RGB: '+col);connection.send(col);} function sendW(){var w=document.getElementById('w').value**2/255;var w16=0<<8|w;var wstr='White: '+w16.toString(16);console.log('white: '+wstr);connection.send(wstr);} function submitDevices(){var numDevices=document.getElementById('numDev').value;console.log('Number of Devices: '+numDevices);} function rainbowEffect(){}</script> </body></html>
)rawliteral";

/*
<script>
var websock;
function start() {
  websock = new WebSocket('ws://' + window.location.hostname + ':81/');
  websock.onopen = function(evt) { console.log('websock open'); };
  websock.onclose = function(evt) { console.log('websock close'); };
  websock.onerror = function(evt) { console.log(evt); };
  websock.onmessage = function(evt) {
    console.log(evt);
    var e = document.getElementById('ledstatus');
    if (evt.data === 'ledon') {
      e.style.color = 'red';
    }
    else if (evt.data === 'ledoff') {
      e.style.color = 'black';
    }
    else {
      console.log('unknown event');
    }
  };
}
function buttonclick(e) {
  websock.send(e.id);
}
</script>

<body onload="javascript:start();">
<h1>ESP8266 WebSocket Demo</h1>
<div id="ledstatus"><b>LED</b></div>
<button id="ledon"  type="button" onclick="buttonclick(this);">On</button> 
<button id="ledoff" type="button" onclick="buttonclick(this);">Off</button>
 */
