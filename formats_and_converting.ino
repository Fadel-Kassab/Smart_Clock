#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

bool page = 0;
const short ctrlBtn = 4;
const short rBtn = 3;
const short lBtn = 2;
const short upBtn = 5;
const short downBtn = 6;
unsigned long eventClock = 200;
unsigned long previousClock = 0;
unsigned long eventBtn = 200;
unsigned long previousBtn = 0;
short m1 = 0;
short m2 = 0;
short h1 = 2;
short h2 = 1;
const String am_pm[2] = {"AM", "PM"};
boolean ap = 0;
boolean am_pm_gate = true;
boolean format = 0;
String adjustmentArrow;
short colAdjustmentArrow = 5;
short colClock = 5;
bool rowClock = 0;

void setup(void) 
{
  lcd.begin(16, 2);
  pinMode(lBtn, INPUT_PULLUP);
  pinMode(rBtn, INPUT_PULLUP);
  pinMode(upBtn, INPUT_PULLUP);
  pinMode(ctrlBtn, INPUT_PULLUP);
  pinMode(downBtn, INPUT_PULLUP);
}

void loop(void) 
{
  unsigned long current = millis();

  if(current - previousClock == eventClock)
  {
    Clock();
    previousClock = current;
  }

  if(current - previousBtn == eventBtn)
  {
    if(page == 1)
    {
      lcd.setCursor(colClock, rowClock);
      lcd.print(h2);
      lcd.setCursor(colClock + 1, rowClock);
      lcd.print(h1);
      lcd.setCursor(colClock + 2, rowClock);
      lcd.print(":");
      lcd.setCursor(colClock + 3, rowClock);
      lcd.print(m2);
      lcd.setCursor(colClock + 4, rowClock);
      lcd.print(m1);    
      if(format == 0)
      {
        lcd.setCursor(colClock + 5, rowClock);
        lcd.print(am_pm[ap]); 
      }
  
      if(digitalRead(ctrlBtn) == LOW)
      {
        convert_format();
      }
    }
    
    else if(page == 0)
    {
      lcd.setCursor(colClock, rowClock);
      lcd.print(h2);
      lcd.setCursor(colClock + 1, rowClock);
      lcd.print(h1);
      lcd.setCursor(colClock + 2, rowClock);
      lcd.print(":");
      lcd.setCursor(colClock + 3, rowClock);
      lcd.print(m2);
      lcd.setCursor(colClock + 4, rowClock);
      lcd.print(m1);
      lcd.setCursor(colAdjustmentArrow, 1);
      lcd.print(adjustmentArrow);
      lcd.setCursor(13, 1);
      lcd.print("OK");
      if(format == 0)
      {
        lcd.setCursor(colClock + 5, rowClock);
        lcd.print(am_pm[ap]);
      }
      else
      {
        lcd.setCursor(colClock + 5, rowClock);
        lcd.print("H~24");
      }
      
      // moving
      if(digitalRead(rBtn) == LOW)
      {
        move_arrow("RIGHT");
      }
      else if(digitalRead(lBtn) == LOW)
      {
        move_arrow("LEFT");
      }
      
      switch(colAdjustmentArrow)
      {
        case 12:
          adjustmentArrow = ">";
          if(digitalRead(ctrlBtn) == LOW)
          {
            adjustment_to_home();  
          }
          break;
        
        default:
          adjustmentArrow = "^";
          if(digitalRead(upBtn) == LOW)
          {
            switch(colAdjustmentArrow)
            {
              case 5:
                h2_change("UP");
                break;
  
              case 6:
                h1_change("UP");
                break;
  
              case 8:
                m2_change("UP");
                break;
  
              case 9:
                m1_change("UP");
                break;
  
              case 10:
                format_change("UP");
                break;
            }
          }

          else if(digitalRead(downBtn) == LOW)
          {
            switch(colAdjustmentArrow)
            {
              case 5:
                h2_change("DOWN");
                break;
  
              case 6:
                h1_change("DOWN");
                break;
  
              case 8:
                m2_change("DOWN");
                break;
  
              case 9:
                m1_change("DOWN");
                break;
  
              case 10:
                format_change("DOWN");
                break;
            }
          }
          break;
      }
    }
    previousBtn = current;
  }
}
// mission 1 functions //
void Clock(void)
{
  if(m2 == 5 && m1 == 9)
  {
    m2 = 0;
    m1 = 0;
    h1++;
  }
  else if(m1 == 9)
  {
    m1 = 0;
    m2++;  
  }
  else
  {
    m1++;
  }
  
  if(format == 0)
  {
    day_night_am_pm();   
    if (h2 == 1 && h1 == 3)
    {
      h1 = 1;
      h2 = 0;
    } 
    
    else if (h1 == 10)
    {
      h1 = 0;
      h2++; 
    } 
  }

  else if(format == 1) // to be removed
  {
    day_night_24h();
    if(h1 == 4 && h2 == 2)
    {
      h1 = 0;
      h2 = 0;  
    }
    
    else if(h1 == 10)
    {
      h1 = 0;
      h2++;
    }
  }
}

