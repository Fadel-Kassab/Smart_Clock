 //Smart Clock
//Project By Fadel Kassab
//Grapotronics

#include <Wire.h>      
#include <LiquidCrystal_I2C.h>
#include <dht11.h>
#define DHT11PIN 8      
dht11 DHT11;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int page = 1;
int downBtn = 6;
int lBtn = 2;
int rBtn = 3;
int upBtn = 5;
int ctrlBtn = 4;

int m1 = 0;   // Minute vars
int m2 = 0;
int h1 = 2;   // Hour vars AM||PM
int h2 = 1;
int hr1 = 0;  // Hour vars 24-Cycle
int hr2 = 0;
bool change_to_pm = false;
bool change_to_am = false;

int format = 0; // AM||PM = 0 & 24-Cycle = 1
int ap = 0;  // AM||PM var AM --> 0 ; PM --> 1
String amPm[] = {"AM", "PM"};

unsigned long previousBtn = 0;
unsigned long eventBtn = 200;
unsigned long eventClock = 200; // 60000
unsigned long previousClock = 0;

/* phrases page 1 */

int ranChar = random(0, 4);
int ranPrefChar = random(0, 4);
int ranWord = random(0, 4);
bool chooseChar = true;
bool defualtPhrases = true;
String userPrefPhrase = "SDC"; // this var is set according to the user's preference
String words[] = {"CHILL", "RELAX", "LOVE", "LEARN"};

/* Style adjustment */
int rowClock = 0;
int colClock = 0;

int rowPhrase = 1;
int colPhrase = 0;

int rowTmp = 0;
int colTmp = 9;

int rowHmd = 1;
int colHmd = 9;

/* Menu arrow */
int rowMenuArrow = 0;
int colMenuArrow = 0;

/* Time adjustment */

int setMin1 = 0;
int setMin2 = 0;
int setHour1 = 2;
int setHour2 = 1;
int formatSetupVar = 0;
int columnSetup = 4;
bool changed = false;
bool cyc24Setup = false;
String formatSetup [] = {"AM", "PM", "h/24"};
String arrowArray [] = {"^", ">"};
bool adjusted_to_pm = false;

/* Style choose */
int option = 0;
bool scrolled = false;

bool addHmd = true;
bool addPhrase = true;
bool addClock = true;
bool addTmp = true;

bool initialHmd = true;
bool initialTmp = true;
bool initialClock = true;
bool initialPhrase = true;
bool stored_values = false;

/* Style set  */
int colClockStyle = 0;
int rowClockStyle = 0;

int colTmpStyle = 0;
int rowTmpStyle = 0;

int colHmdStyle = 0;
int rowHmdStyle = 0;

int colPhraseStyle = 0;
int rowPhraseStyle = 0;

bool defualtStyle;
int colPhraseFull;

int longestInArr = 5; // length of longest phrase in array
int prefPhraseLen = userPrefPhrase.length();

String remAll = "       ";

/* Banners */
int colErrorArrow = 4;
String errorArrows[] = {"<", ">"};
int errorArrow = 0;


/* Bytes */
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
byte tick[] = {
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
  B00000,
  B00000,
  B00000
};

void setup(void)
{
  lcd.begin(16, 2);
  pinMode(ctrlBtn, INPUT_PULLUP);
  pinMode(lBtn, INPUT_PULLUP);
  pinMode(rBtn, INPUT_PULLUP);
  pinMode(upBtn, INPUT_PULLUP);
  pinMode(downBtn, INPUT_PULLUP);
  pinMode(8, INPUT);

  lcd.createChar(0, heart);
  lcd.createChar(1, music);
  lcd.createChar(2, iceBall);
  lcd.createChar(3, tree);
  lcd.createChar(4, tick);

}

