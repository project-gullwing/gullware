#include <Arduino.h>
#include <dobson.h>

Dobson* pDobson = NULL;


void setup() {
    Serial.begin(115200);
    Serial.println("Start");
    pDobson = new Dobson();
}

void loop() {
    pDobson->move();
}
