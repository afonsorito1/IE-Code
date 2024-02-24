#include "BluetoothSerial.h"
#include "esp_random.h"
#include "Wire.h"
#include "Protocentral_FDC1004.h"

#define UPPER_BOUND  0X8800               // max readout capacitance
#define LOWER_BOUND  (-1 * UPPER_BOUND)
#define CHANNEL 0                          // channel to be read
#define MEASURMENT 0                       // measurment channel

int capdac = 0;
char result[100];

uint32_t sum = 0;
uint16_t i = 0;

FDC1004 FDC;

BluetoothSerial SerialBT;
uint16_t randomNumber = 0;
uint8_t flag=0;

void setup() {
  Wire.begin();        //i2c begin
  Serial.begin(115200); // serial baud rate
  SerialBT.begin("ESP32 Gostosinha Afonso Rito"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

 void loop() {
/*
  if (Serial.available()) {
    // randomNumber = esp_random() % 10 + 1; // Generate a random number between 1 and 10
    if(flag==0)
    randomNumber +=1;
    else
    randomNumber -=1;

    
    if(randomNumber==0)
      flag=0;

    if(randomNumber==1023)
      flag=1;  

    SerialBT.println(randomNumber); // Send the random number via Bluetooth
    Serial.write(randomNumber);
    delay(10); // Wait X ms seconds
    printf("Random value is: %d\n",randomNumber);
  }
  
  delay(20);
  */

  FDC.configureMeasurementSingle(MEASURMENT, CHANNEL, capdac);
  FDC.triggerSingleMeasurement(MEASURMENT, FDC1004_400HZ);

  //wait for completion
  delay(15);
  uint16_t value[2];

  if (! FDC.readMeasurement(MEASURMENT, value))
  {
    int16_t msb = (int16_t) value[0];
    int16_t lsb = (int16_t) value[1];
    int32_t capacitance = ((int32_t)457) * ((int32_t)msb); //in attofarads

    capacitance /= 1000;   //in femtofarads
    capacitance += ((int32_t)3028) * ((int32_t)capdac);

    if (msb > UPPER_BOUND)               // adjust capdac accordingly
	  {
      if (capdac < FDC1004_CAPDAC_MAX)
	      capdac++;
      }
	  else if (msb < LOWER_BOUND)
	  {
      if (capdac > 0)
	    capdac--;
    }

    sum += capacitance;
    i++;
    if(i==50){
      Serial.print((((float)(sum/50)/1000))*2,4);
      Serial.print("  pf\n");
      sum = 0;
      i = 0;
    }

  }
}