void loop(void)
{
  unsigned long current = millis();
  am_pm_tracker();
  choose_phrase();

  /* Format */
  if(current - previousClock == eventClock)
  { 
    if(format == 0)
    {
      am_pm();
      to_24_hour();
    }

    else
    {
      cycle24();  
      to_am_pm();
    }
    
    previousClock = current;  
    
  }
    
  if(current - previousBtn == eventBtn)
  {
    /* Navigation */
    if(digitalRead(rBtn) == LOW && page < 5)
    {
      page++;
      lcd.clear();
    }
    else if(digitalRead(lBtn) == LOW && page < 5)
    {
      page--;
      lcd.clear();
    }

    /* Quote page */
    if(page == 0)
    {
      
    }

    /* Main page */  
    else if(page == 1)
    {
      tempHmdSens();
      if(addClock == true)
      {
        lcd.setCursor(colClock + 2, rowClock);
        lcd.print(":"); 
        switch(format)
        {
          case 0:
            lcd.setCursor(colClock + 5, rowClock);
            lcd.print(amPm[ap]);
            lcd.setCursor(colClock + 4, rowClock);
            lcd.print(m1);
            lcd.setCursor(colClock + 3, rowClock);
            lcd.print(m2);
            lcd.setCursor(colClock + 1, rowClock);
            lcd.print(h1);
            lcd.setCursor(colClock, rowClock);
            lcd.print(h2);
            break;
              
          case 1:
            lcd.setCursor(colClock + 4, rowClock);
            lcd.print(m1);
            lcd.setCursor(colClock + 3, rowClock);
            lcd.print(m2);
            lcd.setCursor(colClock + 1, rowClock);
            lcd.print(hr1);
            lcd.setCursor(colClock, rowClock);
            lcd.print(hr2);
            break;
        }
      
        if(digitalRead(ctrlBtn) == LOW)
        {
          erase_2(colClock + 5, rowClock);
          
          switch(format)
          {
            case 0:
              format = 1;
              break;
              
            case 1:
              format = 0;
              break;
          }
        }
      }

      if(addPhrase == true)
      {
        if(defualtPhrases == true)
        {
          lcd.setCursor(colPhrase, rowPhrase);
          lcd.print(words[ranWord]);
          lcd.setCursor(colPhrase + words[ranWord].length(), rowPhrase);
          lcd.write(byte(ranChar));
        }
  
        else
        {
          lcd.setCursor(colPhrase, rowPhrase);
          lcd.write(byte(ranChar));
          lcd.setCursor(colPhrase + 1, rowPhrase);
          lcd.print(userPrefPhrase);
          lcd.setCursor(colPhrase + userPrefPhrase.length() + 1, rowPhrase);
          lcd.write(byte(ranChar));
        }

        if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          if(defualtPhrases == true)
          {
            erase_2(colPhrase + words[ranWord].length(), rowPhrase);
            defualtPhrases = false;
          }
  
          else
          {
            defualtPhrases = true;
          }
        }
      }
    }

    /* Alarm & Stopwatch menu */  
    else if(page == 2)
    {
      lcd.setCursor(3, 0);
      lcd.print("Alarm");
      lcd.setCursor(3, 1);
      lcd.print("Stopwatch");
    }
    
    /* Settings menu */  
    else if(page == 3)
    {
      lcd.setCursor(3, 0);
      lcd.print("SETTINGS");
      lcd.setCursor(3, 1);
      lcd.print("CREDITS");
      lcd.setCursor(colMenuArrow, rowMenuArrow);
      lcd.print("->");

      if(digitalRead(downBtn) == LOW || digitalRead(upBtn) == LOW) 
      {
        erase_2(colMenuArrow, rowMenuArrow);

        if(rowMenuArrow == 0)
        {
          rowMenuArrow = 1;
        }

        else
        {
          rowMenuArrow = 0;
        }
      }

      else if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        if(rowMenuArrow == 0)
        {
          page = 5; // Adjustment Menu
          colMenuArrow = 12;
        }

        else
        {
          page = 9; // Credits
        }
        
        rowMenuArrow = 0;
      }
    }
    
    /* Rolling through pages */  
    else if(page == -1)
    {
      page = 3;
    }  
    else if(page == 4)
    {
      page = 0;
    }

    /* not for user pages */
    //Adjustment Menu
    else if(page == 5)
    {
      lcd.setCursor(0, 0);
      lcd.print("ADJUST TIME");
      lcd.setCursor(0, 1);
      lcd.print("STYLE");
      lcd.setCursor(colMenuArrow, rowMenuArrow);
      lcd.print("<-");
      
      if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
      {
        if(rowMenuArrow == 0)
        {
          erase_2(colMenuArrow, rowMenuArrow);
          rowMenuArrow = 1;
          colMenuArrow = 6;
        }

        else
        {
          erase_2(colMenuArrow, rowMenuArrow);
          rowMenuArrow = 0;
          colMenuArrow = 12;
        }
      }

      else if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        if(rowMenuArrow == 0)
        {
          page = 6; // AdjustTime
        }

        else
        {
          page = 7; // Style menu
          rowMenuArrow = 0;
          colMenuArrow = 10;
        }
      }

      else if(digitalRead(rBtn) == LOW || digitalRead(lBtn) == LOW)
      {
        lcd.clear();
        page = 3;
        rowMenuArrow = 0;
        colMenuArrow = 0;
      }
    } 

    /* AdjustTime page */
    else if(page == 6)
    {
      if(formatSetupVar == 2 && changed == true)
      {
        setHour1 = 0;
        setHour2 = 0;
        changed = false;
      }
      
      else if(formatSetupVar == 1 && changed == true)
      {
        erase_2(11, 0);
        setHour1 = 2;
        setHour2 = 1;
        changed = false;
      }
      
      else if(formatSetupVar == 0 && changed == true)
      {
        erase_2(11, 0);
        setHour1 = 2;
        setHour2 = 1;
        changed = false;
      }
      
      lcd.setCursor(4, 0);
      lcd.print(setHour2);
      lcd.setCursor(5, 0);
      lcd.print(setHour1);
      lcd.setCursor(6, 0);
      lcd.print(":");
      lcd.setCursor(7, 0);
      lcd.print(setMin2);
      lcd.setCursor(8, 0);
      lcd.print(setMin1);
      lcd.setCursor(9, 0);
      lcd.print(formatSetup[formatSetupVar]);
      lcd.setCursor(12, 1);
      lcd.print("Done");
    
      if(columnSetup != 11)
      {
        lcd.setCursor(columnSetup, 1);
        lcd.print(arrowArray[0]);
      }
    
      else
      {
        lcd.setCursor(columnSetup, 1);
        lcd.print(arrowArray[1]);
      }

      if(digitalRead(rBtn) == LOW)
      {
        erase_2(columnSetup - 1, 1);
        
        switch(columnSetup)
        {
          case 11:
            columnSetup = 4;
            break;
  
          case 9:
            columnSetup = 11;
            break;
  
          case 5:
            columnSetup = columnSetup + 2;
            break;
  
          default:
            columnSetup++;
            break;
        }
      } 

      else if(digitalRead(lBtn) == LOW)
      {
        erase_2(columnSetup - 1, 1);
        switch(columnSetup)
        {
          case 11:
            columnSetup = 9;
            break;

          case 7:
            columnSetup = columnSetup - 2;
            break;

          case 4:
            columnSetup = 11;
            break;
  
          default:
            columnSetup--;
            break;
         }
       }

      else if(digitalRead(upBtn) == LOW) // Up Button
      {
        switch(columnSetup)
        {
          case 4:
            //24H case
            if(cyc24Setup == true)
            { 
              if(setHour2 == 2)
              {
                setHour2 = 0;
              }
  
              else if(setHour2 == 1 && setHour1 <= 3)
              {
                setHour2++;
              }
  
              else if(setHour2 == 0)
              {
                setHour2++;
              }
            }
            
            //AM||PM case
            else
            {
              if(setHour2 == 1 && setHour1 != 0)
              {
                setHour2 = 0;
              }
              
              else if(setHour2 == 0 && setHour1 <= 2)
              {
                setHour2 = 1;
              }
            }
            break;
            
          case 5:
            if(cyc24Setup == true)
            {
              if(setHour2 == 2 && setHour1 == 3)
              {
                setHour1 = 0;
                setHour2 = 0; 
              }
  
              else if(setHour2 == 2 && setHour1 < 3)
              {
                setHour1++;
              }
  
              else if(setHour2 == 1 && setHour1 == 9)
              {
                setHour2 = 2;
                setHour1 = 0;
              }
  
              else if(setHour2 == 1 && setHour1 < 9)
              {
                setHour1++;
              }
  
              else if(setHour2 == 0 && setHour1 == 9)
              {
                setHour1 = 0;
                setHour2 = 1;
              }
  
              else if(setHour2 == 0 && setHour1 < 9)
              {
                setHour1++;
              }
            }
  
            else
            {
              if(setHour2 == 0 && setHour1 == 9)
              {
                setHour2 = 1;
                setHour1 = 0;
              }
              
              else if(setHour2 == 1 && setHour1 == 2)
              {
                setHour2 = 0;
                setHour1 = 1;
              }
              
              else if(setHour2 == 1 && setHour1 > 2)
              {
                setHour2 = 1;
              }
              
              else if(setHour2 == 1 && setHour1 < 2)
              {
                setHour1++;
              }
              
              else if(setHour2 == 0 && setHour1 < 9)
              {
                setHour1++;
              }
            }
          break;
  
        case 7:
          if(setMin2 == 5)
          {
            if(setMin1 == 9)
            {
              setMin1 = 0;
            }
      
            setMin2 = 0;
          }
          
          else
          {
            setMin2++;
          }
          break;
  
        case 8:
          if(setMin1 == 9 && setMin2 < 5)
          {
            setMin1 = 0;
            setMin2++;
          }
          
          else if(setMin1 == 9 && setMin2 == 5)
          {
            setMin1 = 0;
            setMin2 = 0;
          }
          
          else
          {
            setMin1++;
          }
          break;
  
        case 9:
          if(formatSetupVar == 2)
          {
            formatSetupVar = 0;
            cyc24Setup = false;
            changed = true;
          }
          
          else if(formatSetupVar == 1)
          {
            formatSetupVar++;
            cyc24Setup = true;
            changed = true;
          }
          
          else
          {
            formatSetupVar++;
          }
          break;
        }   
       }

      else if(digitalRead(downBtn) == LOW)
      {
      switch(columnSetup)
      {
        case 4:
          if(cyc24Setup == true)
          {
            if(setHour2 == 2 || setHour2 == 1)
            {
              setHour2--;
            }
  
            else if(setHour2 == 0 && setHour1 <= 3)
            {
              setHour2 = 2;
            }
          }
  
          else
          {
            if(setHour2 == 0 && setHour1 <= 2)
            {
              setHour2 = 1;
            }
            
            else if(setHour2 == 1 && setHour1 != 0)
            {
              setHour2 = 0;
            }
          }
          break;
  
        case 5:
          if(cyc24Setup == true)
          {
            if(setHour2 == 0 && setHour1 == 0)
            {
              setHour2 = 2;
              setHour1 = 3;
            }
  
            else if(setHour2 == 2 && setHour1 > 0)
            {
              setHour1--;
            }
  
            else if(setHour2 == 2 && setHour1 == 0)
            {
              setHour2 = 1;
              setHour1 = 9;
            }
  
            else if(setHour2 == 1 && setHour1 > 0)
            {
              setHour1--;
            }
  
            else if(setHour2 == 1 && setHour1 == 0)
            {
              setHour2 = 0;
              setHour1 = 9;
            }
  
            else if(setHour2 == 0 && setHour1 > 0)
            {
              setHour1--;
            }
          }
  
          else
          {
            if(setHour1 == 1 && setHour2 == 0)
            {
              setHour1 = 2;
              setHour2 = 1;
            }
  
            else if(setHour2 == 0 && setHour1 > 1)
            {
              setHour1--;
            }
  
            else if(setHour2 == 1 && setHour1 > 0)
            {
              setHour1--;
            }
  
            else if(setHour2 == 1 && setHour1 == 0)
            {
              setHour1 = 9;
              setHour2 = 0;
            }
          }
          break;
  
        case 7:
          if(setMin2 == 0)
          {
            if(setMin1 == 0)
            {
              setMin1 = 9;  
            }
            
            setMin2 = 5;
          }
          else
          {
            setMin2--;
          }
          break;
  
        case 8:
          if(setMin2 == 0 && setMin1 == 0)
          {
            setMin2 = 5;
            setMin1 = 9;
          }
          else if(setMin2 > 0 && setMin1 == 0)
          {
            setMin1 = 9;
            setMin2--;
          }
          else
          {
            setMin1--;
          }
          break;
  
        case 9:
          if(formatSetupVar == 0)
          {
            formatSetupVar = 2;
            changed = true;
            cyc24Setup = true;
          }
          else if(formatSetupVar == 2)
          {
            formatSetupVar = 1;
            changed = true;
            cyc24Setup = false;
          }
          else if(formatSetupVar == 1)
          {
            formatSetupVar = 0;
          }
          break;
        }
      }

      else if(digitalRead(ctrlBtn) == LOW)
      {
        if(columnSetup == 11)
        {
          m1 = setMin1;
          m2 = setMin2;

          if(formatSetupVar == 0)
          {
            ap = 0;
            format = 0;
            change_to_pm = false;
            h1 = setHour1;
            h2 = setHour2;
          }

          else if(formatSetupVar == 1)
          {
            ap = 1;
            adjusted_to_pm = true;
        
            format = 0;
            h1 = setHour1;
            h2 = setHour2;
            if(h1 != 2 && h2 != 1)
            {
              change_to_am = true;
            }
            
          }

          else
          {
            format = 1;
            hr1 = setHour1;
            hr2 = setHour2;

            if(hr2 == 2 || (hr2 == 1 && hr1 >= 3))
            {
              ap = 1;
            }

            else
            {
              ap = 0;
            }
          }
   
          lcd.clear();
          setMin1 = 0;
          setMin2 = 0;
          setHour1 = 2;
          setHour2 = 1;
          formatSetupVar = 0;
          columnSetup = 4;
          changed = false;
          cyc24Setup = false;
          page = 1;
          colMenuArrow = 0;
          rowMenuArrow = 0;
          m1--;
          if(format == 0)
          {
            am_pm();
            to_24_hour();
          }
          
          else
          {
            cycle24();  
            if(hr1 == 0 && hr2 == 0)
            {
              ap = 0;
              change_to_pm = false;
            }

            else if(hr1 == 2 && hr2 == 1)
            {
              ap = 1;
              change_to_am = false;
            }
            to_am_pm();
          }
        }
      }
    }

    /* Style menu */
    else if(page == 7)
    {
      lcd.setCursor(0, 0);
      lcd.print("CUSTOMIZE");
      lcd.setCursor(0, 1);
      lcd.print("DEFAULT");
      lcd.setCursor(colMenuArrow, rowMenuArrow);
      lcd.print("<-");

      if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
      {
        erase_2(colMenuArrow, rowMenuArrow);
        if(rowMenuArrow == 0)
        {
          colMenuArrow = 8;
          rowMenuArrow = 1;
        }

        else
        {
          colMenuArrow = 10;
          rowMenuArrow = 0;
        }
      }

      else if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        if(rowMenuArrow == 0)
        {
          page = 8;
        }

        else
        {
          addClock = true;
          addPhrase = true;
          addTmp = true;
          addHmd = true;
          rowClock = 0;
          colClock = 0;
          rowPhrase = 1;
          colPhrase = 0;
          rowTmp = 0;
          colTmp = 9;
          rowHmd = 1;
          colHmd = 9;
          page = 1;
          rowMenuArrow = 0;
          colMenuArrow = 0;
        }
      }

      else if(digitalRead(rBtn) == LOW || digitalRead(lBtn) == LOW)
      {
        lcd.clear();
        page = 5;
        rowMenuArrow = 0;
        colMenuArrow = 12;
      }
    }
    
    /* Custom style select page */
    else if(page == 8)
    {
      store_initial_state();
      
      if(scrolled == true)
      {
        lcd.setCursor(0, 0);
        lcd.print("HUMID");
        lcd.setCursor(9, 0);
        lcd.print("TEMP");
        lcd.setCursor(0, 1);
        lcd.print("EXIT");
        lcd.setCursor(9, 1);
        lcd.print("NEXT");
        
        if(addHmd == true)
        {
          lcd.setCursor(5, 0);
          lcd.write(byte(4));
        }
        else
        {
          lcd.setCursor(5, 0);
          lcd.print(" ");
        }
        
        if(addTmp == true)
        {
          lcd.setCursor(13, 0);
          lcd.write(byte(4));
        }
        else
        {
          lcd.setCursor(13, 0);
          lcd.print(" ");
        }
        
        switch(option)
        {
          case 2:
          lcd.setCursor(6, 0);
            break;
  
          case 3:
          lcd.setCursor(14, 0);
            break;
  
          case 4:
          lcd.setCursor(4, 1);
            break;
  
          case 5:
          lcd.setCursor(13, 1);
            break;
        }
        lcd.print("<");
       
       if(digitalRead(lBtn) == LOW || digitalRead(rBtn) == LOW)
       {
        switch(option)
        {
          case 2:
            lcd.setCursor(6, 0);
            lcd.print(" ");
            option = 3;
            break;
  
          case 3:
            lcd.setCursor(14, 0);
            lcd.print(" ");
            option = 2;
            break;
  
          case 4:
            lcd.setCursor(4, 1);
            lcd.print(" ");
            option = 5;
            break;
  
          case 5:
            lcd.setCursor(13, 1);
            lcd.print(" ");
            option = 4;
            break;
        }
       }
  
       else if(digitalRead(upBtn) == LOW)
       {
          switch(option)
          {
            case 2:
              scrolled = false;
              option = 0;
              lcd.clear();
              break;
  
            case 3:
              scrolled = false;
              option = 1;
              lcd.clear();
              break;
  
            case 4:
              lcd.setCursor(4, 1);
              lcd.print(" ");
              option = 2;
              break;
  
            case 5:
              lcd.setCursor(13, 1);
              lcd.print(" ");
              option = 3;
              break;
          }
       }
  
       else if(digitalRead(downBtn) == LOW)
       {
          switch(option)
          {
            case 2:
              lcd.setCursor(6, 0);
              lcd.print(" ");
              option = 4;
              break;
  
            case 3:
              lcd.setCursor(14, 0);
              lcd.print(" ");
              option = 5;
              break;
  
            case 4:
              scrolled = false;
              option = 0;
              lcd.clear();
              break;
  
            case 5:
              scrolled = false;
              option = 1;
              lcd.clear();
              break;
          }
       }
  
      else if(digitalRead(ctrlBtn) == LOW)
      {
        switch(option)
        {
          case 2:
            if(addHmd == false)
            {
              addHmd = true;
            }

            else
            {
              addHmd = false;
            }
            break;
            
          case 3:
            if(addTmp == true)
            {
              addTmp = false;
            }

            else
            {
              addTmp = true;
            }
            break;

          case 4:
            style_menu_to_home();
            break;
          
          case 5:
            customize();
            break;
        }
      }
    }
  
       else if(scrolled == false)
       {
        lcd.setCursor(0, 0);
        lcd.print("CLOCK");
        lcd.setCursor(0, 1);
        lcd.print("HUMID");
        lcd.setCursor(9, 0);
        lcd.print("QUOTE");
        lcd.setCursor(9, 1);
        lcd.print("TEMP");
  
        if(addClock == true)
        {
          lcd.setCursor(5, 0);
          lcd.write(byte(4));
        }
        else
        {
          lcd.setCursor(5, 0);
          lcd.print(" ");
        }
        
        if(addHmd == true)
        {
          lcd.setCursor(5, 1);
          lcd.write(byte(4));  
        }
        else
        {
          lcd.setCursor(5, 1);
          lcd.print(" ");
        }
        
        if(addPhrase == true)
        {
          lcd.setCursor(14, 0);
          lcd.write(byte(4)); 
        }
        else
        {
          lcd.setCursor(14, 0);
          lcd.print(" ");
        }
        
        if(addTmp == true)
        {
          lcd.setCursor(13, 1);
          lcd.write(byte(4));
        }
        else
        {
          lcd.setCursor(13, 1);
          lcd.print(" ");
        }
  
        switch(option)
        {
          case 0:
            lcd.setCursor(6, 0);
            break;
            
          case 1:
            lcd.setCursor(15, 0);
            break;
            
          case 2:
            lcd.setCursor(6, 1);
            break;
            
          case 3:
            lcd.setCursor(14, 1);
            break; 
        }
        lcd.print("<");
       
        if(digitalRead(upBtn) == LOW)
        {
          switch(option)
          {
            case 0:
              scrolled = true;
              option = 4;
              lcd.clear();
              break;
              
            case 1:
              scrolled = true;
              option = 5;
              lcd.clear();
              break;
              
            case 2:
              lcd.setCursor(6, 1);
              option = 0;
              break;
              
            case 3:
              lcd.setCursor(14, 1);
              option = 1;
              break;
          }
          lcd.print(" ");
        }
  
        else if(digitalRead(downBtn) == LOW)
        {
          switch(option)
          {
            case 0:
              lcd.setCursor(6, 0);
              option = 2;
              break;
  
            case 1:
              lcd.setCursor(15, 0);
              option = 3;
              break;
  
            case 2:
              scrolled = true;
              option = 4;
              lcd.clear();
              break;
  
            case 3:
              scrolled = true;
              option = 5;
              lcd.clear();
              break;
          }
          lcd.print(" ");
        }

        if(digitalRead(rBtn) == LOW || digitalRead(lBtn) == LOW)
        {
          switch(option)
          {
            case 0:
              lcd.setCursor(6, 0);
              option = 1;
              break;
              
            case 1:
              lcd.setCursor(15, 0);
              option = 0;
              break;
              
            case 2:
              lcd.setCursor(6, 1);
              option = 3;
              break;
              
            case 3:
              lcd.setCursor(14, 1);
              option = 2;
              break;
          }
          lcd.print(" "); 
        }

       else if(digitalRead(ctrlBtn) == LOW)
        {
          switch(option)
          {
            case 0:
              if(addClock == true)
              {
                addClock = false;
              }

              else
              {
                addClock = true;
              }
              break;
              
            case 1:
              if(addPhrase == true)
              {
                addPhrase = false;
              }
  
              else
              {
                addPhrase = true;
              }
              break;

            case 2:
              if(addHmd == true)
              {
                addHmd = false;
              }

              else
              {
                addHmd = true;
              }
              break;

            case 3:
              if(addTmp == true)
              {
                addTmp == false;
              }
              else
              {
                addTmp = true;
              }
              break;
          }
        }
      }
    }
    
    /* Phrase set page (style)*/
    else if(page == 9)
    {
      
      if(addPhrase == true)
      {
        int longestPhraseLen = longest_phrase_len();
        
        if(defualtStyle == true)
        {
          colPhraseFull = longestPhraseLen + colPhraseStyle;
          lcd.setCursor(colPhraseStyle, rowPhraseStyle);
          lcd.print(words[0]);
          lcd.setCursor(colPhraseFull, rowPhraseStyle);
          lcd.write(byte(ranChar));  
        }
        
        else
        {
          colPhraseFull = longestPhraseLen + colPhraseStyle + 1;
          lcd.setCursor(colPhraseStyle, rowPhraseStyle);
          lcd.write(byte(ranChar));
          lcd.setCursor(colPhraseStyle + 1, rowPhraseStyle);
          lcd.print(userPrefPhrase);
          lcd.setCursor(colPhraseFull, rowPhraseStyle);
          lcd.write(byte(ranChar));
        }
        
        if(digitalRead(rBtn) == LOW)
        {
          move_right_oneWid(colPhraseFull, colPhraseStyle, rowPhraseStyle);         
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_oneWid(colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_oneWid(colPhraseStyle, rowPhraseStyle, true);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          if(addClock == true)
          {
            smart_locating_oneWid(rowPhraseStyle, rowClockStyle);
            page = 10;
          }

          else if(addTmp == true)
          {
            smart_locating_oneWid(rowPhraseStyle, rowTmpStyle);
            page = 11;
          }

          else if(addHmd == true)
          {
            smart_locating_oneWid(rowPhraseStyle, rowHmdStyle);
            page = 12;
          }

          else
          {
            style_to_home();
          }
        }
      }

      else
      {
        if(addClock == true)
        {
          page = 10;
        }

        else if(addTmp == true)
        {
          page = 11;
        }

        else if(addHmd == true)
        {
          page = 12;
        }

        else
        {
          style_to_home();
        }
      }
    }
    
    /* Clock set page (style) */
    else if(page == 10)
    {
      int colClockFull = colClockStyle + 6;
      lcd.setCursor(colClockStyle, rowClockStyle);
      lcd.print("12:00AM");

      if(addPhrase == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_twoWid(colClockStyle + 6, colClockStyle, rowClockStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }
        
        else if(digitalRead(lBtn) == LOW)
        {
          move_left_twoWid(colClockStyle + 6, colClockStyle, rowClockStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_twoWid(colClockStyle + 6, colClockStyle, rowClockStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          if(addTmp == true)
          {
            page = 11;
            smart_locating_twoWid(colClockStyle + 6, colClockStyle, rowClockStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colTmpStyle, rowTmpStyle);
          }

          else if(addHmd == true)
          {
            page = 12;
            smart_locating_twoWid(colClockStyle + 6, colClockStyle, rowClockStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colHmdStyle, rowHmdStyle);     
          }

          else
          {
            style_to_home();
          }
        }
      }

      else
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_oneWid(colClockFull, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_oneWid(colClockFull, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_oneWid(colClockStyle, rowClockStyle, false);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          if(addTmp == true)
          {
            smart_locating_oneWid(rowClockStyle, rowTmpStyle);
            page = 11;
          }

          else if(addHmd == true)
          {
            smart_locating_oneWid(rowClockStyle, rowHmdStyle);
            page = 12;
          }
          
          else
          {
            style_to_home();
          }
        }
      }
    }
    
    /* Tmp set page (style) */
    else if(page == 11)
    {
      int colTmpFull = colTmpStyle + 6;
      lcd.setCursor(colTmpStyle, rowTmpStyle);
      lcd.print("Tmp:0 C");

      if(addPhrase == true && addClock == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_threeWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_threeWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }
    
        else if(digitalRead(downBtn) == LOW || digitalRead(upBtn) == LOW)
        {
          move_up_down_threeWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }
            
        else if(digitalRead(ctrlBtn) == LOW)
        {
          if(addHmd == true)
          {
            page = 12;
            smart_locating_threeWid(colPhraseFull, colPhraseStyle, rowPhraseStyle, colClockStyle + 6, colClockStyle, rowClockStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle, colHmdStyle, rowHmdStyle);
          }

          else
          {
            style_to_home();
          }
        }
      }

      else if(addClock == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_twoWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_twoWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_twoWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          if(addHmd == true)
          {
            page = 12;
            smart_locating_twoWid(colClockStyle + 6, colClockStyle, rowClockStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle, colHmdStyle, rowHmdStyle);   
          }

          else
          {
            style_to_home();
          }
        }
      }
      
      else if(addPhrase == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_twoWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_twoWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_twoWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          if(addHmd == true)
          {
            page = 12;
            smart_locating_twoWid(colTmpStyle + 6, colTmpStyle, rowTmpStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colHmdStyle, rowHmdStyle);   
          }

          else
          {
            style_to_home();
          }
        }
      }

      else
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_oneWid(colTmpFull, colTmpStyle, rowTmpStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_oneWid(colTmpFull, colTmpStyle, rowTmpStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_oneWid(colTmpStyle, rowTmpStyle, false);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          if(addHmd == true)
          {
            page = 12;
            smart_locating_oneWid(rowTmpStyle, rowHmdStyle);
          }

          else
          {
            style_to_home();
          }
        }
      }
    }
    
    /* Hmd set page (style) */
    else if(page == 12)
    {
      int colHmdFull = colHmdStyle + 6;
      lcd.setCursor(colHmdStyle, rowHmdStyle);
      lcd.print("Hmd:0 %");

      if(addPhrase == true && addClock == true && addTmp == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_fourWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colClockStyle + 6, colClockStyle, rowClockStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_fourWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colClockStyle + 6, colClockStyle, rowClockStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle);        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          style_to_home();
        }
      }

      else if(addPhrase == true && addClock == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_threeWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_threeWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_threeWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          style_to_home();
        }
      }

      else if(addPhrase == true && addTmp == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_threeWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_threeWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_threeWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          style_to_home();
        }
      }

      else if(addClock == true && addTmp == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_threeWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_threeWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle, colClockStyle + 6, colClockStyle, rowClockStyle); 
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_threeWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          style_to_home();
        }
      }

      else if(addClock == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_twoWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_twoWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_twoWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colClockStyle + 6, colClockStyle, rowClockStyle);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          style_to_home();
        }
      }

      else if(addTmp == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_twoWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_twoWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_twoWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colTmpStyle + 6, colTmpStyle, rowTmpStyle);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          style_to_home();
        }
      }

      else if(addPhrase == true)
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_twoWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_twoWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_twoWid(colHmdStyle + 6, colHmdStyle, rowHmdStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          style_to_home();
        }
      }

      else
      {
        if(digitalRead(rBtn) == LOW)
        {
          move_right_oneWid(colHmdFull, colHmdStyle, rowHmdStyle);
        }

        else if(digitalRead(lBtn) == LOW)
        {
          move_left_oneWid(colHmdFull, colHmdStyle, rowHmdStyle);
        }

        else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
        {
          move_up_down_oneWid(colHmdStyle, rowHmdStyle, false);
        }

        else if(digitalRead(ctrlBtn) == LOW)
        {
          style_to_home();
        }
      }
    }
    
    /* errors */
    else if(page == 20)
    {
      error_no_space();
    }
    
    previousBtn = current;
  }
}

