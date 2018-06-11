// call one of these two functions on an incoming osc message:
// message.route("/1", exampleRouteCallback);                 // allows partial address match
// message.dispatch("/address/1", exampleDispatchCallback);   // requires exact address match


// you generally can ignore 'addressOffset'
// but you HAVE TO HAVE IT as a parameter for the callback function for "ROUTE"
// (it indicates how many chars in the address before it matched)
void exampleRouteCallback(OSCMessage &msg, int addressOffset) {
  if (msg.isFloat(0)) {
    float _inFloat = msg.getFloat(0);
    Serial.printf("[OSC] got float in example route: %.3f\n", _inFloat);
  }
  else {
    Serial.printf("[OSC] (route) argument 0 not a float\n");
  }
}


void exampleDispatchCallback(OSCMessage &msg) {
  if (msg.isFloat(0)) {
    float _inFloat = msg.getFloat(0);
    Serial.printf("[OSC] got float in example dispatch: %.3f\n", _inFloat);
  }
  else {
    Serial.printf("[OSC] (dispatch) argument 0 not a float\n");
  }
}

