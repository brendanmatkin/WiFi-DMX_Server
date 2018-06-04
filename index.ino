// TODO: either use minified or store html, css, & js in file system. 

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
    -webkit-touch-callout:none;
    -webkit-user-select:none;
    -khtml-user-select:none;
    -moz-user-select:none;
    -ms-user-select:none;
    user-select:none;
    
  }
  .container {
    width: 85%;
    margin: 0 auto;
  }
  h3 {
    text-transform: lowercase;
    margin: 1.1em 0 0.75em;
  }

  .label {
      font-weight:normal;
      font-size: 80%;
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
  .disabled::-webkit-slider-thumb {
    cursor: default;
  }
  .disabled:hover {
    opacity: 0.3;
  }


  #col {

  }

  .fields {
    padding: 2px;
    width: 100px;
    font-size:16px;
    /*margin: 0;*/
    margin: -10px 0;
  }

  button {
    width: 30%;
    height: 30px;
    font-size:12px;
    margin: 1%;
    border:none;
    cursor: pointer;
  }
  button:disabled {
    border: 2px solid #0033cc;
    color: #0033cc;
    cursor: default;
  }
  select {
    width: 30%;
    height: 30px;
    font-size: 12px;
    border: none;
    margin: 1%;
    cursor: pointer;
  }


  /* toggle switch stuff: */
  .switch {
    position: relative;
    display: inline-block;
    width: 60px;
    height: 34px;
    top: -5px;
  }
  .switch input {display:none;}
  .slide-switch {
    position: absolute;
    cursor: pointer;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-color: #e0e0e0;
    -webkit-transition: .4s;
    transition: .4s;
    border-radius: 34px;
  }
  .slide-switch:before {
    position: absolute;
    content: "";
    height: 26px;
    width: 26px;
    left: 4px;
    bottom: 4px;
    background-color: white;
    -webkit-transition: .4s;
    transition: .4s;
    border-radius: 50%;
  }
  input:checked + .slide-switch {
    background-color: #101010;
    opacity:0.7;
  }
  input:focus + .slide-switch {
    box-shadow: 0 0 1px #101010;
    opacity:0.7;
  }
  input:checked + .slide-switch:before {
    -webkit-transform: translateX(26px);
    -ms-transform: translateX(26px);
    transform: translateX(26px);
  }

/*  button.preset {
    width:22%;
  }*/
</style>
</head>