void cycle24(void)
{
  if(m2 == 5 && m1 == 9)
  {
    m2 = 0;
    m1 = 0;
    hr1++; 
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
  
  if(hr1 == 4 && hr2 == 2)
  {
    hr1 = 0;
    hr2 = 0;  
  }
  
  else if(hr1 == 10)
  {
    hr1 = 0;
    hr2++;
  }
}

void am_pm(void)
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

void to_am_pm(void)
{ 
  if(hr1 == 0 && hr2 == 0)
  {
    h1 = 2;
    h2 = 1;
  }

  else if((hr2 == 0 && hr1 >= 1) || (hr2 == 1 && hr1 < 3))
  {
    h1 = hr1;
    h2 = hr2; 
  }

  else if(hr2 == 1 && hr1 >= 3 && hr1 <= 9)
  {
    h1 = hr1 - 2;
    h2 = hr2 - 1;
  }

  else if(hr2 == 2 && hr1 >= 0 && hr1 <= 1)
  {
    h1 = hr1 + 8;
    h2 = hr2 - 2;
  }

  else if(hr2 == 2 && hr1 >= 2 && hr1 <= 3)
  {
    h1 = hr1 - 2;
    h2 = hr2 - 1;
  }
}

void to_24_hour(void)
{
  if(ap == 0)
  {
    if(h1 == 2 && h2 == 1 && change_to_pm == false)
    {
      hr1 = 0;
      hr2 = 0;
    }

    else 
    {
      hr1 = h1;
      hr2 = h2;
      change_to_pm = true;
    }
  }
  
  else if(ap == 1)
  {
    if(change_to_am == true && h1 == 2 && h2 == 1)
    {
      hr1 = 0;
      hr2 = 0;
      ap = 0;
      change_to_am = false;
    }
    
    else if(h1 == 2 && h2 == 1)
    {
      hr1 = h1;
      hr2 = h2;  
    }

    
    else if(h2 == 0 && h1 <= 7)
    {
      change_to_am = true;
      hr1 = h1 + 2;
      hr2 = 1; 
    }

    else if(h2 == 0 && h1 >= 8 && h1 <= 9)
    {
      hr1 = h1 - 8;
      hr2 = 2;
    }

    else if(h2 == 1 && h1 >= 0 && h1 <= 1)
    {
      hr1 = h1 + 2;
      hr2 = h2 + 1;
      change_to_pm = false; 
    }
  }
}

void am_pm_tracker(void)
{
  if(adjusted_to_pm == true)
  {
    ap = 1;
    adjusted_to_pm = false;
  }
   
  else if(hr2 == 1 && hr1 == 2)
  {
    ap = 1;
  }

  else if(hr2 == 0 && hr1 == 0)
  {
    ap = 0;
  }
}

void choose_phrase(void)
{
    if((chooseChar == true) && ((hr2 == 0 && hr1 == 3) || (hr2 == 0 && hr1 == 9) || (hr2 == 1 && hr1 == 2)))
  {
    ranChar = random(0, 4);
    ranPrefChar = random(0, 4);
    ranWord = random(0, 4);
    lcd.setCursor(words[ranWord].length() + 1, 1);
    lcd.print(" ");
    chooseChar = false;
  }

  
  if((hr2 == 0 && hr1 ==2) || (hr2 == 0 && hr1 == 8) || (hr2 == 1 && hr1 == 1))
  {
    chooseChar = true;
  } 
}

void tempHmdSens(void)
{ 
  int chk = DHT11.read(DHT11PIN);
  /* Temp */
  if(addTmp == true)
  {
    lcd.setCursor(colTmp, rowTmp);
    lcd.print("Tmp:");
    lcd.print((float)DHT11.temperature, 0);
    lcd.setCursor(colTmp + 6, rowTmp);
    lcd.print("C");
  }
  
  /* Humidity */
  if(addHmd == true)
  {
    lcd.setCursor(colHmd, rowHmd);
    lcd.print("Hmd:");
    lcd.print((float)DHT11.humidity, 0);
    lcd.setCursor(colHmd + 6, rowHmd);
    lcd.print("%");
  }
}

void erase_2(int x, int y)
{
  lcd.setCursor(x, y);
  lcd.print(" ");
  lcd.setCursor(x + 1, y);
  lcd.print(" ");
}

void erase_1(int x, int y)
{
  lcd.setCursor(x, y);
  lcd.print(" ");
}

void def_style(void)
{
  colTmpStyle = 0;
  rowTmpStyle = 0;
  colHmdStyle = 0;
  rowTmpStyle = 0;
  colClockStyle = 0;
  rowClockStyle = 0;
  colPhraseStyle = 0;
  rowPhraseStyle = 0;
}

void error_no_space(void)
{
  lcd.setCursor(1, 0);
  lcd.print("NO SPACE LEFT!");
  lcd.setCursor(0, 1);
  lcd.print("HOME");
  lcd.setCursor(12, 1);
  lcd.print("EDIT");
  lcd.setCursor(colErrorArrow, 1);
  lcd.print(errorArrows[errorArrow]);

  if(digitalRead(rBtn) == LOW || digitalRead(lBtn) == LOW)
  {
    if(colErrorArrow == 4)
    {
      erase_1(colErrorArrow, 1);
      errorArrow = 1;
      colErrorArrow = 11;
    }

    else
    {
      erase_1(colErrorArrow, 1);
      errorArrow = 0;
      colErrorArrow = 4;
    }
  }

  else if(digitalRead(ctrlBtn) == LOW)
  {
    if(colErrorArrow == 4)
    {
      lcd.clear();
      page = 1;
      rowMenuArrow = 0;
      colMenuArrow = 0;
    }

    else
    {
      lcd.clear();
      page = 9;

      colClockStyle = 0;
      rowClockStyle = 0;

      colTmpStyle = 0;
      rowTmpStyle = 0;

      colHmdStyle = 0;
      rowHmdStyle = 0;

      colPhraseStyle = 0;
      rowPhraseStyle = 0;
    }
  }
}

void move_right_oneWid(int colFull, int &col, int row)
{
  if(colFull < 15)
  {
    col++;
    erase_1(col - 1, row);
  }

  else
  {
    lcd.clear();
    col = 0;
  }
}

void move_left_oneWid(int colFull, int &col, int row)
{
  if(col > 0)
  {
    erase_1(colFull, row);
    col--;
  }

  else
  {
    lcd.clear();
    col = 15 - colFull;
  }
}

void move_up_down_oneWid(int col, int &row, bool phrase)
{
  if(phrase == true)
  {
    lcd.clear();  
  }
  
  else
  {
    lcd.setCursor(col, row);
    lcd.print(remAll);
  }
  
  if(row == 1)
  {
    row = 0;  
  }

  else
  {
    row = 1;
  }
}

void move_right_twoWid(int colFull, int &col, int row, int colFull2, int col2, int row2)
{
  if(row2 != row)
  {
    if(colFull < 15)
    {
      col++;
      erase_1(col - 1, row);
    }

    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 0;
    }
  }

  else
  {
    if(colFull == col2 - 1)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 0;        
    }
    
    else if(colFull < 15)
    {
      col++;
      erase_1(col - 1, row);
    }

    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = colFull2 + 1;
    }
  }
}

