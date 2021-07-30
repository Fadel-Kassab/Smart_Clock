#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
short page = 2;
short colClock = 5;
bool rowClock = 0;
short colAdjustmentArrow = 5;
const short ctrlBtn = 4;
const short rBtn = 3;
const short lBtn = 2;
const short upBtn = 5;
const short downBtn = 6;
unsigned long eventClock = 200;
unsigned long previousClock = 0;
unsigned long eventBtn = 200;
unsigned long previousBtn = 0;
const String arrows[5] = {"<", ">", "^", "<-", "->"};
short arrow;
short m1 = 0;
short m2 = 0;
short h1 = 2;
short h2 = 1;
const String am_pm[2] = {"AM", "PM"};
boolean ap = 0;
boolean am_pm_gate = true;
boolean format = 0;
short alarm_m1 = 0;
short alarm_m2 = 0;
short alarm_h1 = 2;
short alarm_h2 = 1;
boolean alarm_ap = 0;
boolean alarm_on = false;
short target_h1;
short target_h2;
short target_m1;
short target_m2;
short target_ap;
short target_repeats = 1;
short alarm_repeats = 1;

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
  
  if(current - previousClock == eventClock)
  {
    Clock();
    previousClock = current;
  }
  
  if(current - previousBtn == eventBtn)
  {   
    if(page == 1)
    {
      if(digitalRead(rBtn) == LOW)
      {
        page++;
      }

      else if(digitalRead(lBtn) == LOW)
      {
        page--;
      }
      lcd.setCursor(5, 1);
      lcd.print(target_h2);
      lcd.setCursor(6, 1);
      lcd.print(target_h1);
      lcd.setCursor(8, 1);
      lcd.print(target_m2);
      lcd.setCursor(9, 1);
      lcd.print(target_m1);
      lcd.setCursor(10, 1);
      lcd.print(target_ap);
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

    else if(page == 2)
    {
      alarm_GUI();
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
          arrow = 1;
          if(digitalRead(ctrlBtn) == LOW)
          { 
            lcd.clear();
            page = 3;
            target_h2 = alarm_h2;
            target_h1 = alarm_h1;
            target_m1 = alarm_m1;
            target_m2 = alarm_m2;
            target_ap = alarm_ap;
          }
          break;
  
        case 4:
          arrow = 0;
          if(digitalRead(ctrlBtn) == LOW)
          {
            lcd.clear();
            page = 1;     
          }
          break;
  
        default:
          arrow = 2;
          if(digitalRead(upBtn) == LOW)
          {
            switch(colAdjustmentArrow)
            {
              case 5:
                h2_change("UP", alarm_h2, alarm_h1, 0);
                break;
  
              case 6:
                h1_change("UP", alarm_h2, alarm_h1, 0);
                break;
  
              case 8:
                m2_change("UP", alarm_m2, alarm_m1);
                break;
  
              case 9:
                m1_change("UP", alarm_m2, alarm_m1);
                break;
  
              case 10:
                am_pm_change();
                break;
             }
          }
  
          else if(digitalRead(downBtn) == LOW)
          {
            switch(colAdjustmentArrow)
            {
              case 5:
                h2_change("DOWN", alarm_h2, alarm_h1, 0);
                break;
  
              case 6:
                h1_change("DOWN", alarm_h2, alarm_h1, 0);
                break;
  
              case 8:
                m2_change("DOWN", alarm_m2, alarm_m1);
                break;
  
              case 9:
                m1_change("DOWN", alarm_m2, alarm_m1);
                break;
  
              case 10:
                am_pm_change();
                break;
            } 
          }
          break;
        }
      }
  
    else if(page == 3)
    {
      alarm_options();
    }

    else if(page == 4)
    {
      alarm_started_banner();
    }
    
    else if(page == 5)
    {
      alarm_done();
    }

    if(alarm_on == true)
    { 
      if(target_repeats == 10)
      {
        if(format == 0)
        {
          check_alarm();        
        }
      
        else
        {
          check_alarm();
        }      
      }
      else if(target_repeats > alarm_repeats)
      {
        if(format == 0)
        {
          check_alarm();        
        }
      
        else
        {
          check_alarm();
        }
      }

      if(target_repeats == alarm_repeats)
      {
        target_repeats = 1;
        alarm_repeats = 1;
        alarm_on = false;
      }
    }
    previousBtn = current;
  }
}

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
  
  if(alarm_on == true)
  {
    convert_alarm();
  }
  
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

