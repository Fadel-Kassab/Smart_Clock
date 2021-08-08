// Grapotronics
// By Fadel Kassab
// 08/08/2021
#include <LiquidCrystal_I2C.h>
#include <dht11.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define DHT11PIN 8      
dht11 DHT11;
const short lBtn = 2;
const short rBtn = 3;
const short ctrlBtn = 4;
const short upBtn = 5;
const short downBtn = 6;
unsigned long previousBtn = 0;
unsigned long eventBtn = 200;
unsigned long previousClock = 0;
unsigned long eventClock = 60000;
unsigned long previousQuotes = 0;
unsigned long eventQuotes = 4500;
short page = 1;
const String arrows[5] = {"<", ">", "^", "<-", "->"};
bool scrolled = false;
short arrow = 4;
short colArrow = 0;
bool rowArrow = 0;
String userPhrase = "";
const String words[4] = {"RELAX", "CHILL", "LEARN", "LOVE"};
bool chooseChar = true;
bool defualtPhrases = true;
short ranWord;
short ranChar;
const String quotes[25] = {
"KEEP IT SIMPLE",
"LIFE WON’T WAIT",
"NEVER GIVE UP",
"YES YOU CAN",
"BE THE CHANGE",
"HAKUNA MATATA",
"AIM HIGH",
"SCARE YOUR FEARS",
"AMPLIFY HOPE",
"SHINE ON",
"WHERE ARE YOU?",
"WHY NOT?",
"DREAM BIG",
"ALL IS WELL",
"NOW OR NEVER",
"USE YOUR WINGS",
"LET IT BE",
"JUST DO IT!",
"GOOD VIBES",
"STAY BEAUTIFUL",
"CHASE GRACE",
"CHANGE THE GAME",
"AND STILL I RISE",
"BE FEARLESS",
"YOU MATTER"};
String quote;
short colQuote;
bool startAnimation = true;
const String am_pm[2] = {"AM", "PM"};
bool format = 0;
short h2 = 1;
short h1 = 2;
short m2 = 0;
short m1 = 0;
bool ap = 0;
bool am_pm_gate = true;
bool addHmd = true;
bool addPhrase = true;
bool addClock = true;
bool addTmp = true;
short colClock = 0;
bool rowClock = 0;
short colPhrase = 0;
bool rowPhrase = 1;
short colTmp = 9;
bool rowTmp = 0;
short colHmd = 9;
bool rowHmd = 1;
short colChar = 0;
short colClockStyle = 0;
bool rowClockStyle = 0;
short colTmpStyle = 0;
bool rowTmpStyle = 0;
short colHmdStyle = 0;
bool rowHmdStyle = 0;
short colPhraseStyle = 0;
bool rowPhraseStyle = 0;
short option = 0;
bool initialHmd = true;
bool initialTmp = true;
bool initialClock = true;
bool initialPhrase = true;
bool stored_values = false;
bool defualtStyle;
short colPhraseFull;
const String remAll = "       ";
bool alarm_on = false;
bool alarm_ap = 0;
short target_repeats = 1;
short alarm_repeats = 1;
short alarm_m1 = 0;
short alarm_m2 = 0;
short alarm_h1 = 2;
short alarm_h2 = 1;
short target_h1;
short target_h2;
short target_m1;
short target_m2;
short target_ap;
char line_one[16];
char line_two[16];
const String Data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!?<>{}[]()~$%*/+-=|&;:_ ";
short character = 0;
short colWriting = 0;
bool rowWritingChar = 0;
bool rowWritingArrow = 1;
bool rowWritingArrow2 = 1;
bool rowWritingChar2 = 0;
short colWriting2 = 0;
bool credits_animation = false;
short colCredits = 15;
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
  pinMode(DHT11PIN, INPUT);
  lcd.createChar(0, heart);
  lcd.createChar(1, music);
  lcd.createChar(2, iceBall);
  lcd.createChar(3, tree);
  lcd.createChar(4, turtle);
  lcd.createChar(5, tick);
  lcd.createChar(6, downArrow);  
}

