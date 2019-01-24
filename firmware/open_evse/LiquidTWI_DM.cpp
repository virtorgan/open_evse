#include "LiquidTWI_DM.h"
#include <Wire.h>

/*
uint8_t _displayfunction; // Function set
uint8_t _displaycontrol; // Cursor & Display shift
uint8_t _displaymode; // Display ON OFF & Blinking
uint8_t _numlines,_currline;
uint8_t _i2cAddr;
uint8_t _backlightInverted;
uint8_t _backlightColor;
*/
//**************************************************************************
// Constructor
//**************************************************************************
LiquidTWI_DM::LiquidTWI_DM(uint8_t i2cAddr,uint8_t detectDevice,uint8_t backlightInverted) {
  // if detectDevice != 0, set _deviceDetected to 2 to flag that we should
  // scan for it in begin()
#ifdef DETECT_DEVICE
  _deviceDetected = detectDevice ? 2 : 1;
#endif
  _backlightInverted = backlightInverted;

  //  if (i2cAddr > 7) i2cAddr = 7;
  _i2cAddr = i2cAddr; // transfer this function call's number into our internal class state
  _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS; // in case they forget to call begin() at least we have something
}

//**************************************************************************
// begin
//**************************************************************************
void LiquidTWI_DM::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
    uint8_t result;
      // now set up input/output pins
    Wire.begin();
    Wire.beginTransmission(MCP23017_ADDRESS);
    Wire.write(MCP23017_IODIRA);
    Wire.write(BUTTON_SELECT); // PA1 button input, all others output
    Wire.write(0x00); // all PORTB pins output
    result = Wire.endTransmission();
    #ifdef DETECT_DEVICE
      if (result) {
        if (_deviceDetected == 2) {
          _deviceDetected = 0;
          return;
        }
      }
    #endif

  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;
  _currline = 0;
  _backlightColor = (WHITE << 2);

  command(LCD_FUNCTIONSET | _displayfunction); // then send 0011NF00 (N=lines, F=font)
  delay(5); // for safe keeping...



  // clear it off
  clear();
  // turn on Display
  display();
  _displaymode = LCD_ENTRYINCREMENT | LCD_ENTRYNOSHIFT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);
#ifdef DETECT_DEVICE
  // If we haven't failed by now, then we pass
  if (_deviceDetected == 2) _deviceDetected = 1;
#endif
}

//**************************************************************************
// clear
//**************************************************************************
void LiquidTWI_DM::clear()
{
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}
//**************************************************************************
// home
//**************************************************************************
void LiquidTWI_DM::home()
{
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}
//**************************************************************************
// noDisplay
//**************************************************************************
void LiquidTWI_DM::noDisplay() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
//**************************************************************************
// display
//**************************************************************************
void LiquidTWI_DM::display() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
//**************************************************************************
// noBlink
//**************************************************************************
void LiquidTWI_DM::noBlink() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
//**************************************************************************
// blink
//**************************************************************************
void LiquidTWI_DM::blink() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
//**************************************************************************
// noCursor
//**************************************************************************
void LiquidTWI_DM::noCursor() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
//**************************************************************************
// cursor
//**************************************************************************
void LiquidTWI_DM::cursor() {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
//**************************************************************************
// scrollDisplayLeft
//**************************************************************************
void LiquidTWI_DM::scrollDisplayLeft(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
//**************************************************************************
// scrollDisplayRight
//**************************************************************************
void LiquidTWI_DM::scrollDisplayRight(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}
//**************************************************************************
// leftToRight
//**************************************************************************
void LiquidTWI_DM::leftToRight(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaymode |= LCD_ENTRYINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}
//**************************************************************************
// rightToLeft
//**************************************************************************
void LiquidTWI_DM::rightToLeft(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaymode &= ~LCD_ENTRYINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}
//**************************************************************************
// autoscroll
//**************************************************************************
void LiquidTWI_DM::autoscroll(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaymode |= LCD_ENTRYSHIFT;
  command(LCD_ENTRYMODESET | _displaymode);
}
//**************************************************************************
// noAutoscroll
//**************************************************************************
void LiquidTWI_DM::noAutoscroll(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  _displaymode &= ~LCD_ENTRYSHIFT;
  command(LCD_ENTRYMODESET | _displaymode);
}
//**************************************************************************
// setBacklight
//**************************************************************************
void LiquidTWI_DM::setBacklight(uint8_t status) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
_backlightColor = status << 2;
Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
Wire.write(MCP23017_GPIOA);
Wire.write(_backlightColor);
Wire.endTransmission();
}
//**************************************************************************
// createChar
//**************************************************************************
void LiquidTWI_DM::createChar(uint8_t location, uint8_t charmap[]) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}
//**************************************************************************
// setCursor
//**************************************************************************
void LiquidTWI_DM::setCursor(uint8_t col, uint8_t row)
{
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > _numlines ) row = _numlines - 1;    // we count rows starting w/0
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}
//**************************************************************************
// write
//**************************************************************************
#if defined(ARDUINO) && (ARDUINO >= 100) //scl
inline size_t LiquidTWI_DM::write(uint8_t value) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return 1;
#endif
  send(value, HIGH);
  return 1;
}
#else
inline void LiquidTWI_DM::write(uint8_t value) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return;
#endif
  send(value, HIGH);
}
#endif
//**************************************************************************
// command
//**************************************************************************
inline void LiquidTWI_DM::command(uint8_t value) {
  send(value, LOW);
}
//**************************************************************************
// readButtons
//**************************************************************************
uint8_t LiquidTWI_DM::readButtons(void) {
#ifdef DETECT_DEVICE
  if (!_deviceDetected) return 0;
#endif
  Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
  Wire.write(MCP23017_GPIOA);
  Wire.endTransmission();
  Wire.requestFrom(MCP23017_ADDRESS | _i2cAddr, 1);
  return ~Wire.read() & BUTTON_SELECT;
}
//**************************************************************************
// readRegister
//**************************************************************************
uint8_t LiquidTWI_DM::readRegister(uint8_t reg) {
  // read a register
  Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(MCP23017_ADDRESS | _i2cAddr, 1);
  return Wire.read();
}
//**************************************************************************
// setRegister
//**************************************************************************
void LiquidTWI_DM::setRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MCP23017_ADDRESS | _i2cAddr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
//**************************************************************************
// send
//**************************************************************************
void LiquidTWI_DM::send(uint8_t value, uint8_t mode) {
  // n.b. RW bit stays LOW to write
  uint8_t buf = _backlightColor;
  if (mode) buf |= (BIT_RS | BIT_EN);
  else buf |= BIT_EN;
  Wire.beginTransmission(MCP23017_ADDRESS |_i2cAddr);
  Wire.write(MCP23017_GPIOA);
  Wire.write(buf);
  Wire.write(value);
  while(Wire.endTransmission());
  //delay(5);
  buf &= ~BIT_EN;
  Wire.beginTransmission(MCP23017_ADDRESS |_i2cAddr);
  Wire.write(MCP23017_GPIOA);
  Wire.write(buf);
  while(Wire.endTransmission());
  //delay(5);

  Wire.beginTransmission(MCP23017_ADDRESS |_i2cAddr);
  Wire.write(MCP23017_GPIOA);
  Wire.write(_backlightColor);
  while(Wire.endTransmission());
}