void move_left_twoWid(int colFull, int &col, int row, int colFull2, int col2, int row2)
{
  if(row2 != row)
  {
    if(col > 0)
    {
      erase_1(colFull, row);
      col--;
    }

    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 9;
    }
  }

  else
  {
    if(col == colFull2 + 1)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 9;
    }

    else if(col == 0)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = col2 - 7;
    }

    else
    {
      erase_1(colFull, row);
      col--;
    }
  }
}

void move_up_down_twoWid(int colFull, int col, int &row, int colFull2, int col2, int row2)
{
  if(row2 == row)
  {
    lcd.setCursor(col, row);
    lcd.print(remAll);
    
    if(row == 1)
    {
      row = 0;  
    }
  
    else
    {
      row = 1;
    }
  }

  else
  {
    if(colFull2 < col || colFull < col2)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      
      if(row == 1)
      {
        row = 0;
      }

      else
      {
        row = 1;
      }
    }
  }
}

void move_right_threeWid(int colFull, int &col, int &row, int colFull2, int col2, int row2, int colFull3, int col3, int row3)
{
  if(row2 == row3)
  {
    if(colFull < 15)
    {
      col++;
      erase_1(col - 1, row);
    }

    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 0;
    }
  }
  
  else
  {
    if(row == row3)
    {
      if(colFull == col3 - 1)
      {
        if(15 - colFull2 >= 7)
        {
          lcd.setCursor(col, row);
          lcd.print(remAll);
          col = colFull2 + 1;
          row = row2;
        }

        else
        {
          lcd.setCursor(col, row);
          lcd.print(remAll);
          col = 0;
        }
      }
  
      else if(colFull < 15)
      {
        col++;
        erase_1(col - 1, row);
      }
  
      else
      {
        lcd.setCursor(col, row);
        lcd.print(remAll);
        col = colFull3 + 1;
      }
    }
  
    else if(row == row2)
    {
      if(colFull == col2 - 1)
      {
        if(15 - colFull3 >= 7)
        {
          lcd.setCursor(col, row);
          lcd.print(remAll);
          col = colFull3 + 1;
          row = row3; 
        }

        else
        {
          lcd.setCursor(col, row);
          lcd.print(remAll);
          col = 0; 
        }
      }
  
      else if(colFull < 15)
      {
        col++;
        erase_1(col - 1, row);
      }
  
      else
      {
        lcd.setCursor(col, row);
        lcd.print(remAll);
        col = colFull2 + 1;
      }
    }
  }
}

