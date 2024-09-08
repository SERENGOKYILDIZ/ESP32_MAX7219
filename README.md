# ESP32 MAX7219

**COMMENT**</br>
4 Common cathode 7 segments are installed on the MAX7219.
</br></br>
**CODE**
```c++
//* PIN SETTINGS *//
const int MAX7219_DS_pin  = 19;
const int MAX7219_LTH_pin = 18;
const int MAX7219_CLK_pin = 5;

void setup() {
  Serial.begin(115200);

  pinMode(MAX7219_DS_pin,  OUTPUT);
  pinMode(MAX7219_LTH_pin, OUTPUT);
  pinMode(MAX7219_CLK_pin, OUTPUT);

  digitalWrite(MAX7219_LTH_pin, HIGH);

  max7219_init();  // Initialize the MAX7219
}

void loop() {

  Write_Current(7);
  Write_Voltage(12);
  delay(2000);

  for(int i=0;i<10;i++)
  {
    max7219_send(1, Convert_BCD(i)); 
    max7219_send(2, Convert_BCD(i)); 
    max7219_send(3, Convert_BCD(i)); 
    max7219_send(4, Convert_BCD(i)); 
    delay(2000);
  }

}

//* Converts data to 7 segment format. *//
int Convert_BCD(int value)
{
  switch(value)
  {
    case 0:
    return 0x7E;

    case 1:
    return 0x30;

    case 2:
    return 0x6D;

    case 3:
    return 0x79;

    case 4:
    return 0x33;

    case 5:
    return 0x5B;

    case 6:
    return 0x5F;

    case 7:
    return 0x70;

    case 8:
    return 0xFF;

    case 9:
    return 0x7B;

    default:
    return 0x00;

  }
}

//* Function to send data to MAX7219 (sends 16 bits of data) *//
void max7219_send(byte address, byte data) {
  digitalWrite(MAX7219_LTH_pin, LOW);  // Activate chip select for data transmission
  shiftOut(MAX7219_DS_pin, MAX7219_CLK_pin, MSBFIRST, address);  // First 8 bits (command)
  shiftOut(MAX7219_DS_pin, MAX7219_CLK_pin, MSBFIRST, data);     // Second 8 bits (data)
  digitalWrite(MAX7219_LTH_pin, HIGH);  // Deactivate chip select
}

//* Function to initialize the MAX7219 (sets the operating mode) *//
void max7219_init() {
  
  
  max7219_send(0x09, 0x00);  // Turn off decode mode
  max7219_send(0x0A, 0x08);  // Set medium brightness (value between 0x00-0x0F)
  max7219_send(0x0B, 0x07);  // Activate all 8 digits
  max7219_send(0x0C, 0x01);  // Switch to normal operating mode
  max7219_send(0x0F, 0x00);  // Turn off display test

  for(int i=1;i <= 8;i++)
  {
    max7219_send(i, 0x00);
  }
}

//* Example function for current reading *//
void Write_Current(float current)
{
  const int seg1 = 3;
  const int seg2 = 4;


  int val1 = Convert_BCD((int)current/10);
  int val2 = Convert_BCD(((int)current)%10);
  max7219_send(seg1, val1); 
  max7219_send(seg2, val2); 
}

//* Example function for voltage reading *//
void Write_Voltage(float voltage)
{
  const int seg1 = 1;
  const int seg2 = 2;


  int val1 = Convert_BCD((int)voltage/10);
  int val2 = Convert_BCD(((int)voltage)%10);
  Serial.printf("Val1 : 0x%x \t Val2 : 0x%x\n", val1, val2);
  max7219_send(seg1, val1); 
  max7219_send(seg2, val2); 
}
```
