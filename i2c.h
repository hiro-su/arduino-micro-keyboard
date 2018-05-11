#ifndef INCLUDED_I2C_H_

  #define INCLUDED_I2C_H_

  /*
    addr 0 = A2 low , A1 low , A0 low  000
    addr 1 = A2 low , A1 low , A0 high 001
    addr 2 = A2 low , A1 high , A0 low  010
    addr 3 = A2 low , A1 high , A0 high  011
    addr 4 = A2 high , A1 low , A0 low  100
    addr 5 = A2 high , A1 low , A0 high  101
    addr 6 = A2 high , A1 high , A0 low  110
    addr 7 = A2 high, A1 high, A0 high 111
  
    Connect pin #12 of the expander to Analog 5 (i2c clock)
    Connect pin #13 of the expander to Analog 4 (i2c data)
    Connect pins #15, 16 and 17 of the expander to ground (address selection)
    Connect pin #9 of the expander to 5V (power)
    Connect pin #10 of the expander to ground (common ground)
    Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)
  
    Input #0 is on pin 21 so connect a button or switch from there to ground
  */
  #define MCP_ADDR_RIGHT 0
  #define MCP_ADDR_LEFT 4

#endif