void move_left_threeWid(int colFull, int &col, int &row, int colFull2, int col2, int row2, int colFull3, int col3, int row3)
{
  if(row == row3)
  {
    if(col == colFull3 + 1)
    {
      if(col2 >= 7)
      {
        lcd.setCursor(col, row);
        lcd.print(remAll);
        row = row2;
        col = col2 - 7;
      }
      else
      {
        lcd.setCursor(col, row);
        lcd.print(remAll);
        col = 9;
      }
    }

    else if(col == 0)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = col3 - 7;
    }

    else
    {
      erase_1(colFull, row);
      col--;
    }
  }

  else if(row == row2)
  {
    if(col == colFull2 + 1)
    {
      if(col3 >= 7)
      {
        lcd.setCursor(col, row);
        lcd.print(remAll);
        row = row3;
        col = col3 - 7;
      }
      
      else
      {
        lcd.setCursor(col, row);
        lcd.print(remAll);
        col = 9;
      }
    }

    else if(col == 0)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = col2 - 7;
    }

    else
    {
      erase_1(colFull, row);
      col--;
    }
  }

  else
  {
    if(col > 0)
    {
      erase_1(colFull, row);
      col--;
    }
  
    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 15 - colFull;
    }
  }
}

void move_up_down_threeWid(int colFull, int col, int &row, int colFull2, int col2, int row2, int colFull3, int col3, int row3)
{
  if(row2 != row3)
  {
    if(row2 == row)
    {
      if(colFull3 < col || colFull < col3)
      {
        lcd.setCursor(col, row);
        lcd.print(remAll);
        
        if(row == 1)
        {
          row = 0;
        }
  
        else
        {
          row = 1;
        }
      }
    }

    else if(row3 == row)
    {
      if(colFull2 < col || colFull < col2)
      {
        lcd.setCursor(col, row);
        lcd.print(remAll);
        
        if(row == 1)
        {
          row = 0;
        }
  
        else
        {
          row = 1;
        }
      }
    }
  }
}

