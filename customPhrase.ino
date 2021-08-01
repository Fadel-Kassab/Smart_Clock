#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const short lBtn = 2;
const short rBtn = 3;
const short ctrlBtn = 4;
const short upBtn = 5;
const short downBtn = 6;
unsigned long previous = 0;
unsigned long event = 200;

bool page = 0;
short ActualLength = 1;
char userPhrase[13];
const String Data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?<>{}[]()~$%*/+-=|&;: ";
short character = 0;
short colUserPhrase = 0;

void setup(void)
{
  lcd.begin(16, 2);
  pinMode(lBtn, INPUT_PULLUP);
  pinMode(rBtn, INPUT_PULLUP);
  pinMode(ctrlBtn, INPUT_PULLUP);
  pinMode(upBtn, INPUT_PULLUP);
  pinMode(downBtn, INPUT_PULLUP);
}

void loop(void)
{
  unsigned long current = millis();
  if(current - previous == event)
  {
    if(page == 0)
    {
      lcd.setCursor(colUserPhrase, 0);
      lcd.print(Data[character]);
      lcd.setCursor(colUserPhrase, 1);
      lcd.print("^");
      
      if(digitalRead(upBtn) == LOW)
      {
        change_char("UP");
      }
  
      else if(digitalRead(downBtn) == LOW)
      {
        change_char("DOWN");      
      }
  
      else if(digitalRead(rBtn) == LOW)
      {
        next_char("NEXT", false);
      }
  
      else if(digitalRead(lBtn) == LOW)
      {
        next_char("BACK", false);
      }
  
      else if(digitalRead(ctrlBtn) == LOW)
      {
        userPhrase[colUserPhrase] = Data[character];
        lcd.clear();
        page = 1;
      } 
    }

    else if(page == 1)
    {
      lcd.setCursor(1, 0);
      lcd.print(userPhrase);
    }
    previous = current;  
  }
}

void change_char(String Direction)
{
  const short arrayEnd = Data.length() - 1;
  
  if(Direction == "UP")
  {
    if(character == arrayEnd)
    {
      character = 0;
    }

    else
    {
      character++;
    }  
  }

  else
  {
    if(character == 0)
    {
      character = arrayEnd;  
    }

    else
    {
      character--;
    }
  }
}

void next_char(String Direction, bool mode)
{
  if(mode == 0)
  {
    lcd.setCursor(colUserPhrase, 1);
    lcd.print(" ");
    if(Direction == "NEXT")
    {
      userPhrase[colUserPhrase] = Data[character];
      if(colUserPhrase < 13)
      {
        colUserPhrase++; 
      }
  
      else
      {
        colUserPhrase = 0;
      }
      character = 0; 
    }
  
    else
    {
      if(colUserPhrase > 0)
      {
        colUserPhrase--;
      }
  
      else
      {
        colUserPhrase = 13;
      }
    } 
  }

  else if(mode == 1)
  {
    
  }
}
