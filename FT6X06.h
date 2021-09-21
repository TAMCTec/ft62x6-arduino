
#ifndef FT6X06_H
#define FT6X06_H

#include "Arduino.h"
#include <Wire.h>
#include "pin.h"

#define ADDR 0x38

#define I2C_SCL 5
#define I2C_SDA 4

#define REG_DEV_MODE 0x00
#define REG_GEST_ID 0x01
#define REG_TD_STATUS 0x02
#define REG_P1_XH 0x03
#define REG_P1_XL 0x04
#define REG_P1_YH 0x05
#define REG_P1_YL 0x06
#define REG_P1_WEIGHT 0x07
#define REG_P1_MISC 0x08
#define REG_P2_XH 0x09
#define REG_P2_XL 0x0A
#define REG_P2_YH 0x0B
#define REG_P2_YL 0x0C
#define REG_P2_WEIGHT 0x0D
#define REG_P2_MISC 0x0E
#define REG_TH_GROUP 0x80

#define WORKING_MODE 0x00
#define MOVE_UP 0x10
#define MOVE_RIGHT 0x14
#define MOVE_DOWN 0x18
#define MOVE_LEFT 0x1C
#define ZOOM_IN 0x48
#define ZOOM_OUT 0x49
#define NO_GESTURE 0x00
#define IDLE 0xFF

#define DEFAULT_THRESHOLD 128

class TP_Point {
  public:
    TP_Point(void);
    TP_Point(long x, long y, int weight, int area);
    TP_Point(byte *data, int rot);

    bool operator==(TP_Point);
    bool operator!=(TP_Point);

    long x;
    long y;
    int weight;
    int area;
};

class FT6X06 {
  public:
    FT6X06(void);
    FT6X06(int _sda, int _scl);
    void begin(int thresh = DEFAULT_THRESHOLD);
    void setRotation(int rot);
    byte getGesture(void);
    void read(void);
    int touches = 0;
    bool isTouched = false;
    byte gesture = NO_GESTURE;
    TP_Point points[2];

  private:
    void writeByte(byte reg, byte val);
    byte readByte(byte reg);
    int rotation = 0;
    int sda;
    int scl;
};

#endif // FT6X06_H