void move_right_fourWid(int colFull, int &col, int row, int colFull2, int col2, int row2, int colFull3, int col3, int row3, int colFull4, int col4, int row4)
{
  if(row2 == row3)
  {
    if(colFull == col4 - 1)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 0; 
    }

    else if(colFull < 15)
    {
      col++;
      erase_1(col - 1, row);
    }

    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = colFull4 + 1;
    }
  }

  else if(row2 == row4)
  {
    if(colFull == col3 - 1)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 0; 
    }

    else if(colFull < 15)
    {
      col++;
      erase_1(col - 1, row);
    }

    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = colFull3 + 1;
    }
  }

  else if(row4 == row3)
  {
    if(colFull == col2 - 1)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 0; 
    }

    else if(colFull < 15)
    {
      col++;
      erase_1(col - 1, row);
    }

    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = colFull2 + 1;
    }
  }
}

void move_left_fourWid(int colFull, int &col, int row, int colFull2, int col2, int row2, int colFull3, int col3, int row3, int colFull4, int col4, int row4)
{
  if(row2 == row3)
  {
    if(col == colFull4 + 1)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 9;
    }

    else if(col == 0)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = col4 - 7;
    }

    else
    {
      erase_1(colFull, row);
      col--;
    }
  }

  else if(row2 == row4)
  {
    if(col == colFull3 + 1)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 9;
    }

    else if(col == 0)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = col3 - 7;
    }

    else
    {
      erase_1(colFull, row);
      col--;
    }
  }

  else if(row4 == row3)
  {
    if(col == colFull2 + 1)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = 9;
    }

    else if(col == 0)
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = col2 - 7;
    }

    else
    {
      erase_1(colFull, row);
      col--;
    }
  }
}

