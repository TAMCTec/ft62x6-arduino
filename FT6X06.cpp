#include "Arduino.h"
#include <FT6X06.h>
#include <Wire.h>

FT6X06::FT6X06() {
  sda = I2C_SDA;
  scl = I2C_SCL;
}
FT6X06::FT6X06(int _sda, int _scl): sda(_sda), scl(_scl) {}

void FT6X06::begin(int thresh) {
  Wire.begin(sda, scl);
  writeByte(REG_TH_GROUP, thresh);
}
void FT6X06::setRotation(int rot) {
  rotation = rot;
}
void FT6X06::read(void) {
  byte data[16];

  Wire.beginTransmission(ADDR);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(ADDR, 16);
  for (int i = 0; i < 16; i++)
    data[i] = Wire.read();

  gesture = data[REG_GEST_ID];
  if (gesture != IDLE) {
    touches = data[REG_TD_STATUS];
    isTouched = touches != 0;

    if (touches >= 1){
      byte d[6];
      for (int i=0; i<6; i++){
        d[i] = data[REG_P1_XH + i];
      }
      points[0] = TP_Point(d, rotation);
    }
    if (touches >= 2){
      byte d[6];
      for (int i=0; i<6; i++){
        d[i] = data[REG_P2_XH + i];
      }
      points[1] = TP_Point(d, rotation);
    }
  }
}

byte FT6X06::readByte(byte reg) {
  byte x;
  Wire.beginTransmission(ADDR);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(ADDR, 1);
  x = Wire.read();

  return x;
}

void FT6X06::writeByte(byte reg, byte val) {
  // use i2c
  Wire.beginTransmission(ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

TP_Point::TP_Point(void) {
  x = y = weight = area = 0;
}
TP_Point::TP_Point(long _x, long _y, int _weight, int _area) {
  x = _x;
  y = _y;
  weight = _weight;
  area = _area;
}
TP_Point::TP_Point(byte *data, int rot) {
  byte temp;
  x = data[0] & 0x0F;
  x <<= 8;
  x |= data[1];
  y = data[2] & 0x0F;
  y <<= 8;
  y |= data[3];
  weight = data[4];
  area = data[5] & 0x0F;
  switch (rot){
    case 0:
      x = 240 - x;
      y = 320 - y;
      break;
    case 1:
      temp = x;
      x = 320 - y;
      y = temp;
      break;
    case 2:
      x = x;
      y = y;
      break;
    case 3:
      temp = x;
      x = y;
      y = 240 - temp;
      break;
    default:
      break;
  }
}
bool TP_Point::operator==(TP_Point point) {
  return ((point.x == x) && (point.y == y) && (point.weight == weight) && (point.area == area));
}
bool TP_Point::operator!=(TP_Point point) {
  return ((point.x != x) || (point.y != y) || (point.weight != weight) || (point.area != area));
}