<body>
    <div class = "container">
      <header>
        <h1>Pixel Controller</h1>
      </header>

      <h3>PRESETS: <span class="label">(HOLD TO SAVE)</span></h3>
      <!-- NOTE: change this callbacks to event listeners?? --> 
        <button id="preset1" class="preset" style="background-color:#EEE" onmousedown="checkPreset(1, event);" ontouchstart="checkPreset(1, event);" onmouseup="setPreset(1)" ontouchend="setPreset(1)">Preset 1</button>
        <button id="preset2" class="preset" style="background-color:#EEE" onmousedown="checkPreset(2, event);" ontouchstart="checkPreset(2, event);" onmouseup="setPreset(2)" ontouchend="setPreset(2)">Preset 2</button>
        <button id="preset3" class="preset" style="background-color:#EEE" onmousedown="checkPreset(3, event);" ontouchstart="checkPreset(3, event);" onmouseup="setPreset(3)" ontouchend="setPreset(3)">Preset 3</button>

      <h3>ANIMATION SPEED: <span class="label" id="label_s"></span></h3>
        <input class="enabled slider" id="s" type="range" min="0" max="255" step="1" oninput="updateValues();" value="127">
      <h3>DELAY/OFFSET: <span class="label" id="label_d"></span></h3>
        <input class="enabled slider" id="d" type="range" min="0" max="255" step="1" oninput="updateValues();" value="0">
      <h3>COLOR: <span class="label" id="label_h"></span></h3>
        <input class="enabled slider" id="h" type="range" min="0" max="255" step="1" oninput="updateValues();" value="160">
      
      <h3>OPERATING MODE:</h3>
        <button id="mode0" class="button" style="background-color:#EEE" onclick="setMode(0);">LFO</button>
        <button id="mode1" class="button" style="background-color:#EEE" onclick="setMode(1);">MANUAL</button>
        <button id="mode2" class="button" style="background-color:#EEE" onclick="setMode(2);">BLACKOUT</button>

      <h3>HUE CYCLE SPEED: <span class="label" id="label_hc"></span></h3>
        <input class="enabled slider" id="hc" type="range" min="0" max="255" step="1" oninput="updateValues();" value="0">
      <h3>BRIGHTNESS: <span class="label" id="label_b"></span></h3>
        <input class="enabled slider" id="b" type="range" min="0" max="255" step="1" oninput="updateValues();" value="255">
      <h3>WHITE: <span class="label" id="label_w"></span></h3>
        <input class="enabled slider" id="w" type="range" min="0" max="255" step="1" oninput="updateValues();" value="0">
      
      <!-- 
      <h3>SAVE AS PRESET:</h3>
        <select id="presets">
          <option value=1>1</option>
          <option value=2>2</option>
          <option value=3>3</option>
        </select>
        <button id="preset-save" class="preset" style="background-color:#EEE" onclick="savePreset();">SAVE</button> 
      -->
      <h3>NUMBER OF FIXTURES:</h3>
        <input class="fields" id="numDev" type="number" name="num" min="1" max="40" placeholder="number" oninput="updateValues();">
      
      <h3>ENABLE SEND: <span class="label" id="label_es">yes</span></h3>
      <!--<span style="margin: 0px -10px 0px 30px;">enable send: </span>-->
        <label class="switch">
          <input type="checkbox" id="sendToggle" checked>
          <span class="slide-switch"></span>
        </label>
    </div>

  <script>
  "use strict"
    // http://minifycode.com/html-minifier/ this minifier strips comments
    // http://unminify.com/ this is a good unminifier

    let state = {
        speed: 127,
        delay: 0,
        hue: 160,
        mode: 0,
        hue_cycle: 0,
        brightness: 255,
        white: 0,
        preset: 0,
        num_devices: 8
    };

    let presets = [
        Object.assign({}, state),
        Object.assign({}, state),
        Object.assign({}, state)
    ];

    // preset save timer stuff:
    let timer_id = 0,
        hold_time = 1000;

    //let mode = false;
    let mode = 0;   // 0 = LFO, 1 = MANUAL, 2 = BLACKOUT
    let sendWS = true;
    let connection; 


    if (sendWS) {
      connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
      //connection = new WebSocket('ws://127.0.0.1:8080/');
      connection.onopen = function () {
        //initializeStates();
        connection.send('Connect ' + new Date());
        // send mode: 
        //document.getElementById('mode-display').innerHTML = '(SDC)';
        //connection.send('SDC');
      };
      connection.onerror = function (error) {
        console.log('WebSocket Error ', error);
      };
      connection.onmessage = function (e) {
        // check if array or single object
        if(Array.isArray(JSON.parse(e.data))) {
          getPresetsFromWS(e.data);
        }
        else {
          getStateFromWS(e.data);
        }
        //console.log('Server: ', e.data);
      };
      connection.onclose = function () {
        console.log('WebSocket connection closed');
      };
    }



    function sendStateToWS() {
      let stateJSON = JSON.stringify(state);
      console.log(stateJSON);
      if (sendWS) connection.send(stateJSON);
    }

    function getStateFromWS(rawJSON) {
      console.log('Server State: ' + rawJSON);
      state = JSON.parse(rawJSON);
      //setMode(state.mode);          // this now happens in setPreset!
      //updateValues(false);          // this also now happens in setPreset!
      setPreset(state.preset, false);
      console.log('Updated state object. ' + state.speed);
      //let newStates = JSON.parse(rawJSON);
    }

    function getPresetsFromWS(rawJSON) {
      console.log('Server Presets: ' + rawJSON);
      presets = JSON.parse(rawJSON);
      console.log('Update preset array. ' + presets[2].preset);
    }

    // all at once instead of in batches - it's a few ms slower but much more convenient. 
    function updateValues(getValuesFromDOM = true) {
      if (getValuesFromDOM) {
        state.speed = parseInt(document.getElementById('s').value);
        state.delay = parseInt(document.getElementById('d').value);
        state.hue   = parseInt(document.getElementById('h').value);
        state.brightness = parseInt(document.getElementById('b').value);
        state.hue_cycle = parseInt(document.getElementById('hc').value);
        state.white = Math.floor(parseInt(document.getElementById('w').value)** 2 / 255);
        state.num_devices = parseInt(document.getElementById('numDev').value);

        if (state.preset != 0) setPreset(0, false); // manual changes override preset! 

        if (sendWS) sendStateToWS();
      }
      else {
        // push values to faders
        document.getElementById('s').value = state.speed;
        document.getElementById('d').value = state.delay;
        document.getElementById('h').value = state.hue;
        document.getElementById('b').value = state.brightness;
        document.getElementById('hc').value = state.hue_cycle;
        document.getElementById('w').value = state.white;
        document.getElementById('numDev').value = state.num_devices;
      }

      let __h = state.hue/255.0*360.0;
      document.getElementById('h').style.backgroundColor = `hsl(${__h}, 100%, 50%)`;
      document.getElementById('label_s').innerHTML = state.speed;
      document.getElementById('label_d').innerHTML = state.delay;
      document.getElementById('label_h').innerHTML = state.hue;
      document.getElementById('label_b').innerHTML = state.brightness;
      document.getElementById('label_hc').innerHTML = state.hue_cycle;
      document.getElementById('label_w').innerHTML = state.white;
    }


    function checkPreset(presetNum, event) {
      // if (event.type === "click" && event.button !== 0) {
      //   return;
      // }
      timer_id = setTimeout(function() { 
        savePreset(presetNum) 
      }, hold_time);
    }

    function setPreset(presetNum, sendState = true) {
      clearTimeout(timer_id);
      // preset 0 is NO PRESET
      // whenever any value is changed, set preset to 0?? 
      if (presetNum == 1) {
        document.getElementById('preset1').disabled = true;
        document.getElementById('preset2').disabled = false;
        document.getElementById('preset3').disabled = false;
      }
      else if (presetNum == 2) {
        document.getElementById('preset1').disabled = false;
        document.getElementById('preset2').disabled = true;
        document.getElementById('preset3').disabled = false;
      }
      else if (presetNum == 3) {
        document.getElementById('preset1').disabled = false;
        document.getElementById('preset2').disabled = false;
        document.getElementById('preset3').disabled = true;
      }
      else {
        document.getElementById('preset1').disabled = false;
        document.getElementById('preset2').disabled = false;
        document.getElementById('preset3').disabled = false;
        state.preset = 0;
      }
      if (presetNum > 0) state = Object.assign({}, presets[presetNum-1]);
      updateValues(false);
      setMode(state.mode, false);
      //state.preset = presetNum;
      console.log ('loaded preset: ' + state.preset);

      //if (sendState) sendStateToWS();     // this happens in setMode!
    }

    function savePreset(presetNum) {
      //state.preset = parseInt(document.getElementById('presets').value);
      state.preset = presetNum;

      let txt = 'Save current values as PRESET ' + state.preset + ' ?';
      if (confirm(txt)) {
          presets[state.preset-1] = Object.assign({}, state);   // update the new preset here! 
          console.log('saved as preset #' + state.preset);
          setPreset(state.preset);                              // then set/update everything!
          if (sendWS) connection.send(JSON.stringify(presets)); // and send new preset array back to server
      } 
      else {
          console.log('preset save cancelled');
      }
    }
    
    function setMode(modeNum, cancelPreset = true) {
      // SDC/LFO
      if (modeNum == 0) {
        document.getElementById('s').className = 'slider enabled';
        document.getElementById('d').className = 'slider enabled';
        document.getElementById('h').className = 'slider enabled';
        document.getElementById('w').className = 'slider enabled';
        document.getElementById('b').className = 'slider disabled';
        document.getElementById('hc').className = 'slider enabled';
        document.getElementById('s').disabled = false;
        document.getElementById('d').disabled = false;
        document.getElementById('h').disabled = false;
        document.getElementById('w').disabled = false;
        document.getElementById('b').disabled = true;
        document.getElementById('hc').disabled = false;

        document.getElementById('mode0').disabled = true;
        document.getElementById('mode1').disabled = false;
        document.getElementById('mode2').disabled = false;
      }
      // MANUAL
      else if (modeNum == 1) {
        document.getElementById('s').className = 'slider disabled';
        document.getElementById('d').className = 'slider disabled';
        document.getElementById('h').className = 'slider enabled';
        document.getElementById('w').className = 'slider enabled';
        document.getElementById('b').className = 'slider enabled';
        document.getElementById('hc').className = 'slider enabled';
        document.getElementById('s').disabled = true;
        document.getElementById('d').disabled = true;
        document.getElementById('h').disabled = false;
        document.getElementById('w').disabled = false;
        document.getElementById('b').disabled = false;
        document.getElementById('hc').disabled = false;

        document.getElementById('mode0').disabled = false;
        document.getElementById('mode1').disabled = true;
        document.getElementById('mode2').disabled = false;
      }
      // BLACKOUT
      else if (modeNum == 2) {
        document.getElementById('s').className = 'slider disabled';
        document.getElementById('d').className = 'slider disabled';
        document.getElementById('h').className = 'slider disabled';
        document.getElementById('w').className = 'slider disabled';
        document.getElementById('b').className = 'slider disabled';
        document.getElementById('hc').className = 'slider disabled';
        document.getElementById('s').disabled = true;
        document.getElementById('d').disabled = true;
        document.getElementById('h').disabled = true;
        document.getElementById('w').disabled = true;
        document.getElementById('b').disabled = true;
        document.getElementById('hc').disabled = true;

        document.getElementById('mode0').disabled = false;
        document.getElementById('mode1').disabled = false;
        document.getElementById('mode2').disabled = true;
      }
      else {
        console.log('BAD MODE: ' + modeNum + '. Leaving unchanged (' + state.mode + ')');
        state.mode = 0;     // default animated on error
        return null;
      }
      state.mode = modeNum;
      if (state.preset != 0 && cancelPreset) setPreset(0, false); // manual changes override preset! 
      if (sendWS) connection.send(JSON.stringify(state));
      console.log('mode: ' + state.mode);
    }

    /* EVENT LISTENERS */
    //disable right click for preset buttons (needed for mobile browser)
    let nodes = document.getElementsByClassName("preset");
    for (let node of nodes) {
      node.addEventListener("contextmenu", function (event) {
        event.preventDefault();
        //event.stopPropagation();
        //return false;
      });
    }

    let _toggler = document.getElementById("sendToggle");
    _toggler.addEventListener('click', function() {
      setSendWS(_toggler.checked);
    });


    function setSendWS(setSend) {
      console.log("setSendWS: " + setSend);
      if (setSend) {
        sendWS = true;
        sendStateToWS();
        document.getElementById('label_es').innerHTML = "yes";
      }
      else {
        sendWS = false;
        document.getElementById('label_es').innerHTML = "no";
      }
    }

    // restyle color picker:
    // let color_picker = document.getElementById("col");
    // let color_picker_wrapper = document.getElementById("color-picker-wrapper");
    // color_picker.onchange = function() {
    //   color_picker_wrapper.style.backgroundColor = color_picker.value;    
    // }
    // color_picker_wrapper.style.backgroundColor = color_picker.value;
    //document.getElementById('w').disabled = true;
  </script>
</body>
</html>
)rawliteral";
