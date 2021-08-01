#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
String quotes[15] = {
"HAKUNA MATATA",
"AIM HIGH",
"SCARE YOUR FEARS",
"AMPLIFY HOPE",
"FEAR NOT",
"SHINE ON",
"WHERE ARE YOU?",
"WHY NOT?",
"DREAM BIG",
"LET IT BE",
"JUST DO IT!",
"CHANGE THE GAME",
"GAME ON",
"SPEAK THE TRUTH",
"AND STILL I RISE"};

unsigned long previous = 0;
unsigned long event = 4500;
String quote;
short colQuote;
bool startAnimation = true;

byte music[] = {
  B00100,
  B00110,
  B00101,
  B00100,
  B00100,
  B01100,
  B01100,
  B00000
};
byte iceBall[] = {
  B00111,
  B01110,
  B11111,
  B11011,
  B11011,
  B11111,
  B01110,
  B11100
};
byte tree[] = {
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B00100,
  B00100
};
byte heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};
byte turtle[] = {
  B00100,
  B10101,
  B01110,
  B01110,
  B01110,
  B01110,
  B10001,
  B00000
};

void setup(void)
{
  lcd.begin(16, 2);
  lcd.createChar(0, music);
  lcd.createChar(1, iceBall);
  lcd.createChar(2, tree);
  lcd.createChar(3, heart);
  lcd.createChar(4, turtle);
}

void loop(void)
{
  unsigned long current = millis();  
  
  if(current - previous == event)
  {
    startAnimation = true;
    lcd.clear();   
    previous = current; 
  }
  view_quotes();
}

void view_quotes(void)
{
  if(startAnimation == true)
  {
    quote = quotes[random(0, 15)];
    
    if(quote.length() % 2 == 0)
    {
      colQuote = (16 - quote.length()) / 2;
      lcd.setCursor(colQuote, 0);
      lcd.print(quote);
      print_bytes();
    }
  
    else
    {
      lcd.setCursor(0, random(0, 2));
      lcd.print(quote);   
    }
  startAnimation = false;
  }
}

void print_bytes(void)
{
  short choice = random(0, 4);
  short Byte = byte(random(0, 5));
  
  if(choice == 1)
  {
    if(quote.length() != 16)
    {
      lcd.setCursor(colQuote - 1, 0);
      lcd.write(Byte);
      lcd.setCursor(colQuote + quote.length(), 0);
      lcd.write(Byte);
    }
    
    short choice_2 = random(0, 6);
    short Byte_2 = byte(random(0, 5));
    
    if(choice_2 == 2)
    {
      lcd.setCursor(6, 1);
      lcd.write(Byte_2);
      lcd.setCursor(9, 1);
      lcd.write(Byte_2);
    }
  }
}
