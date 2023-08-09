#include <sbus.h>
#include <Arduino.h>
#include <RecurringTask.h>


// used pins
#define SBUS_PIN 3   // D3


SBUS sbus;


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

  sbus.begin(SBUS_PIN, sbusNonBlocking);  


    packet.header1 = 0x20;
    packet.header2 = 0x40;

    for (int i = 0; i < 14; i++)
    {
        packet.channels[i] = 0x5dc;
    }

    packet.checksum = calc_checksum(packet);
}



void loop() {

/*  for (int i=1; i <= 18; ++i) {
    Serial.print(sbus.getChannel(i)); 
    Serial.print(" ");
  }
  
  if (sbus.signalLossActive())
    Serial.print("SIGNAL_LOSS ");
    
  if (sbus.failsafeActive())
    Serial.print("FAILSAFE");
    
  Serial.println();
    
  delay(200);

*/
   RecurringTask::interval(20, []() {
        for (int i = 0; i < 14; i++)
        {
            packet.channels[0] = (sbus.getChannel(1));
            packet.channels[1] = (sbus.getChannel(2));
            packet.channels[2] = (sbus.getChannel(3));
            packet.channels[3] = (sbus.getChannel(4));
            packet.channels[4] = (sbus.getChannel(5));
            packet.channels[5] = (sbus.getChannel(6));
            packet.channels[6] = (sbus.getChannel(7));
            packet.channels[7] = (sbus.getChannel(8));
            packet.channels[8] = (sbus.getChannel(9));
            packet.channels[9] = (sbus.getChannel(10));
            packet.channels[10] = (sbus.getChannel(11));
            packet.channels[11] = (sbus.getChannel(12));
            packet.channels[12] = (sbus.getChannel(13));
            packet.channels[13] = (sbus.getChannel(14));
            packet.channels[14] = (sbus.getChannel(15));
            packet.channels[15] = (sbus.getChannel(16));

        }
        packet.checksum = calc_checksum(packet);
    });

    send_packet(packet);

    delay(10);

}
