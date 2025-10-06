// Shows how to use SPISlave on a single device in asynchronous mode
// Core0 runs as an SPI master and initiates a transmission to the slave
// Core1 runs the SPI Slave mode and provides a unique reply to messages from the master
//
// Released to the public domain 2024 by Earle F. Philhower, III <earlephilhower@yahoo.com>

#include <SPI.h>
#include <SPISlave.h>
#include <I2S.h>

// Create the I2S port using a PIO state machine
I2S i2s(OUTPUT);

// GPIO pin numbers
#define pBCLK 20
#define pWS (pBCLK+1)
#define pDOUT 22
const int sampleRate = 24000; 


// Wiring:
// Master RX  GP0 <-> GP11  Slave TX
// Master CS  GP1 <-> GP9   Slave CS
// Master CK  GP2 <-> GP10  Slave CK
// Master TX  GP3 <-> GP8   Slave RX

SPISettings spisettings(124000/*1000000*/, MSBFIRST, SPI_MODE1);

// Core 0 will be SPI master
void setup1() {
  // SPI.setRX(0);
  // SPI.setCS(1);
  // SPI.setSCK(2);
  // SPI.setTX(3);
  // SPI.begin(true);

  // delay(5000);
}

// int transmits = 0;
void loop1() {
  // char msg[42];
  // int loops = 0;
  // memset(msg, 0, sizeof(msg));
  // sprintf(msg, "What's up? This is transmission %d", transmits);
  // Serial.printf("\n\nM-SEND: '%s'\n", msg);
  // SPI.beginTransaction(spisettings);
  // SPI.transferAsync(msg, msg, sizeof(msg));
  // while (!SPI.finishedAsync()) {
  //   loops++;
  // }
  // SPI.endTransaction();
  // Serial.printf("M-RECV: '%s', idle loops %d\n", msg, loops);
  // transmits++;
  // delay(5000);
}

// Core 1 will be SPI slave

#define CBUF_SIZE 100
volatile int head=0, tail=0;
int16_t cbuf[CBUF_SIZE][2];

// volatile bool recvBuffReady = false;
uint8_t recvBuff[4];
int recvIdx = 0;
void recvCallback(uint8_t *data, size_t len) {

    digitalWrite(LED_BUILTIN, 1);
    digitalWrite(LED_BUILTIN, 0);  

  // memcpy(recvBuff + recvIdx, data, len);
  // recvIdx += len;
  // if (recvIdx == sizeof(recvBuff)) {
  //   recvBuffReady = true;
  //   recvIdx = 0;
  // }

  // Serial.print(len);
  // Serial.print(" ");
  // Serial.print(*data);

  if (len==4)
    recvIdx = 0;

  int16_t tp[] = {0,1,0,-1};

  for (int i=0; i<len; i++)
  {
    recvBuff[recvIdx] = data[i];

    recvIdx++;
    if (recvIdx==4)
    {
      recvIdx = 0;


      int16_t si = (((uint16_t)recvBuff[0])<<8) + recvBuff[1];
      int16_t sq = (((uint16_t)recvBuff[2])<<8) + recvBuff[3];

      cbuf[head][0] = si;
      cbuf[head][1] = sq;
      
      static uint16_t phase=0;

      int16_t ssb = si*tp[phase&3] - sq*tp[(phase+1)&3];
      phase++;

      i2s.write(ssb);
      i2s.write(ssb);

      // i2s.write(cbuf[head][1]);
      // i2s.write(cbuf[head][1]);

      // i2s.write(int16_t(cbuf[head][0]-cbuf[head][1]));
      // i2s.write(int16_t(cbuf[head][0]-cbuf[head][1]));

      head++;
      if (head==CBUF_SIZE)
        head = 0;

    // memcpy(recvBuff, data, 4);

       }
  }


  // if (len==9)
  {




    // Serial.print(data[0]);
    // Serial.print(' ');
    // Serial.print(data[1]);
    // Serial.print(' ');
    // Serial.print(data[2]);
    // Serial.print(' ');
    // Serial.print(data[3]);
    // Serial.println();
  }



  // for (int i=0; i<len; i++)
  // {
  //   digitalWrite(LED_BUILTIN, 1);
  //   digitalWrite(LED_BUILTIN, 0);
 
  // }

  // Serial.println(" bytes received");

  // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

}

int sendcbs = 0;
// Note that the buffer needs to be long lived, the SPISlave doesn't copy it.  So no local stack variables, only globals or heap(malloc/new) allocations.
char sendBuff[4];
void sentCallback() {
  memset(sendBuff, 0, sizeof(sendBuff));
  sprintf(sendBuff, "Slave to Master Xmission %d", sendcbs++);
  SPISlave1.setData((uint8_t*)sendBuff, sizeof(sendBuff));
}

// Note that we use SPISlave1 here **not** because we're running on
// Core 1, but because SPI0 is being used already.  You can use
// SPISlave or SPISlave1 on any core.
void setup() {
  SPISlave1.setRX(8);
  SPISlave1.setCS(9);
  SPISlave1.setSCK(10);
  SPISlave1.setTX(11);
  // Ensure we start with something to send...
  sentCallback();
  // Hook our callbacks into the slave
  SPISlave1.onDataRecv(recvCallback);
  SPISlave1.onDataSent(sentCallback);
  SPISlave1.begin(spisettings);
  delay(3000);
  Serial.println("S-INFO: SPISlave started");

  pinMode(LED_BUILTIN, OUTPUT);



    // gpio_init(18);
    // gpio_set_dir(18, GPIO_OUT);


// zzz:



// gpio_put(18, 0);
// // gpio_put(18, 0);
// // gpio_put(18, 0);
// // gpio_put(18, 0);
// // gpio_put(18, 0);

// // delay(1);
// gpio_put(18, 1);
// // gpio_put(18, 1);
// // gpio_put(18, 1);
// // gpio_put(18, 1);
// // gpio_put(18, 1);

// // delay(1);

//     goto zzz;


  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(sampleRate)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }

}

void loop() {
  // if (recvBuffReady) {
  //   Serial.printf("S-RECV: '%s'\n", recvBuff);
  //   recvBuffReady = false;
  // }


//  delay(1000);

    // Serial.print(recvBuff[0]);
    // Serial.print(' ');
    // Serial.print(recvBuff[1]);
    // Serial.print(' ');
    // Serial.print(recvBuff[2]);
    // Serial.print(' ');
    // Serial.print(recvBuff[3]);

    // int16_t s1 = (((uint16_t)recvBuff[0])<<8) + ((uint16_t)recvBuff[1]);
    // int16_t s2 = (((uint16_t)recvBuff[2])<<8) + ((uint16_t)recvBuff[3]);

    // Serial.print(s1);
    // Serial.print(' ');
    // Serial.print(s2);

    // Serial.println();


    //     uint16_t sample = *((uint16_t*)data);
    // i2s.write(sample);
    // i2s.write(sample);

    if (head != tail)
    {

      // i2s.write(cbuf[tail][0]);
      // i2s.write(cbuf[tail][0]);

      tail++;
      if (tail==CBUF_SIZE)
        tail = 0;
    }
}
