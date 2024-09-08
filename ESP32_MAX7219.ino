const int MAX7219_DS_pin  = 19;
const int MAX7219_LTH_pin = 18;
const int MAX7219_CLK_pin = 5;


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

// MAX7219'a veri gönderme fonksiyonu (16 bit veri gönderir)
void max7219_send(byte address, byte data) {
  digitalWrite(MAX7219_LTH_pin, LOW);  // Veri gönderimi için chip select aktif
  shiftOut(MAX7219_DS_pin, MAX7219_CLK_pin, MSBFIRST, address);  // İlk 8 bit (komut)
  shiftOut(MAX7219_DS_pin, MAX7219_CLK_pin, MSBFIRST, data);     // İkinci 8 bit (veri)
  digitalWrite(MAX7219_LTH_pin, HIGH);  // Chip select'i pasif yap
}

void Write_Current(float current)
{
  const int seg1 = 3;
  const int seg2 = 4;


  int val1 = Convert_BCD((int)current/10);
  int val2 = Convert_BCD(((int)current)%10);
  max7219_send(seg1, val1); 
  max7219_send(seg2, val2); 
}

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

// MAX7219 başlatma fonksiyonu (çalışma modunu ayarlar)
void max7219_init() {
  
  
  max7219_send(0x09, 0x00);  // Decode modunu kapat
  max7219_send(0x0A, 0x08);  // Orta parlaklık (0x00-0x0F arası değer)
  max7219_send(0x0B, 0x07);  // Tüm 8 digit'i aktif et
  max7219_send(0x0C, 0x01);  // Normal çalışma moduna geç
  max7219_send(0x0F, 0x00);  // Display testi kapalı

  for(int i=1;i <= 8;i++)
  {
    max7219_send(i, 0x00);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(MAX7219_DS_pin,  OUTPUT);
  pinMode(MAX7219_LTH_pin, OUTPUT);
  pinMode(MAX7219_CLK_pin, OUTPUT);

  digitalWrite(MAX7219_LTH_pin, HIGH);

  max7219_init();  // MAX7219'u başlat
}

void loop() {
  Write_Current(7);
  Write_Voltage(12);
  /*
  for(int i=0;i<10;i++)
  {
    max7219_send(1, Convert_BCD(i)); 
    max7219_send(2, Convert_BCD(i)); 
    max7219_send(3, Convert_BCD(i)); 
    max7219_send(4, Convert_BCD(i)); 
    delay(2000);
  }*/
}

