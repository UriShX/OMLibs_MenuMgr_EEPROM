/**  Example OMMenuMgr Sketch

 @author
 C. A. Church
 
 */

#include "OMMenuMgr.h"
#include <LiquidCrystal.h>


 // ui setup
 
 // ui setup
 
    // lcd pins
const byte LCD_RS  = 17;
const byte LCD_EN  = 18;
const byte LCD_D4  = 11;
const byte LCD_D5  = 8;
const byte LCD_D6  = 7;
const byte LCD_D7  = 4;

//#define BUTTONTYPE ANALOG
#define BUTTONTYPE ANALOG

const byte LCD_ROWS = 2;
const byte LCD_COLS = 16;

 // button values
#ifdef BUTTONTYPE ANALOG 
 // which input is our button
const byte BUT_PIN = 14;
// analog button read values
const int BUTSEL_VAL  = 70;
const int BUTFWD_VAL  = 250;
const int BUTREV_VAL  = 450;
const int BUTDEC_VAL  = 655;
const int BUTINC_VAL  = 830;

const byte BUT_THRESH  = 60;

// mapping of analog button values for menu
int BUT_MAP_ANALOG[5][2] = {
  {BUTFWD_VAL, BUTTON_FORWARD}, 
  {BUTINC_VAL, BUTTON_INCREASE}, 
  {BUTDEC_VAL, BUTTON_DECREASE}, 
  {BUTREV_VAL, BUTTON_BACK}, 
  {BUTSEL_VAL, BUTTON_SELECT} 
};
#endif

#ifdef BUTTONTYPE DIGTAL                             
int BUT_MAP_DIGITAL[5][2] = {
  {4, BUTTON_FORWARD}, 
  {5, BUTTON_INCREASE}, 
  {6, BUTTON_DECREASE}, 
  {7, BUTTON_BACK}, 
  {8, BUTTON_SELECT} 
};
#endif

// ====== Test Menu =========== 

byte foo = 0;
byte sel = 0;
unsigned int bar = 1;
long baz  = 0;
float bak = 0.0;

  // Create a list of states and values for a select input
const MENU_SELECT_ITEM  sel_ign = { 2, {"Ignore"} };
const MENU_SELECT_ITEM  sel_on  = { 1, {"On"} };
const MENU_SELECT_ITEM  sel_off = { 0, {"Off"} };

const MENU_SELECT_LIST  const state_list[] = { &sel_ign, &sel_on, &sel_off };
                                  
  // the special target for our state input
  
                                   // TARGET VAR   LENGTH                          TARGET SELECT LIST
const MENU_SELECT state_select = { &sel,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };

  // values to use 

                          //    TYPE            MAX    MIN    TARGET 
const MENU_VALUE foo_value = { TYPE_BYTE,       100,   0,     MENU_TARGET(&foo) };
const MENU_VALUE bar_value = { TYPE_UINT,       10000, 100,   MENU_TARGET(&bar) };
const MENU_VALUE baz_value = { TYPE_LONG,       10000, 1,     MENU_TARGET(&baz) };
const MENU_VALUE bak_value = { TYPE_FLOAT_1000, 0,     0,     MENU_TARGET(&bak) };
const MENU_VALUE sel_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&state_select) };

                          //        LABEL           TYPE        LENGTH    TARGET
const MENU_ITEM item_checkme  = { {"Byte Edit"},    ITEM_VALUE,  0,        MENU_TARGET(&foo_value) };
const MENU_ITEM item_barme    = { {"UInt Edit"},     ITEM_VALUE,  0,        MENU_TARGET(&bar_value) };
const MENU_ITEM item_bazme    = { {"Long Edit"},    ITEM_VALUE,  0,        MENU_TARGET(&baz_value) };
const MENU_ITEM item_bakme    = { {"Float Edit"},   ITEM_VALUE,  0,        MENU_TARGET(&bak_value) };
const MENU_ITEM item_state    = { {"Select Input"}, ITEM_VALUE,  0,        MENU_TARGET(&sel_value) };
const MENU_ITEM item_testme   = { {"Test Action"},  ITEM_ACTION, 0,        MENU_TARGET(uiQwkScreen) };

                   //        List of items in menu level
const MENU_LIST const root_list[]   = { &item_checkme, &item_barme, &item_bazme, &item_bakme, &item_state, &item_testme };

                  // Root item is always created last, so we can add all other items to it
const MENU_ITEM menu_root     = { {"Root"},        ITEM_MENU,   MENU_SIZE(root_list),    MENU_TARGET(&root_list) };


 // initialize LCD object
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity


OMMenuMgr Menu(&menu_root, MENU_DIGITAL);



void setup() {

  lcd.begin(LCD_COLS, LCD_ROWS);
  
  uiClear();
  
  Menu.setDrawHandler(uiDraw);
  Menu.setExitHandler(uiClear);
#ifdef BUTTONTYPE ANALOG
  Menu.setAnalogButtonPin(BUT_PIN, BUT_MAP_ANALOG, BUT_THRESH);
#endif
#ifdef BUTTONTYPE DIGITAL
  Menu.setDigitalButtonPins(BUT_MAP_DIGITAL);
#endif
  Menu.enable(true); 
  
  
}

void loop() {
 Menu.checkInput();//return The button pressed, one of: BUTTON_NONE, BUTTON_FORWARD, BUTTON_BACK, BUTTON_INCREASE, BUTTON_DECREASE, BUTTON_SELECT
 
}

void testAction() {

 digitalWrite(5, HIGH);  
}


void uiDraw(char* p_text, int p_row, int p_col, int len) {
  lcd.setCursor(p_col, p_row);
  
  for( int i = 0; i < len; i++ ) {
    if( p_text[i] < '!' || p_text[i] > '~' )
      lcd.write(' ');
    else  
      lcd.write(p_text[i]);
  }
}


void uiClear() {
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Enter for Menu"));
}


void uiQwkScreen() {
  lcd.clear();
  Menu.enable(false);
  
  lcd.print("Action!");
  lcd.setCursor(0, 1);
  lcd.print(F("Enter 2 return"));
  
  while( Menu.checkInput() != BUTTON_SELECT ) {
    ; // wait!
  }
//  
  Menu.enable(true);
  lcd.clear();
}  
