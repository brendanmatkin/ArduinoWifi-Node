#ifndef INDEX_H
#define INDEX_H

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0">
<title>ESP Server</title>
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
    width: 80%;
    margin: 0 auto;
  }
  h3 {
    text-transform: capitalize;
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
      opacity: 1.0;
      border-radius: 3px; 
      -webkit-transition: .2s;
      transition: opacity .2s;
  }
  /*.slider:hover {
      opacity: 1.0;
  }*/
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
  .disabled::-webkit-slider-thumb {
    cursor: default;
  }
  .disabled {
    opacity: 0.3;
  }
  .disabled:hover {
    opacity: 0.3;
  }

</style>
</head>

<body>
  <div class="container">
    <header> 
      <h1>ESP Server UI</h1>
    </header>
  
    <h3>hue: </h3>
      <input class="slider" id ="hue" type="range" min="0" max="255" step="1" oninput="sendHue();" value="0">
    <br>
    <br>
    <br>
    <h3>red: </h3>
      <input class="slider" id ="r" type="range" min="0" max="255" step="1" oninput="sendHexColor();" value="0">
    <h3>green: </h3>
      <input class="slider" id ="g" type="range" min="0" max="255" step="1" oninput="sendHexColor();" value="0">
    <h3>blue: </h3>
      <input class="slider" id ="b" type="range" min="0" max="255" step="1" oninput="sendHexColor();" value="0">
  </div>
  <script>
  // note, we are using "let" instead of "var" as per ES6
  "use strict"

  /* webSocket setup: */
  let connection;
  //connection = new WebSocket( 'ws://' + location.hostname + ':81/', ['arduino'] );
  connection = new WebSocket( 'ws://127.0.0.1:8080/', ['arduino'] );
  // send this when connecting to a server:
  connection.onopen = function() {
    connection.send( 'Hello, I am a client. ' + new Date() );
  }
  connection.onerror = function( error ) {
    console.log( '[WS] error: ', error );
  }
  connection.onmessage = function( e ) {
    console.log( '[WS] Message from Server: ' + e.data );
  }
  connection.onclose = function() {
    console.log( '[WS] Connection Closed.' );
  }

  function sendHue() {
    let hueFader = document.getElementById('hue');
    let hue = hueFader.value;

    let _hue = hue/255.0*360.0;
    hueFader.style.backgroundColor = `hsl(${_hue}, 100%, 50%)`;

    let hueString = 'H' + hue.toString(10);   // base 10, i.e. decimal
    console.log('Send hue: ' + hueString);
    connection.send(hueString);
  }

  function sendHexColor() {
    var r = document.getElementById('r').value** 2 / 255;
    var g = document.getElementById('g').value** 2 / 255;
    var b = document.getElementById('b').value** 2 / 255;

    var rgb = r << 16 | g << 8 | b;
    var rgbstr = '#' + rgb.toString(16);    // base 16 i.e. hex
    console.log('RGB: ' + rgbstr);
    connection.send(rgbstr);
  }
  

  </script>
</body>
</html>
)rawliteral";


#endif /*INDEX_H*/
