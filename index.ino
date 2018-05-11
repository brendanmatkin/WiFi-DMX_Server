static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0">
<title>ESP8266 WebSocket Test</title>
<style>
  body { 
    background-color: #FFFFFF;
    font-family: Arial, Helvetica, Sans-Serif;
    color: #020202;
  }
  .container {
    width: 75%;
    margin: 0 auto;
  }

  /* slider css: */
  .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 15px;
      /*margin: 15px 0;*/
      background: #e0e0e0;
      outline: none;
      opacity: 0.7;
      border-radius: 3px; 
      -webkit-transition: .2s;
      transition: opacity .2s;
  }
  .slider:hover {
      opacity: 1;
  }
  .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 32px;
      height: 32px;
      border-radius: 50%; 
      background: #101010;
      cursor: pointer;
  }
  .slider::-moz-range-thumb {
      width: 30px;
      height: 30px;
      border-radius: 50%; 
      background: #101010;
      cursor: pointer;
  }
  .disabled {
    opacity: 0.3;
  }
  .disabled:hover {
    opacity: 0.3;
  }

  #col {

  }

  .fields {
    padding: 1px;
    width: 100px;
    font-size:16px;
    margin: 0;
  }

  .button {
    width: 100px;
    height: 30px;
    font-size:18px;
  }

  /* color picker css: */
  input[type="color"] {
  /*.color-pick {*/
    -webkit-appearance: none;
    border: none;
    width: 50px;
    height: 50px;
    cursor: pointer;
    display: block;
    margin:10px 0px;
  }
  input[type="color"]::-webkit-color-swatch-wrapper {
  /*.color-pick {*/
    padding: 0;
  }
  input[type="color"]::-webkit-color-swatch {
  /*.color-pick {*/
    border: none;
  }
</style>