void loop(void)
{
  unsigned long current = millis();
  choose_phrase();
  if(current - previousClock == eventClock)
  {
    Clock();
    previousClock = current;
  }
  if(current - previousQuotes == eventQuotes)
  {
    startAnimation = true;
    if(page == 0)
    {
      lcd.clear(); 
    }   
    previousQuotes = current; 
  }
  if(current - previousBtn == eventBtn)
  {
    if(alarm_on == true)
    { 
      if(target_repeats > alarm_repeats || target_repeats == 10)
      {
        check_alarm();        
      }
    }
    if(digitalRead(rBtn) == LOW && page < 4) // Navigaition
    {
      lcd.clear();
      if(page == 3)
      {
        page = 0;
      }
      else
      {
        page++; 
      }
      rowArrow = 0;
    }
    else if(digitalRead(lBtn) == LOW && page < 4)
    {
      lcd.clear();
      if(page == 0)
      {
        page = 3;
      }
      else
      {
        page--; 
      }
      rowArrow = 0;
    }
   
    /* QUOTE PAGE */
    if(page == 0){
      view_quotes();     
    }
    /* HOME PAGE */
    else if(page == 1)
    {
      home_page_labels();
      if(digitalRead(ctrlBtn) == LOW)
      {
        convert_format();
      }
      else if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
      {
        if(defualtPhrases == false)
        {
          defualtPhrases = true;
          lcd.setCursor(colPhrase, rowPhrase);
          lcd.print("                ");
        }
        else
        {
          defualtPhrases = false;
          erase(colPhrase, rowPhrase, 1);
          erase(colPhrase + 2, rowPhrase, 1);
          erase(colPhrase + 4, rowPhrase, 1);
        }
      }
    }
    /* ALARM NOTES MENU */
    else if(page == 2)
    {
      colArrow = 0;
      lcd.setCursor(4, 0);
      lcd.print("ALARM");
      lcd.setCursor(4, 1);
      lcd.print("NOTEPAD");      
      lcd.setCursor(colArrow, rowArrow);
      lcd.print(arrows[4]);
      if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
      {
        erase(colArrow, rowArrow, 1);
        if(rowArrow == 1)
        {
          rowArrow = 0;
        }
        else
        {
          rowArrow = 1;
        }
      }
      else if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        if(rowArrow == 0)
        {
          colArrow = 10;
          page = 4; 
        }
        else
        {
          page = 6;
        }
        rowArrow = 0;
      }
    }
    /* SETTINGS CREDITS MENU */
    else if(page == 3)
    {
      colArrow = 0;
      lcd.setCursor(4, 0);
      lcd.print("SETTINGS");
      lcd.setCursor(4, 1);
      lcd.print("CREDITS");
      lcd.setCursor(colArrow, rowArrow);
      lcd.print(arrows[4]);
      if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
      {
        erase(colArrow, rowArrow, 1);
        if(rowArrow == 1)
        {
          rowArrow = 0;
        }
        else
        {
          rowArrow = 1;
        }
      }
      else if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        if(rowArrow == 0)
        {
          page = 9;
        }

        else
        {
          page = 18;
        }
      } 
    }
    /* ALARM MENU */
    else if(page == 4)
    {
      if(rowArrow == 0)
      {
        colArrow = 10;
      }
      else
      {
        colArrow = 11;
      } 
      lcd.setCursor(0, 0);
      lcd.print("SET ALARM");
      lcd.setCursor(0, 1);
      lcd.print("STOP ALARM");
      lcd.setCursor(colArrow, rowArrow);
      lcd.print(arrows[3]);
      if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
      {
        erase(colArrow, rowArrow, 1);
        if(rowArrow == 1)
        {
          rowArrow = 0;
        }
        else
        {
          rowArrow = 1;
        }
      }
      else if(digitalRead(rBtn) == LOW || digitalRead(lBtn) == LOW)
      {
        lcd.clear();
        rowArrow = 0;
        page = 2;
      }
      else if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        if(rowArrow == 0)
        {
          arrow = 2;
          rowArrow = 1;
          colArrow = 5;
          page = 5;
        }
        else
        {
          alarm_on = false;
          target_repeats = 1;
          alarm_repeats = 1;
          page = 25; 
        }
      }
    }
    /* ALARM ADJUSTMENT */
    else if(page == 5)
    {
      alarm_page_labels();
      if(digitalRead(rBtn) == LOW)
      {
        move_arrow("RIGHT", 1);
      }
      else if(digitalRead(lBtn) == LOW)
      {
        move_arrow("LEFT", 1);
      }
      switch(colArrow)
      {
        case 12:
          arrow = 1;
          if(digitalRead(ctrlBtn) == LOW)
          { 
            lcd.clear();
            page = 20; // banner
            target_h2 = alarm_h2;
            target_h1 = alarm_h1;
            target_m1 = alarm_m1;
            target_m2  = alarm_m2;
            target_ap = alarm_ap;
          }
          break;
  
        case 4:
          arrow = 0;
          if(digitalRead(ctrlBtn) == LOW)
          {
            rowArrow = 0;
            lcd.clear();
            page = 1;     
          }
          break;
  
        default:
          arrow = 2;
          if(digitalRead(upBtn) == LOW)
          {
            switch(colArrow)
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
            switch(colArrow)
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
    /* NOTES MENU */
    else if(page == 6)
    {
      if(rowArrow == 1)
      {
        colArrow = 6;
      }
      else
      {
        colArrow = 5;
      }
      lcd.setCursor(0, 0);
      lcd.print("READ"); 
      lcd.setCursor(0, 1);
      lcd.print("WRITE");
      lcd.setCursor(colArrow, rowArrow);
      lcd.print(arrows[3]);
      if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
      {
        erase(colArrow, rowArrow, 1);
        if(rowArrow == 1)
        {
          rowArrow = 0;
        }
        else
        {
          rowArrow = 1;
        }
      }
      else if(digitalRead(lBtn) == LOW || digitalRead(rBtn) == LOW)
      {
        lcd.clear();
        rowArrow = 0;
        page = 2;     
      }
      else if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        if(rowArrow == 0)
        {
          page = 7;
        }

        else
        {
          page = 8;
        }
      }
    }
    /* NOTES READ */
    else if(page == 7)
    {
      lcd.setCursor(0, 0);
      lcd.print(line_one);
      lcd.setCursor(0, 1);
      lcd.print(line_two);
      if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        page = 8;
      }
      else if(digitalRead(lBtn) == LOW || digitalRead(rBtn) == LOW)
      {
        rowArrow = 0;
        lcd.clear();
        page = 6;
      }
    }
    /* NOTES WRITE */
    else if(page == 8)
    {
      lcd.setCursor(0, 0);
      lcd.print(line_one);
      lcd.setCursor(0, 1);
      lcd.print(line_two);
      lcd.setCursor(colWriting, rowWritingChar);
      lcd.print(Data[character]);
      lcd.setCursor(colWriting, rowWritingArrow);
      if(rowWritingArrow == 1)
      {
        lcd.print(arrows[2]);
      }
      else
      {
        lcd.write(byte(6));
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
        page = 7;
      }
    }
    /* STYLE & TIME ADJUSTMENT MENU */
    else if(page == 9)
    {
      if(rowArrow == 0)
      {
        colArrow = 12;
      }
      else
      {
        colArrow = 6;
      }
      lcd.setCursor(0, 0);
      lcd.print("ADJUST TIME");
      lcd.setCursor(0, 1);
      lcd.print("STYLE");
      lcd.setCursor(colArrow, rowArrow);
      lcd.print(arrows[3]);
      if(digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)    
      {
        erase(colArrow, rowArrow, 1);
        if(rowArrow == 1)
        {
          rowArrow = 0;
        }
        else
        {
          rowArrow = 1;
        }
      }
      else if(digitalRead(lBtn) == LOW || digitalRead(rBtn) == LOW)
      {
        lcd.clear();
        rowArrow = 0;
        page = 3;
      }
      else if(digitalRead(ctrlBtn) == LOW)
      {
        lcd.clear();
        if(rowArrow == 0)
        {
          arrow = 2;
          colArrow = 5;
          page = 10;
        }

        else
        {
          scrolled = false;
          rowArrow = 0;
          page = 11;
        }
      }
    }
    /* TIME ADJUSTMENT */
    else if(page == 10)
    {
      time_adjustment_labels();
      if(digitalRead(rBtn) == LOW)
      {
        move_arrow("RIGHT", 0);
      }
      else if(digitalRead(lBtn) == LOW)
      {
        move_arrow("LEFT", 0);
      }
      
      switch(colArrow)
      {
        case 12:
          arrow = 1;
          if(digitalRead(ctrlBtn) == LOW)
          {
            time_adjustment_to_home();  
          }
          break;
       
        default:
          arrow = 2;
          if(digitalRead(upBtn) == LOW)
          {
            switch(colArrow)
            {
              case 5:
                h2_change("UP", h2, h1, format);
                break;
  
              case 6:
                h1_change("UP", h2, h1, format);
                break;
  
              case 8:
                m2_change("UP", m2, m1);
                break;
  
              case 9:
                m1_change("UP", m2, m1);
                break;
  
              case 10:
                format_change("UP");
                break;
            }
          }
          else if(digitalRead(downBtn) == LOW)
          {
            switch(colArrow)
            {
              case 5:
                h2_change("DOWN", h2, h1, format);
                break;
  
              case 6:
                h1_change("DOWN", h2, h1, format);
                break;
  
              case 8:
                m2_change("DOWN", m2, m1);
                break;
  
              case 9:
                m1_change("DOWN", m2, m1);
                break;
  
              case 10:
                format_change("DOWN");
                break;
            }
          }
          break;
      }
    }
    /* STYLE MENU*/
    else if(page == 11)
    {
      if(scrolled == false)
      {
        if(rowArrow == 0)
        {
          colArrow = 10;
        }
        else
        {
          colArrow = 12;
        }
        lcd.setCursor(0, 0);
        lcd.print("CUSTOMIZE");
        lcd.setCursor(0, 1);
        lcd.print("EDIT PHRASE");
        lcd.setCursor(colArrow, rowArrow);
        lcd.print(arrows[3]);
        if(digitalRead(lBtn) == LOW || digitalRead(rBtn) == LOW)
        {
          lcd.clear();
          page = 9;  
        }
        else if(digitalRead(upBtn) == LOW)
        {
          erase(colArrow, rowArrow, 1);
          if(rowArrow == 1)
          {
            rowArrow = 0;
          }
          else
          {
            lcd.clear();
            scrolled = true;
            rowArrow = 1;
          }
        }
        else if(digitalRead(downBtn) == LOW)
        {
          erase(colArrow, rowArrow, 1);
          if(rowArrow == 0)
          {
            rowArrow = 1;
          }
          else
          {
            lcd.clear();
            scrolled = true;
            rowArrow = 1;
          }
        }
        else if(digitalRead(ctrlBtn) == LOW)
        {
          if(rowArrow == 0)
          {
            lcd.clear();
            page = 12;        
          }
          else
          {
            lcd.clear();
            character = 0;
            
            rowWritingArrow2 = 1;
            rowWritingChar2 = 0;
            colWriting2 = 0;
            userPhrase = "";
            page = 17;
          }
        }
      }
      else
      {
        if(rowArrow == 0)
        {
          colArrow = 12;
        }
        else
        {
          colArrow = 9;
        }
        lcd.setCursor(0, 0);
        lcd.print("EDIT PHRASE");
        lcd.setCursor(0, 1);
        lcd.print("DEFUALTS");
        lcd.setCursor(colArrow, rowArrow);
        lcd.print(arrows[3]);
        if(digitalRead(lBtn) == LOW || digitalRead(rBtn) == LOW)
        {
          lcd.clear();
          page = 9;
        }
        else if(digitalRead(upBtn) == LOW)
        {
          erase(colArrow, rowArrow, 1);
          if(rowArrow == 1)
          {
            rowArrow = 0;
          }
          else
          {
            rowArrow = 0;
            lcd.clear();
            scrolled = false;
          }
        }
        else if(digitalRead(downBtn) == LOW)
        {
          erase(colArrow, rowArrow, 1);
          if(rowArrow == 0)
          {
            rowArrow = 1;
          }

          else
          {
            rowArrow = 0;
            lcd.clear();
            scrolled = false;
          }
        }
        else if(digitalRead(ctrlBtn) == LOW)
        {
          lcd.clear();
          if(rowArrow == 0)
          {
            page = 17;
            character = 0;
            rowWritingArrow2 = 1;
            rowWritingChar2 = 0;
            colWriting2 = 0;
            userPhrase = "";
          }
          else
          {
            style_defualts();
            page = 1;
          }
        }
      }
    }
    /* CHOOSE WIDGETS */
    else if(page == 12)
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
          lcd.write(byte(5));
        }
        else
        {
          lcd.setCursor(5, 0);
          lcd.print(" ");
        }
        
        if(addTmp == true)
        {
          lcd.setCursor(13, 0);
          lcd.write(byte(5));
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
        lcd.print(arrows[0]);
       
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
            rowArrow = 0;
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

        if(addTmp == true)
        { 
          lcd.setCursor(13, 1);
          lcd.write(byte(5));
        }
        else
        {
          lcd.setCursor(13, 1);
          lcd.print(" ");
        }
        
        if(addClock == true)
        {
          lcd.setCursor(5, 0);
          lcd.write(byte(5));
        }
        else
        {
          lcd.setCursor(5, 0);
          lcd.print(" ");
        }
        
        if(addHmd == true)
        {
          lcd.setCursor(5, 1);
          lcd.write(byte(5));  
        }
        else
        {
          lcd.setCursor(5, 1);
          lcd.print(" ");
        }
        
        if(addPhrase == true)
        {
          lcd.setCursor(14, 0);
          lcd.write(byte(5)); 
        }
        else
        {
          lcd.setCursor(14, 0);
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
                addTmp = false;
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
    /* LOCATE PHRASE */
    else if(page == 13)
    {
      if(addPhrase == true)
      {
        short longestPhraseLen = longest_phrase_len();
        
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
          lcd.print(userPhrase);
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
            page = 14;
          }

          else if(addTmp == true)
          {
            smart_locating_oneWid(rowPhraseStyle, rowTmpStyle);
            page = 15;
          }

          else if(addHmd == true)
          {
            smart_locating_oneWid(rowPhraseStyle, rowHmdStyle);
            page = 16;
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
          page = 14;
        }

        else if(addTmp == true)
        {
          page = 15;
        }

        else if(addHmd == true)
        {
          page = 16;
        }

        else
        {
          style_to_home();
        }
      }
    }
    /* LOCATE CLOCK */
    else if(page == 14)
    {
      short colClockFull = colClockStyle + 6;
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
            page = 15;
            smart_locating_twoWid(colClockStyle + 6, colClockStyle, rowClockStyle, colPhraseFull, colPhraseStyle, rowPhraseStyle, colTmpStyle, rowTmpStyle);
          }

          else if(addHmd == true)
          {
            page = 16;
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
            page = 15;
          }

          else if(addHmd == true)
          {
            smart_locating_oneWid(rowClockStyle, rowHmdStyle);
            page = 16;
          }
          
          else
          {
            style_to_home();
          }
        }
      }
    }
    /* LOCATE TMP */
    else if(page == 15)
    {
      short colTmpFull = colTmpStyle + 6;
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
            page = 16;
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
            page = 16;
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
            page = 16;
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
            page = 16;
            smart_locating_oneWid(rowTmpStyle, rowHmdStyle);
          }

          else
          {
            style_to_home();
          }
        }
      }
    }
    /* LOCATE HMD */
    else if(page == 16)
    {
      short colHmdFull = colHmdStyle + 6;
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
    /* EDIT USER PHRASE */
    else if(page == 17)
    {
      lcd.setCursor(colWriting2, rowWritingChar2);
      lcd.print(Data[character]);
      lcd.setCursor(colWriting2, rowWritingArrow2);
      lcd.print(arrows[2]);
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
        userPhrase.concat(Data[character]);
        lcd.clear();
        if(check_userPhrase_len() == true)
        {
          page = 1; 
        }
        else
        {
          arrow = 0;
          colArrow = 4;
          page = 24;
        }
      }
    }
    /* CREDITS */
    else if(page == 18)
    {
      view_credits();
      if(digitalRead(ctrlBtn) == LOW || digitalRead(rBtn) == LOW || digitalRead(lBtn) == LOW || digitalRead(upBtn) == LOW || digitalRead(downBtn) == LOW)
      {
        credits_animation = false;
        colCredits = 15;
        lcd.clear();
        page = 3;
      }
    }
    /* BANNERS */
    else if(page == 20)
    {
      alarm_options();
    }
    else if(page == 21)
    {
      alarm_started();
    }
    else if(page == 22)
    {
      alarm_done();
    }
    else if(page == 23)
    {
      error_no_space(0);
    }
    else if(page == 24)
    {
      error_no_space(1);
    }
    else if(page == 25)
    {
      alarm_off();
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

    else
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
    if(addClock == true)
    {
      lcd.setCursor(colClock + 5, rowClock);
      lcd.print("  "); 
    }
  }
}
void erase(short col, short row, bool amount)
{
  lcd.setCursor(col, row);
  if(amount == 1)
  {
    lcd.print("  ");
  }
  else
  {
    lcd.print(" ");
  }
}
void choose_phrase(void)
{
  if(format == 0)
  {
    if((chooseChar == true) && ((h2 == 0 && h1 == 3) || (h2 == 0 && h1 == 9) || (h2 == 1 && h1 == 2) || (h2 == 0 && h1 == 6)))
    {
      ranChar = random(0, 5);
      ranWord = random(0, 4);
      lcd.setCursor(words[ranWord].length() + 1, 1);
      lcd.print(" ");
      chooseChar = false;
    }
  
    
    if((h2 == 0 && h1 == 2) || (h2 == 0 && h1 == 5) || (h2 == 0 && h1 == 8) || (h2 == 1 && h1 == 1))
    {
      chooseChar = true;
    } 
  }

  else
  {
    if((chooseChar == true) && ((h2 == 1 && h2 == 8) || (h2 == 0 && h1 == 0) || (h2 == 2 && h1 == 1) || (h2 == 1 && h1 == 5) || (h2 == 0 && h1 == 3) || (h2 == 0 && h1 == 9) || (h2 == 1 && h1 == 2) || (h2 == 0 && h1 == 6)))
    {
      ranChar = random(0, 5);
      ranWord = random(0, 4);
      lcd.setCursor(words[ranWord].length() + 1, 1);
      lcd.print(" ");
      chooseChar = false;
    }

  
    if((h2 == 1 && h2 == 7) || (h2 == 2 && h1 == 3) || (h2 == 2 && h1 == 0) || (h2 == 1 && h1 == 4) || (h2 == 0 && h1 == 5) || (h2 == 0 && h1 == 2) || (h2 == 0 && h1 == 8) || (h2 == 1 && h1 == 1))
    {
      chooseChar = true;
    }  
  }
}
void home_page_labels(void)
{
  int chk = DHT11.read(DHT11PIN);
  if(addClock == true)
  {
    lcd.setCursor(colClock,  rowClock);
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
  }
  if(addHmd == true)
  {
    lcd.setCursor(colHmd, rowHmd);
    lcd.print("Hmd:");
    lcd.print((float)DHT11.humidity, 0);
    lcd.setCursor(colHmd + 6, rowHmd);
    lcd.print("%");
  }
  if(addTmp == true)
  {
    lcd.setCursor(colTmp, rowTmp);
    lcd.print("Tmp:");
    lcd.print((float)DHT11.temperature, 0);
    lcd.setCursor(colTmp + 6, rowTmp);
    lcd.print("C");
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
      lcd.print(userPhrase);
      lcd.setCursor(colPhrase + userPhrase.length() + 1, rowPhrase);
      lcd.write(byte(ranChar));
    }
  }
}
void alarm_page_labels(void)
{
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
  lcd.setCursor(colArrow, 1);
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
  lcd.setCursor(10, 0);
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
  lcd.setCursor(3, 0);
  lcd.print("RING");
  lcd.setCursor(8, 0);
  if(target_repeats == 1)
  {
    lcd.print("ONCE");
    erase(12 , 0, 1);
  }
  else if(target_repeats < 10)
  {
    lcd.print(target_repeats);
    lcd.setCursor(10, 0);
    lcd.print("DAYS");
  }
  else
  {
    lcd.print("ALWAYS");
  }
  lcd.setCursor(8, 1);
  lcd.print(arrows[2]);
  
  if(digitalRead(upBtn) == LOW)
  {
    if(target_repeats < 10)
    {
      erase(9, 0, 0);
      target_repeats++;
    }

    else
    {
      erase(12 , 0, 1);
      target_repeats = 1;
    }
  }

  else if(digitalRead(downBtn) == LOW)
  {
    if(target_repeats > 1)
    {
      erase(9, 0, 0);
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
    page = 21; // alarm set banner
  }
}
void alarm_started(void)
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
    page = 22; // alarm done
    alarm_repeats++;
    if(target_repeats == alarm_repeats)
    {
      target_repeats = 1;
      alarm_repeats = 1;
      alarm_on = false;
    }
  }
}
void alarm_done(void)
{
//  buzz();
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
    lcd.clear();
    page = 1;   
  }
}
void move_arrow(String Direction, bool mode)
{
  lcd.setCursor(colArrow, 1);
  lcd.print(" ");
  if(mode == 1)
  {
    if(Direction == "RIGHT")
    {
      if(colArrow == 12)
      {
        colArrow = 4;
      }
      else if(colArrow == 6)
      {
        colArrow = 8;
      }
      else if(colArrow == 10)
      {
        colArrow = 12;
      }
      else
      {
        colArrow++;
      }
    }
    else
    {
      if(colArrow == 4)
      {
        colArrow = 12;
      }
      else if(colArrow == 8)
      {
        colArrow = 6;
      }
      else if(colArrow == 12)
      {
        colArrow = 10;
      }
      else
      {
        colArrow--;
      }
    }
  }
  else if(mode == 0)
  {
    if(Direction == "RIGHT")
    {
      if(colArrow == 12)
      {
        colArrow = 5;
      }
      else if(colArrow == 6)
      {
        colArrow = 8;
      }
      else if(colArrow == 10)
      {
        colArrow = 12;
      }
      else
      {
        colArrow++;
      }
    }
  
    else
    {
      if(colArrow == 5)
      {
        colArrow = 12;
      }
      else if(colArrow == 8)
      {
        colArrow = 6;
      }
      else if(colArrow == 12)
      {
        colArrow = 10;
      }
      else
      {
        colArrow--;
      }
    }
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
    lcd.setCursor(colWriting2, rowWritingArrow2);
    lcd.print(" ");
    
    if(Direction == "NEXT")
    {
      if(userPhrase.length() < 13)
      {
        userPhrase.concat(Data[character]);
        colWriting2++;    
        character = 0;
      }
    }

    else
    {
      lcd.setCursor(colWriting2, rowWritingChar2);
      lcd.print(" "); 
      if(userPhrase.length() > 0)
      {
        userPhrase.remove(userPhrase.length() - 2);
        colWriting2--; 
        character = 0;
      }
    }
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
        line_one[colWriting] = Data[character];  
      }

      else
      {
        line_two[colWriting] = Data[character]; 
      }
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
void view_quotes(void)
{
  if(startAnimation == true)
  {
    quote = quotes[random(0, 25)];
    
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
void style_defualts(void)
{
  addHmd = true;
  addClock = true;
  addPhrase = true;
  addTmp = true;
  colTmpStyle = 0;
  rowTmpStyle = 0;
  colHmdStyle = 0;
  rowTmpStyle = 0;
  colClockStyle = 0;
  rowClockStyle = 0;
  colPhraseStyle = 0;
  rowPhraseStyle = 0;
  colClock = 0;
  rowClock = 0;
  colTmp = 9;
  rowTmp = 0;
  colPhrase = 0;
  rowPhrase = 1;
  colHmd = 9;
  rowHmd = 1;
  userPhrase = "";
}
void time_adjustment_labels(void)
{
  lcd.setCursor(5, rowClock);
  lcd.print(h2);
  lcd.setCursor(6, rowClock);
  lcd.print(h1);
  lcd.setCursor(7, rowClock);
  lcd.print(":");
  lcd.setCursor(8, rowClock);
  lcd.print(m2);
  lcd.setCursor(9, rowClock);
  lcd.print(m1);
  lcd.setCursor(colArrow, 1);
  lcd.print(arrows[arrow]);
  lcd.setCursor(13, 1);
  lcd.print("OK");
  lcd.setCursor(10, rowClock);
  if(format == 0)
  {
    lcd.print(am_pm[ap]);
  }
  else
  {
    lcd.print("H~24");
  }
}
void time_adjustment_to_home(void)
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
  rowArrow = 0;
  lcd.clear();
  page = 1;
}
void error_no_space(bool errorNum)
{
  lcd.setCursor(1, 0);
  lcd.print("NO SPACE LEFT!");
  lcd.setCursor(0, 1);
  lcd.print("HOME");
  lcd.setCursor(12, 1);
  lcd.print("EDIT");
  lcd.setCursor(colArrow, 1);
  lcd.print(arrows[arrow]);
  if(errorNum == 0)
  {
    if(digitalRead(rBtn) == LOW || digitalRead(lBtn) == LOW)
    {
      if(colArrow == 4)
      {
        erase(colArrow, 1, 0);
        arrow = 1;
        colArrow = 11;
      }
  
      else
      {
        erase(colArrow, 1, 0);
        arrow = 0;
        colArrow = 4;
      }
    }
  
    else if(digitalRead(ctrlBtn) == LOW)
    {
      if(colArrow == 4)
      {
        lcd.clear();
        page = 1;
        rowArrow = 0;
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
  else
  {
    if(digitalRead(rBtn) == LOW || digitalRead(lBtn) == LOW)
    {
      if(colArrow == 4)
      {
        erase(colArrow, 1, 0);
        arrow = 1;
        colArrow = 11;
      }
  
      else
      {
        erase(colArrow, 1, 0);
        arrow = 0;
        colArrow = 4;
      }
    }
    else if(digitalRead(ctrlBtn) == LOW)
    {
      lcd.clear();
      userPhrase = "";
      colWriting2 = 0;
      rowWritingChar2 = 0;
      rowWritingArrow2 = 1;
      character = 0;
      if(colArrow == 4)
      {
        page = 1;
      }

      else
      {
        page = 17;
      }
    }
  }
}
void move_right_oneWid(short colFull, short &col, bool row)
{
  if(colFull < 15)
  {
    col++;
    erase(col - 1, row, 0);
  }

  else
  {
    lcd.clear();
    col = 0;
  }
}
void move_left_oneWid(short colFull, short &col, bool row)
{
  if(col > 0)
  {
    erase(colFull, row, 0);
    col--;
  }

  else
  {
    lcd.clear();
    col = 15 - colFull;
  }
}
void move_up_down_oneWid(short col, bool &row, bool phrase)
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
void move_right_twoWid(short colFull, short &col, bool row, short colFull2, short col2, bool row2)
{
  if(row2 != row)
  {
    if(colFull < 15)
    {
      col++;
      erase(col - 1, row, 0);
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
      erase(col - 1, row, 0);
    }

    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = colFull2 + 1;
    }
  }
}
void move_left_twoWid(short colFull, short &col, bool row, short colFull2, short col2, bool row2)
{
  if(row2 != row)
  {
    if(col > 0)
    {
      erase(colFull, row, 0);
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
      erase(colFull, row, 0);
      col--;
    }
  }
}
void move_up_down_twoWid(short colFull, short col, bool &row, short colFull2, short col2, bool row2)
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
void move_right_threeWid(short colFull, short &col, bool &row, short colFull2, short col2, bool row2, short colFull3, short col3, bool row3)
{
  if(row2 == row3)
  {
    if(colFull < 15)
    {
      col++;
      erase(col - 1, row, 0);
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
        erase(col - 1, row, 0);
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
        erase(col - 1, row, 0);
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
void move_left_threeWid(short colFull, short &col, bool &row, short colFull2, short col2, bool row2, short colFull3, short col3, bool row3)
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
      erase(colFull, row, 0);
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
      erase(colFull, row, 0);
      col--;
    }
  }

  else
  {
    if(col > 0)
    {
      erase(colFull, row, 0);
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
void move_up_down_threeWid(short colFull, short col, bool &row, short colFull2, short col2, bool row2, short colFull3, short col3, bool row3)
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
void move_right_fourWid(short colFull, short &col, bool row, short colFull2, short col2, bool row2, short colFull3, short col3, bool row3, short colFull4, short col4, bool row4)
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
      erase(col - 1, row, 0);
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
      erase(col - 1, row, 0);
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
      erase(col - 1, row, 0);
    }

    else
    {
      lcd.setCursor(col, row);
      lcd.print(remAll);
      col = colFull2 + 1;
    }
  }
}
void move_left_fourWid(short colFull, short &col, bool row, short colFull2, short col2, bool row2, short colFull3, short col3, bool row3, short colFull4, short col4, bool row4)
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
      erase(colFull, row, 0);
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
      erase(colFull, row, 0);
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
      erase(colFull, row, 0);
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
  
  rowArrow = 0;

    
  lcd.clear();
  page = 1;
}
int longest_phrase_len(void)
{
  const short longestInArr = 5; // length of longest phrase in array
  short prefPhraseLen = userPhrase.length();
  if(longestInArr >= prefPhraseLen + 1)
  {
    defualtStyle = true;
    return longestInArr;
  }

  defualtStyle = false;
  return prefPhraseLen;
}
void smart_locating_oneWid(bool &oldRow, bool &newRow)
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
void smart_locating_twoWid(short oldColFull1, short &oldCol1, bool &oldRow1, short oldColFull2, short &oldCol2, bool &oldRow2, short &newCol, bool &newRow)
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
void smart_locating_threeWid(short oldColFull1, short oldCol1, bool oldRow1, short oldColFull2, short oldCol2, bool oldRow2, short oldColFull3, short oldCol3, bool oldRow3, short &newCol, bool &newRow)
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
void store_initial_state(void)
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
void style_menu_to_home(void)
{
  addTmp = initialTmp;
  addHmd = initialHmd;
  addClock = initialClock;
  addPhrase = initialPhrase;
  stored_values = false;
  scrolled = false;
  option = 0;
  rowArrow = 0;
  lcd.clear(); 
  page = 1;
}
void customize(void)
{
  initialTmp = addTmp;
  initialHmd = addHmd;
  initialPhrase = addPhrase;
  initialClock = addClock;
  scrolled = false;
  option = 0;
  stored_values = false;
  lcd.clear();
  page = 13;
}
bool check_userPhrase_len(void)
{
  short END = colPhrase + userPhrase.length() + 1;
  if(addPhrase == true)
  {
    if(addHmd == true && rowHmd == rowPhrase)
    {
      if(END >= colHmd)
      {
        return false; 
      }
    }
    else if(addTmp == true && rowTmp == rowPhrase)
    {
      if(END >= colHmd)
      {
        return false; 
      }
    }
    else if(addClock == true && rowClock == rowPhrase)
    {
      if(END >= colHmd)
      {
        return false; 
      }
    }
  }
  return true; 
}
void alarm_off(void)
{
  lcd.setCursor(3, 0);
  lcd.print("ALARM OFF!");
  lcd.setCursor(6, 1);
  lcd.print(">OK<"); 
  if(digitalRead(ctrlBtn) == LOW)
  {
    lcd.clear();
    page = 4;   
  }
}
void view_credits(void)
{
  if(credits_animation == false)
  {
    lcd.setCursor(colCredits, 1);
    lcd.print("GRAPOTRONICS");
    if(colCredits > 2)
    {
      erase(colCredits + 11 , 1, 0);
      colCredits--;
    }
    else
    {
      credits_animation = true;
      colCredits = 15;
    }  
  }
  else
  {
    lcd.setCursor(colCredits, 0);
    lcd.write(byte(2));
    lcd.setCursor(colCredits + 1, 0);
    lcd.print("FADEL KASSAB");
    lcd.setCursor(colCredits + 13, 0);
    lcd.write(byte(2));
    if(colCredits > 1)
    {
      erase(colCredits + 13 , 0, 0);
      colCredits--;
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.write(byte(4));
      lcd.setCursor(15, 1);
      lcd.write(byte(4)); 
    }
  }
}