void day_night_am_pm(void)
{
  short currentHour = (h2 * 10) + h1;
  
  if(currentHour == 12 && am_pm_gate == false)
  {
    if(ap == true)
    {
      ap = false;
    }

    else if(ap == false) // to be removed
    {
      ap = true;
    }
    am_pm_gate = true; // we need to know how to open the gate properly
  }

  else if(currentHour != 12) // gates will open this way to prevent errors
  {
    am_pm_gate = false;
  }
}

void day_night_24h(void)
{
  short currentHour = (h2 * 10) + h1;
  
  if(currentHour < 12)
  {
    ap = false;
  }

  else if(currentHour >= 12) // to be removed
  {
    ap = true;
  }

  if(currentHour != 12 && currentHour != 0)
  {
    am_pm_gate = false;
  }
}

void convert_format(void)
{
  am_pm_gate = true; // gates should be up to not allow errors happen on hour 12
  
  if(format == 0)
  {
    if(ap == 0)
    {
      if(h2 == 1 && h1 == 2)
      {
        h2 = 0;
        h1 = 0;
      }
    }
  
    else
    {
      if(h2 == 0)
      {
        if(h1 < 8)
        {
          h1 = h1 + 2;
          h2 = 1;
        }
  
        else if(h1 <= 9)
        {
          h1 = h1 - 8;
          h2 = 2;
        }
      }
   
      else if(h2 == 1)
      {
        if(h1 < 2)
        {
          h1 = h1 + 2;
          h2 = 2;
        }
      }
    }
  }

  else if(format == 1)
  {
    if(ap == 0)
    {
      if(h2 == 0 && h1 == 0)
      {
        h2 = 1;
        h1 = 2;
      }
    }
  
    else if(ap == 1)
    {
      if(h2 == 1 && h1 != 2)
      {
        h1 = h1 - 2;
        h2 = 0;
      }
  
      else if(h2 == 2)
      {
        if(h1 < 2)
        {
          h1 = h1 + 8;
          h2 = 0;  
        }
  
        else if(h1 <= 3)
        {
          h1 = h1 - 2;
          h2 = 1;
        }
      }
    }
  }

  if(format == 1)
  {
    format = 0;
  }

  else
  {
    format = 1;
    lcd.setCursor(colClock + 5, rowClock);
    lcd.print("  ");
  }
}

// mission 2 functions //
void adjustment_to_home(void)
{
  short currentHour = (h2 * 10) + h1; 
  
  if(format == 0)
  {
    if(currentHour != 12)
    {
      am_pm_gate = false;
    }
    
    else
    {
      am_pm_gate = true;
    }
  }

  else
  {
    if(currentHour != 12 && currentHour != 0)
    {
      am_pm_gate = false;
    }

    else
    {
      am_pm_gate = true;
    }
  }

  lcd.clear();
  page = 1;
}

