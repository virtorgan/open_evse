#ifndef LiquidTWI_DM_h
#define LiquidTWI_DM_h

#include "Arduino.h"
#include "print.h"

#define DETECT_DEVICE
#define MCP23017_ADDRESS 0x20

// Colors
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

//
// button bit PA1
#define BUTTON_SELECT 0x02

// address registers
#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14

#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

// commands
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

// flags for display entry mode
#define LCD_ENTRYDECREMENT 0x00
#define LCD_ENTRYINCREMENT 0x02
#define LCD_ENTRYSHIFT     0x01
#define LCD_ENTRYNOSHIFT   0x00

// flags for display on/off control
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_BACKLIGHT  0x08 // used to pick out the backlight flag since _displaycontrol will never set itself

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// PortA control bits
#define BIT_RS 0b10000000
#define BIT_RW 0b01000000
#define BIT_EN 0b00100000

// For compatibility
#define LTI_TYPE_MCP23017 1

class LiquidTWI_DM : public Print {
public:
	LiquidTWI_DM(uint8_t i2cAddr,uint8_t detectDevice=0,uint8_t backlightInverted=0);

	void begin(uint8_t cols, uint8_t rows,uint8_t charsize = LCD_5x8DOTS);

#ifdef DETECT_DEVICE
	uint8_t LcdDetected() { return _deviceDetected; }
#endif // DETECT_DEVICE
	void clear();
	void home();

	void noDisplay();
	void display();
	void noBlink();
	void blink();
	void noCursor();
	void cursor();
	void scrollDisplayLeft();
	void scrollDisplayRight();
	void leftToRight();
	void rightToLeft();
	void autoscroll();
	void noAutoscroll();
	void setBacklight(uint8_t status);
	void createChar(uint8_t, uint8_t[]);
	void setCursor(uint8_t, uint8_t);
#if defined(ARDUINO) && (ARDUINO >= 100) // scl
	virtual size_t write(uint8_t);
#else
	virtual void write(uint8_t);
#endif
	void command(uint8_t);
	uint8_t readButtons();//read button position
    uint8_t readRegister(uint8_t); //check registers
    void setRegister(uint8_t, uint8_t);  //set registers

		void setMCPType(uint8_t mcptype) {
	// here only for compatibility with LiquidTWI2
	  }
private:
	void send(uint8_t, uint8_t);
	uint8_t _displayfunction; // Function set
	uint8_t _displaycontrol; // Cursor & Display shift
	uint8_t _displaymode; // Display ON OFF & Blinking
	uint8_t _numlines,_currline;
	uint8_t _i2cAddr;
	uint8_t _backlightInverted;
  uint8_t _backlightColor;
#ifdef DETECT_DEVICE
	uint8_t _deviceDetected;
#endif // DETECT_DEVICE
};
#endif