void style_to_home(void)
{
  colHmd = colHmdStyle;
  rowHmd = rowHmdStyle;
  colClock = colClockStyle;
  rowClock = rowClockStyle;
  colTmp = colTmpStyle;
  rowTmp = rowTmpStyle;
  colPhrase = colPhraseStyle;
  rowPhrase = rowPhraseStyle;
  
  colHmdStyle = 0;
  rowHmdStyle = 0;
  colTmpStyle = 0;
  rowTmpStyle = 0;
  colPhraseStyle = 0;
  rowPhraseStyle = 0;
  colClockStyle = 0;
  rowClockStyle = 0;
  
  rowMenuArrow = 0;
  colMenuArrow = 0;
    
  lcd.clear();
  page = 1;
}

int longest_phrase_len()
{
  if(longestInArr >= prefPhraseLen + 1)
  {
    defualtStyle = true;
    return longestInArr;
  }

  defualtStyle = false;
  return prefPhraseLen;
}

void smart_locating_oneWid(int &oldRow, int &newRow)
{
  if(oldRow == 1)
  {
    newRow = 0;
  }

  else
  {
    newRow = 1;
  }
}

void smart_locating_twoWid(int oldColFull1, int &oldCol1, int &oldRow1, int oldColFull2, int &oldCol2, int &oldRow2, int &newCol, int &newRow)
{
  if(oldRow1 != oldRow2)
  {
    if(15 - oldColFull1 >= 7)
    {
      newCol = 9;
      newRow = oldRow1;
    }

    else if(15 - oldColFull2 >= 7)
    {
      newCol = 9;
      newRow = oldRow2;
    }

    else if(oldCol1 > 6)
    {
      newCol = 0;
      newRow = oldRow1;
    }

    else if(oldCol2 > 6)
    {
      newCol = 0;
      newRow = oldRow2;
    }

    else
    {
      lcd.clear();
      page = 20;
    }
  }

  else
  {
    if(oldRow1 == 1)
    {
      newRow = 0;
    }

    else
    {
      newRow = 1;
    }
  }
}

