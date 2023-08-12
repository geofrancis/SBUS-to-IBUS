#include <sbus.h>
#include <Arduino.h>


// used pins
#define SBUS_PIN 3   // D3
int fs;

SBUS sbus;
const int ledPin =  LED_BUILTIN;
int ledState = LOW;


typedef struct
{
    uint8_t header1;
    uint8_t header2;
    uint16_t channels[14];
    uint16_t checksum;
} IBUS_Packet;
IBUS_Packet packet = {};


uint16_t calc_checksum(IBUS_Packet &p)
{
    uint16_t checksum = 0xFFFF;
    checksum -= p.header1;
    checksum -= p.header2;

    for (int i = 0; i < 14; i++)
    {
        checksum -= p.channels[i] & 0xFF;
        checksum -= (p.channels[i] >> 8) & 0xFF;
    }

    return checksum;
}

void send_packet(IBUS_Packet &p)
{
    Serial.write((uint8_t *)&p, sizeof(IBUS_Packet));
}


void setup() {
  Serial.begin(115200);

  sbus.begin(SBUS_PIN, sbusBlocking);  
 pinMode(ledPin, OUTPUT);

    packet.header1 = 0x20;
    packet.header2 = 0x40;

    for (int i = 0; i < 14; i++)
    {
        packet.channels[i] = 0x5dc;
    }

    packet.checksum = calc_checksum(packet);
        Serial.print("Start ");
    
}



void loop() {

  if (!sbus.waitFrame()) {
    
            Serial.println("Timeout!");
            
            packet.channels[0] = (1500);
            packet.channels[1] = (1500);
  } else {
    
            packet.channels[0] = (sbus.getChannel(1));
            packet.channels[1] = (sbus.getChannel(2));
          }
        packet.checksum = calc_checksum(packet);        
        send_packet(packet);
        
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);


}