void alarm_GUI(void)
{
  lcd.setCursor(7, 1);
  lcd.print(alarm_ap);
  lcd.setCursor(5, 0);
  lcd.print(alarm_h2); 
  lcd.setCursor(6, 0);
  lcd.print(alarm_h1);
  lcd.setCursor(7, 0);
  lcd.print(":");
  lcd.setCursor(8, 0);
  lcd.print(alarm_m2);
  lcd.setCursor(9, 0);
  lcd.print(alarm_m1);
  lcd.setCursor(10, 0);
  lcd.print(am_pm[alarm_ap]);
  lcd.setCursor(0, 1);
  lcd.print("BACK");
  lcd.setCursor(13, 1);
  lcd.print("OK");
  lcd.setCursor(colAdjustmentArrow, 1);
  lcd.print(arrows[arrow]);
}

void h1_change(String Direction, short &H2, short &H1, bool FORMAT)
{
  short currentHour = (H2 * 10) + H1;
  if(Direction == "UP")
  {
    if(FORMAT == 0)
    {
      if(currentHour == 12)
      {
        H2 = 0;
        H1 = 1;     
      }

      else if(currentHour == 9)
      {
        H2 = 1;
        H1 = 0;
      }

      else
      {
        H1++;
      }
    }

    else
    {
      if(currentHour == 9)
      {
        H2 = 1;
        H1 = 0;
      }

      else if(currentHour == 19)
      {
        H2 = 2;
        H1 = 0;
      }

      else if(currentHour == 23)
      {
        H2 = 0;
        H1 = 0;
      }

      else
      {
        H1++;
      }
    }
  }

  else
  {
    if(FORMAT == 0)
    {
      if(currentHour == 1)
      {
        H2 = 1;
        H1 = 2;
      }

      else if(currentHour == 10)
      {
        H2 = 0;
        H1 = 9;
      }

      else
      {
        H1--;
      }
    }

    else
    {
      if(currentHour == 0)
      {
        H2 = 2;
        H1 = 3;
      }

      else if(currentHour == 10)
      {
        H2 = 0;
        H1 = 9;
      }

      else if(currentHour == 20)
      {
        H2 = 1;
        H1 = 9;
      }

      else 
      {
        H1--;
      }
    }
  }
}

void h2_change(String Direction, short &H2, short &H1, bool FORMAT)
{
  if(FORMAT == 0)
  {
    if(H2 == 0)
    {
      H2 = 1;  
    }

    else
    {
      H2 = 0;
    }
  }

  else
  {
    if(Direction == "UP")
    {
      if(H2 == 2)
      {
        H2 = 0;
      }
  
      else
      {
        H2++;
      }
    }

    else
    {
      if(H2 == 0)
      {
        H2 = 2;
      }

      else
      {
        H2--;
      }
    }
  }
}

void m1_change(String Direction, short &M2, short &M1)
{
  if(Direction == "UP")
  {
    if(M1 == 9)
    {
      if(M2 == 5)
      {
        M2 = 0;
      }

      else
      {
        M2++;
      }
      M1 = 0;
    }

    else
    {
      M1++;
    }
  }

  else
  {
    if(M1 == 0)
    {
      if(M2 == 0)
      {
        M2 = 5;
      }

      else
      {
        M2--;
      }
      M1 = 9;
    }

    else
    {
      M1--;
    }
  }
}