void move_arrow(String Direction)
{
  lcd.setCursor(colAdjustmentArrow, 1);
  lcd.print(" ");
  
  if(Direction == "RIGHT")
  {
    if(colAdjustmentArrow == 12)
    {
      colAdjustmentArrow = 5;
    }
    else if(colAdjustmentArrow == 6)
    {
      colAdjustmentArrow = 8;
    }
    else if(colAdjustmentArrow == 10)
    {
      colAdjustmentArrow = 12;
    }
    else
    {
      colAdjustmentArrow++;
    }
  }

  else
  {
    if(colAdjustmentArrow == 5)
    {
      colAdjustmentArrow = 12;
    }
    else if(colAdjustmentArrow == 8)
    {
      colAdjustmentArrow = 6;
    }
    else if(colAdjustmentArrow == 12)
    {
      colAdjustmentArrow = 10;
    }
    else
    {
      colAdjustmentArrow--;
    }
  }
}

void h1_change(String Direction)
{
  short currentHour = (h2 * 10) + h1;
  if(Direction == "UP")
  {
    if(format == 0)
    {
      if(currentHour == 12)
      {
        h2 = 0;
        h1 = 1;     
      }

      else if(currentHour == 9)
      {
        h2 = 1;
        h1 = 0;
      }

      else
      {
        h1++;
      }
    }

    else
    {
      if(currentHour == 9)
      {
        h2 = 1;
        h1 = 0;
      }

      else if(currentHour == 19)
      {
        h2 = 2;
        h1 = 0;
      }

      else if(currentHour == 23)
      {
        h2 = 0;
        h1 = 0;
      }

      else
      {
        h1++;
      }
    }
  }

  else
  {
    if(format == 0)
    {
      if(currentHour == 1)
      {
        h2 = 1;
        h1 = 2;
      }

      else if(currentHour == 10)
      {
        h2 = 0;
        h1 = 9;
      }

      else
      {
        h1--;
      }
    }

    else
    {
      if(currentHour == 0)
      {
        h2 = 2;
        h1 = 3;
      }

      else if(currentHour == 10)
      {
        h2 = 0;
        h1 = 9;
      }

      else if(currentHour == 20)
      {
        h2 = 1;
        h1 = 9;
      }

      else 
      {
        h1--;
      }
    }
  }
}

void h2_change(String Direction)
{
  if(format == 0)
  {
    if(h2 == 0)
    {
      h2 = 1;  
    }

    else
    {
      h2 = 0;
    }
  }

  else
  {
    if(Direction == "UP")
    {
      if(h2 == 2)
      {
        h2 = 0;
      }
  
      else
      {
        h2++;
      }
    }

    else
    {
      if(h2 == 0)
      {
        h2 = 2;
      }

      else
      {
        h2--;
      }
    }
  }
}

void m1_change(String Direction)
{
  if(Direction == "UP")
  {
    if(m1 == 9)
    {
      if(m2 == 5)
      {
        m2 = 0;
      }

      else
      {
        m2++;
      }
      m1 = 0;
    }

    else
    {
      m1++;
    }
  }

  else
  {
    if(m1 == 0)
    {
      if(m2 == 0)
      {
        m2 = 5;
      }

      else
      {
        m2--;
      }
      m1 = 9;
    }

    else
    {
      m1--;
    }
  }
}

void m2_change(String Direction)
{
  if(Direction == "UP")
  {
    if(m2 == 5)
    {
      m2 = 0;
    }

    else
    {
      m2++;
    }
  }

  else
  {
    if(m2 == 0)
    {
      m2 = 5;
    }

    else
    {
      m2--;
    }
  }
}

void format_change(String Direction)
{
  lcd.setCursor(colClock + 5, 0);
  lcd.print("    ");
  if(Direction == "UP")
  {
    if(format == 0)
    {
      if(ap == 1)
      {
        convert_format();
      }
  
      else
      {
        ap = 1;
      }
    }

    else
    {
      convert_format();
      ap = 0;
    }
  }

  else
  {
    if(format == 0)
    {
      if(ap == 0)
      {
        convert_format();
      }
  
      else
      {
        ap = 0;
      }
    }

    else
    {
      convert_format();
      ap = 1;
    }
  }
}
