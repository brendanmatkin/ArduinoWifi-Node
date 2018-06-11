const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 8080 });
//const wss = new WebSocket.Server({ port: 81 });


let state = {
        speed: 0,
        delay: 0,
        hue: 160,
        mode: 0,
        hue_cycle: 100,
        brightness: 255,
        white: 0,
        preset: 1,
        num_devices: 8
    };

let presets = [
        Object.assign({}, state),
        Object.assign({}, state),
        Object.assign({}, state)
    ];

presets[0].preset = 1;
presets[1].preset = 2;
presets[2].preset = 3;
presets[0].mode = 0;
presets[1].mode = 1;
presets[2].mode = 2;
presets[0].hue = 160;
presets[1].hue = 100;
presets[2].hue = 200;


wss.on('connection', function connection(ws) {
  ws.on('message', function incoming(message) {
    console.log('received: %s', message);
  });

  ws.send('something');
  //ws.send(JSON.stringify(presets));
  //ws.send(JSON.stringify(state));
});
