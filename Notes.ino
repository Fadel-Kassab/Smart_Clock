const short lBtn = 2;
const short rBtn = 3;
const short ctrlBtn = 4;
const short upBtn = 5;
const short downBtn = 6;
unsigned long previous = 0;
unsigned long event = 200;
short page = 0;

char line_one[16];
char line_two[16];
const String Data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?<>{}[]()~$%*/+-=|&;: ";
short character = 0;
short colWriting = 0;
bool rowWritingChar = 0;
bool rowWritingArrow = 1;

byte downArrow[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10001,
  B01010,
  B00100
};
void setup(void)
{
  lcd.begin(16, 2);
  pinMode(lBtn, INPUT_PULLUP);
  pinMode(rBtn, INPUT_PULLUP);
  pinMode(ctrlBtn, INPUT_PULLUP);
  pinMode(upBtn, INPUT_PULLUP);
  pinMode(downBtn, INPUT_PULLUP);
  lcd.createChar(0, downArrow);  
}

void loop(void)
{
  unsigned long current = millis();
  if(current - previous == event)
  {
    if(page == 0)
    {
      lcd.setCursor(colWriting, rowWritingChar);
      lcd.print(Data[character]);
      lcd.setCursor(colWriting, rowWritingArrow);
      if(rowWritingArrow == 1)
      {
        lcd.print("^"); // arrows[arrow]  
      }
      else
      {
        lcd.write(byte(0));
      }
  
      
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
        next_char("NEXT", true);
      }
  
      else if(digitalRead(lBtn) == LOW)
      {
        next_char("BACK", true);
      }
  
      else if(digitalRead(ctrlBtn) == LOW)
      {
        if(rowWritingChar == 0)
        {
          line_one[colWriting] = Data[character];  
        }
  
        else
        {
          line_two[colWriting] = Data[character]; 
        }
        lcd.clear();
        page = 1;
      } 
    }
  
    else if(page == 1)
    {
      if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        character = 0;
        rowWritingChar = 0;
        rowWritingArrow = 1;
        colWriting = 0;
        page--;
        line_one[16];
        line_two[16];
      }
      lcd.setCursor(0, 0);
      lcd.print(line_one);
      lcd.setCursor(0, 1);
      lcd.print(line_two);
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
//    lcd.setCursor(colUserPhrase, 1);
//    lcd.print(" ");
//    if(Direction == "NEXT")
//    {
//      userPhrase[colUserPhrase] = Data[character];
//      if(colUserPhrase < 13)
//      {
//        colUserPhrase++; 
//      }
//      character = 0; 
//    }
//  
//    else
//    {
//      if(colUserPhrase > 0)
//      {
//        colUserPhrase--;
//      }
//      character = 0;
//    } 
  }

  else if(mode == 1)
  {
    lcd.setCursor(colWriting, rowWritingArrow);
    lcd.print(" ");
    if(Direction == "NEXT")
    {
      if(rowWritingChar == 0)
      {
        line_one[colWriting] = Data[character];
        if(colWriting < 15)
        {
          colWriting++;
        }

        else
        {
          rowWritingArrow = 0;
          rowWritingChar = 1;
          colWriting = 0;
        }
      }

      else
      {
        line_two[colWriting] = Data[character];
        if(colWriting < 15)
        {
          colWriting++;
        }
      }
    }

    else
    {
      if(rowWritingChar == 0)
      {
        if(colWriting > 0)
        {
          colWriting--;
        }
      }

      else
      {
        if(colWriting > 0)
        { 
          colWriting--;      
        }

        else
        {
          rowWritingArrow = 1;
          rowWritingChar = 0;
          colWriting = 15;
        }
      }
    }
    character = 0;
      lcd.setCursor(0, 1);
      lcd.print(line_two); 
  
      lcd.setCursor(0, 0);
      lcd.print(line_one);
  }
}