</head>
<body>
    <div class = "container">
      <header>
        <h1>LED Control</h1>
      </header>
      <h3>SPEED:</h3>
      <input class="enabled slider" id="s" type="range" min="0" max="255" step="1" oninput="sendSDC();" value="0">
      <h3>DELAY:</h3>
      <input class="enabled slider" id="d" type="range" min="0" max="255" step="1" oninput="sendSDC();" value="0">
      <h3>COLOR:</h3>
      <input class="enabled slider" id="c" type="range" min="0" max="255" step="1" oninput="sendSDC();" value="0">
      <h3>CHANGE MODE:</h3>
      <button id="mode" class="button" style="background-color:#EEE" onclick="changeMode();">MODE</button>
      <span id='mode-display'>--</span>
      <h3>COLOR PICKER:</h3>
      <div id="color-picker-wrapper">
        <input class = "color-pick" id="col" type="color" value="#ff0000" oninput="sendRGB();">
      </div>
      <!--<h3>HUE:</h3>
      <input class="enabled slider" id="h" type="range" min="0" max="255" step="1" oninput="sendHue();" value="0">-->
      <h3>WHITE:</h3>
      <input class="enabled slider" id="w" type="range" min="0" max="255" step="1" oninput="sendW();" value="0">
      <h3>NUM DEVICES:</h3>
      <input class="fields" id="numDev" type="number" name="num" min="1" max="40" placeholder="number" oninput="submitDevices();">
      <!--<input class="enabled slider" id="numDev" type="range" min="0" max="50" step="1" oninput="submitDevices();" value="1">
      <p id="devices">--</p>-->
      <!--<button onclick="submitDevices()">SUBMIT</button>-->
    </div>

  <script>
    // http://minifycode.com/html-minifier/ this minifier strips comments
    // http://unminify.com/ this is a good unminifier
    //var mode = false;
    var mode = 0;
    var sendWS = true;
    if (sendWS) {
      var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
      //var connection = new WebSocket('ws://127.0.0.1:8080/');
      connection.onopen = function () {
        connection.send('Connect ' + new Date());
        // send mode: 
        document.getElementById('mode-display').innerHTML = '(SDC)';
        connection.send('SDC');
      };
      connection.onerror = function (error) {
        console.log('WebSocket Error ', error);
      };
      connection.onmessage = function (e) {
        console.log('Server: ', e.data);
      };
      connection.onclose = function () {
        console.log('WebSocket connection closed');
      };
    }
    
    // send speed, delay, color
    function sendSDC() {
      var s = document.getElementById('s').value;
      var d = document.getElementById('d').value;
      var c = document.getElementById('c').value;
      var sdc = s<<16 | d<<8 | c;
      var sdcstr = '&' + sdc.toString(16);
      console.log('SDC: ' + sdcstr);
      if (sendWS) connection.send(sdcstr);
    };

    function sendRGB () {
      // var r = document.getElementById('r').value** 2 / 255;
      // var g = document.getElementById('g').value** 2 / 255;
      // var b = document.getElementById('b').value** 2 / 255;
    
      //var rgb = r << 24 | g << 16 | b << 8 | w;
      //var rgbstr = '#' + rgb.toString(16);
      //console.log('RGB: ' + rgbstr);
      //connection.send(rgbstr);
      var col = document.getElementById('col').value;   // using color picker instead
      console.log('RGB: ' + col);
      if (sendWS) connection.send(col);
    };

    function sendHue() {
      var h = document.getElementById('h').value** 2 / 255;
      var h16 = 0<<8 | h;
      var hstr = 'H' + h16.toString(16);
      console.log('Hue: ' + hstr);
      if (sendWS) connection.send(hstr);
    };

    function sendW() {
      var w = document.getElementById('w').value** 2 / 255;
      var w16 = 0 << 8 | w;
      var wstr = 'W' + w16.toString(16);  // base 16 color var.
      console.log('white: ' + wstr);
      if (sendWS) connection.send(wstr);
    };

    function submitDevices() {
      var numDevices = document.getElementById('numDev').value;
      var n16 = 0<<8 | numDevices;
      var nstr = 'N' + n16.toString(16);
      console.log('NumDevices: ' + nstr);
      if (sendWS) connection.send(nstr);
      //document.getElementById('devices').innerHTML = numDevices;
    };
    
    function changeMode() {
      mode++;
      mode = mode%3;
      
      if (mode == 0) {
        document.getElementById('s').className = 'slider enabled';
        document.getElementById('d').className = 'slider enabled';
        document.getElementById('c').className = 'slider enabled';
        document.getElementById('w').className = 'slider enabled';
        document.getElementById('s').disabled = false;
        document.getElementById('d').disabled = false;
        document.getElementById('c').disabled = false;
        document.getElementById('w').disabled = false;

        document.getElementById('mode-display').innerHTML = '(SDC)';
        if (sendWS) connection.send('SDC');
      } 
      else if (mode == 1) {
        document.getElementById('s').className = 'slider disabled';
        document.getElementById('d').className = 'slider disabled';
        document.getElementById('c').className = 'slider enabled';
        document.getElementById('w').className = 'slider enabled';
        document.getElementById('s').disabled = true;
        document.getElementById('d').disabled = true;
        document.getElementById('c').disabled = false;
        document.getElementById('w').disabled = false;

        document.getElementById('mode-display').innerHTML = '(MANUAL)';
        if (sendWS) connection.send('MANUAL');   
      } 
      else {
        document.getElementById('s').className = 'slider disabled';
        document.getElementById('d').className = 'slider disabled';
        document.getElementById('c').className = 'slider disabled';
        document.getElementById('w').className = 'slider disabled';
        document.getElementById('s').disabled = true;
        document.getElementById('d').disabled = true;
        document.getElementById('c').disabled = true;
        document.getElementById('w').disabled = true;

        document.getElementById('mode-display').innerHTML = '(BLACKOUT)';
        if (sendWS) connection.send('AUTO');
      }
      console.log('mode = ' + mode)
    };

    // restyle color picker:
    var color_picker = document.getElementById("col");
    var color_picker_wrapper = document.getElementById("color-picker-wrapper");
    color_picker.onchange = function() {
      color_picker_wrapper.style.backgroundColor = color_picker.value;    
    }
    color_picker_wrapper.style.backgroundColor = color_picker.value;
    //document.getElementById('w').disabled = true;
  </script>
</body>
</html>
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