void m2_change(String Direction, short &M2, short &M1)
{
  if(Direction == "UP")
  {
    if(M2 == 5)
    {
      M2 = 0;
    }

    else
    {
      M2++;
    }
  }

  else
  {
    if(M2 == 0)
    {
      M2 = 5;
    }

    else
    {
      M2--;
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

void am_pm_change(void)
{
  if(alarm_ap == 0)
  {
    alarm_ap = 1; 
  }

  else
  {
    alarm_ap = 0;
  }
}

void alarm_options(void)
{
  lcd.setCursor(5, 0);
  if(target_repeats < 10)
  {
    lcd.print(target_repeats);
    lcd.setCursor(6, 0);
    lcd.print("TIMES");
  }
  else
  {
    lcd.print("ALWAYS");
  }
  lcd.setCursor(5, 1);
  lcd.print(arrows[2]);
  
  if(digitalRead(upBtn) == LOW)
  {
    if(target_repeats < 10)
    {
      target_repeats++;
    }

    else
    {
      target_repeats = 1;
    }
  }

  else if(digitalRead(downBtn) == LOW)
  {
    if(target_repeats > 1)
    {
      target_repeats--;
    }

    else
    {
      target_repeats = 10;
    }
  }

  else if(digitalRead(ctrlBtn) == LOW)
  {
    lcd.clear();
    page = 4;
  }
}

void alarm_started_banner(void)
{
  alarm_on = true;
  lcd.setCursor(3, 0);
  lcd.print("ALARM SET!");
  lcd.setCursor(6, 1);
  lcd.print(">OK<");
  
  if(digitalRead(ctrlBtn) == LOW)
  {
    alarm_repeats = 0;
    lcd.clear();
    page = 1;
  }
}

void convert_alarm(void)
{  
  if(format == 0)
  {
    if(target_ap == 0)
    {
      if(target_h2 == 1 && target_h1 == 2)
      {
        target_h2 = 0;
        target_h1 = 0;
      }
    }
  
    else if(target_ap == 1)
    {
      if(target_h2 == 0)
      {
        if(target_h1 < 8)
        {
          target_h1 = target_h1 + 2;
          target_h2 = 1;
        }
  
        else if(target_h1 <= 9)
        {
          target_h1 = target_h1 - 8;
          target_h2 = 2;
        }
      }
   
      else if(target_h2 == 1)
      {
        if(target_h1 < 2)
        {
          target_h1 = target_h1 + 2;
          target_h2 = 2;
        }
      }
    }
  }

  else if(format == 1)
  { 
    if(target_ap == 0)
    {
      if(target_h2 == 0 && target_h1 == 0)
      {
        target_h2 = 1;
        target_h1 = 2;
      }
    }
  
    else if(target_ap == 1)
    {
      if(target_h2 == 1 && target_h1 != 2)
      {
        target_h1 = target_h1 - 2;
        target_h2 = 0;
      }
  
      else if(target_h2 == 2)
      {
        if(target_h1 < 2)
        {
          target_h1 = target_h1 + 8;
          target_h2 = 0;  
        }
  
        else if(target_h1 <= 3)
        {
          target_h1 = target_h1 - 2;
          target_h2 = 1;
        }
      }
    }
  }
}

void check_alarm(void)
{
  short Hours = (h2 * 10) + h1;
  short Minutes = (m2 * 10) + m1;
  short alarm_hours = (target_h2 * 10) + target_h1;
  short alarm_minutes = (target_m2 * 10) + target_m1;
  
  if(ap == target_ap && Hours == alarm_hours && Minutes == alarm_minutes)
  {
    lcd.clear();
    page = 5;
  }
}

void alarm_done(void)
{
//  buzz();
  lcd.setCursor(1, 0);
  lcd.print("IT IS");
  lcd.setCursor(7, 0);
  lcd.print(h2);
  lcd.setCursor(8, 0);
  lcd.print(h1);
  lcd.setCursor(9, 0);
  lcd.print(":");
  lcd.setCursor(10, 0);
  lcd.print(m2);
  lcd.setCursor(11, 0);
  lcd.print(m1);
  if(format == 0)
  {
    lcd.setCursor(12, 0);
    lcd.print(am_pm[ap]);
  }
  lcd.setCursor(14, 0);
  lcd.print("!");
  lcd.setCursor(6, 1);
  lcd.print(">OK<");
  
  if(digitalRead(ctrlBtn) == LOW)
  {
    alarm_repeats++;
    lcd.clear();
    page = 1;   
  }
}

void move_arrow(String Direction)
{
  lcd.setCursor(colAdjustmentArrow, 1);
  lcd.print(" ");
  
  if(Direction == "RIGHT")
  {
    if(colAdjustmentArrow == 12)
    {
      colAdjustmentArrow = 4;
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
    if(colAdjustmentArrow == 4)
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
