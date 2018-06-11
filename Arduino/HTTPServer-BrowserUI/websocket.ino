

/*---------- Websocket Event ------------*/
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_ERROR:
      if (SERIAL_DEBUG) Serial.printf("[ws] ERROR!\n");
      break;
    case WStype_DISCONNECTED:
      if (SERIAL_DEBUG) Serial.printf("[ws] [%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
      // num is client ID (because I am a websocket server!)
      IPAddress ip = webSocket.remoteIP(num);
      if (SERIAL_DEBUG) Serial.printf("[ws] [%u] Connected from url: %u.%u.%u.%u%s replying...\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
      }
      break;
    case WStype_TEXT: {
        if (SERIAL_DEBUG) Serial.printf("[ws] [%u] got text: %s\n", num, payload);
        
      }
      break;
    case WStype_BIN:
      if (SERIAL_DEBUG) Serial.printf("[ws] got binary (length): %zu\n", length);
      hexdump(payload, length);
      break;
    default:
      break;
  }
}