void smart_locating_threeWid(int oldColFull1, int oldCol1, int oldRow1, int oldColFull2, int oldCol2, int oldRow2, int oldColFull3, int oldCol3, int oldRow3, int &newCol, int &newRow)
{
  if(oldRow1 == oldRow2)
  {
    if(15 - oldColFull3 >= 7)
    {
      newCol = 9;
      newRow = oldRow3;
    }

    else if(oldCol3 > 6)
    {
      newCol = 0;
      newRow = oldRow3;
    }

    else
    {
      lcd.clear();
      page = 20;
    }
  }

  else if(oldRow1 == oldRow3)
  {
    if(15 - oldColFull2 >= 7)
    {
      newCol = 9;
      newRow = oldRow2;
    }

    else if(oldCol2 > 6)
    {
      newCol = 0;
      newRow = oldRow2;
    }

    else
    {
      lcd.clear();
      page = 20;
    }
  }

  else if(oldRow2 == oldRow3)
  {
    if(15 - oldColFull1 >= 7)
    {
      newCol = 9;
      newRow = oldRow1;
    }

    else if(oldCol1 > 6)
    {
      newCol = 0;
      newRow = oldRow1;
    }

    else
    {
      lcd.clear();
      page = 20;
    }
  }
}

void store_initial_state()
{
  if(stored_values == false)
  {
    initialTmp = addTmp;
    initialHmd = addHmd;
    initialPhrase = addPhrase;
    initialClock = addClock;
    stored_values = true;
  }
}

void style_menu_to_home()
{
  addTmp = initialTmp;
  addHmd = initialHmd;
  addClock = initialClock;
  addPhrase = initialPhrase;
  stored_values = false;
  scrolled = false;
  option = 0;
  rowMenuArrow = 0;
  colMenuArrow = 0;
  lcd.clear();
  
  page = 1;
}

void customize()
{
  initialTmp = addTmp;
  initialHmd = addHmd;
  initialPhrase = addPhrase;
  initialClock = addClock;
  scrolled = false;
  option = 0;
  stored_values = false;
  lcd.clear();
  page = 9;
}
