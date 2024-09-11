// Arduino-TFT_eSPI board-template main routine. There's a TFT_eSPI create+flush driver already in LVGL-9.1 but we create our own here for more control (like e.g. 16-bit color swap).
#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>

////////////////////////// Include Libraries ////////////////////////////////////////////////////////
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>

////////////////////////// Defines //////////////////////////////////////////////////////////////////
#define I2C_Addr 0x20 // I2C Address of PCF8574-board: 0x20 - 0x27

const byte NbrRows = 4;    // Number of Rows
const byte NbrColumns = 3; // Number of Columns

////////////////////////// Layout of the Keys on Keypad /////////////////////////////////////////////
char KeyPadLayout[NbrRows][NbrColumns] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

////////////////////////// Wiring of PCF8574-IO-Pins ////////////////////////////////////////////////
byte PinsLines[NbrRows] = {0, 1, 2, 3};
/*-*/                                     //  ROWS Pins
byte PinsColumns[NbrColumns] = {4, 5, 6}; //  COLUMNS Pins

////////////////////////// Initialise KeyPad ////////////////////////////////////////////////////////
Keypad_I2C i2cKeypad(makeKeymap(KeyPadLayout), PinsLines, PinsColumns, NbrRows, NbrColumns, I2C_Addr);

//*********ROTARY SETUP BELOW HERE**********

#include <MD_REncoder.h>            // This library for rotary
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers
int32_t RotaryCount = 0;            // used to track rotary position
#define RotaryPinA 33               // SET DT PIN
#define RotaryPinB 25               // SET CLK PIN
MD_REncoder R = MD_REncoder(RotaryPinA, RotaryPinB);

#include <MD_UISwitch.h>                   //This library for button click (on the rotary)
const uint8_t DIGITAL_SWITCH_PIN = 35;     // SET SW PIN
const uint8_t DIGITAL_SWITCH_ACTIVE = LOW; // digital signal when switch is pressed 'on'
MD_UISwitch_Digital S(DIGITAL_SWITCH_PIN, DIGITAL_SWITCH_ACTIVE);
int ButtonPressed = 0; // 0 is not pressed, 1 is pressed

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/
static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 240;

enum
{
    SCREENBUFFER_SIZE_PIXELS = screenWidth * screenHeight / 10
};
static lv_color_t buf[SCREENBUFFER_SIZE_PIXELS];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *pixelmap)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    if (LV_COLOR_16_SWAP)
    {
        size_t len = lv_area_get_size(area);
        lv_draw_sw_rgb565_swap(pixelmap, len);
    }

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)pixelmap, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
/*void my_touchpad_read (lv_indev_t * indev_driver, lv_indev_data_t * data)
{
    uint16_t touchX = 0, touchY = 0;

    bool touched = false;//tft.getTouch( &touchX, &touchY, 600 );

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;


        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print( "Data x " );
        Serial.println( touchX );

        Serial.print( "Data y " );
        Serial.println( touchY );
    }
}*/

static uint32_t keypad_get_key(void)
{
    /*char key_ch = 0;
    Wire.requestFrom(I2C_Addr, 1);
    while (Wire.available() > 0) {
      key_ch = Wire.read();
    }*/
    char KeyRead = i2cKeypad.getKey();
    return KeyRead;
}
static void keypad_read(lv_indev_t *indev_drv, lv_indev_data_t *data)
{
    static uint32_t last_key = 0;
    uint32_t act_key;
    act_key = keypad_get_key();
    if (act_key != 0)
    {
        data->state = LV_INDEV_STATE_PR;
        Serial.printf("Key pressed : 0x%x\n", act_key);
        last_key = act_key;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
    data->key = last_key;
}

/*Set tick routine needed for LVGL internal timings*/
static uint32_t my_tick_get_cb(void) { return millis(); }

int counter = 0;
int State;
int old_State;
int move_flag = 0;
void my_encoder_read(lv_indev_t *drv, lv_indev_data_t *data);
static lv_group_t *g;

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */

    //******Rotary and button read below******
    R.begin();
    S.begin();
    S.enableDoublePress(false);
    // S.enableLongPress(false);
    // S.enableRepeat(false);
    //   S.enableRepeatResult(true);
    //******Rotary and button read above******

    //******i2c keypad setup below******
    Serial.println(F("--- Begin: Check Connection ..."));
    Wire.begin(21, 22);               // Init I2C-Bus, GPIO4-Data, GPIO5 Clock
    Wire.beginTransmission(I2C_Addr); // Try to establish connection
    if (Wire.endTransmission() != 0)  // No Connection established
    {
        Serial.print("    NO ");
    } // if (Wire.endTransmission() != 0)
    else
    {
        Serial.print("    ");
    } // else to if (Wire.endTransmission() != 0)

    Serial.print(F("Device found on"));
    Serial.print(F(" (0x"));
    Serial.print(I2C_Addr, HEX);
    Serial.println(F(")."));
    Serial.println(F("--- End: Check Connection"));

    Serial.print(F("--- Starting I2C-KeyPad-Library ..."));
    i2cKeypad.begin(); // Start i2cKeypad
    Serial.println(F("started."));
    //******i2c keypad setup above******

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println(LVGL_Arduino);
    Serial.println("I am LVGL_Arduino");

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    tft.begin();        /* TFT init */
    tft.setRotation(0); /* Landscape orientation, flipped */

    static lv_disp_t *disp;
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_buffers(disp, buf, NULL, SCREENBUFFER_SIZE_PIXELS * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, my_disp_flush);

    /*static lv_indev_t* indev;
    indev = lv_indev_create();
    lv_indev_set_type( indev, LV_INDEV_TYPE_POINTER );
    lv_indev_set_read_cb( indev, my_touchpad_read );*/

    ui_init();

    g = lv_group_create();
    lv_group_set_default(g);

    // encoder
    static lv_indev_t *enc_drv;
    enc_drv = lv_indev_create();
    lv_indev_set_type(enc_drv, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(enc_drv, my_encoder_read);
    lv_indev_set_group(enc_drv, g);
    // keypad
    static lv_indev_t *kb_drv;
    kb_drv = lv_indev_create();
    lv_indev_set_type(kb_drv, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(kb_drv, keypad_read);
    lv_indev_set_group(kb_drv, g);

    lv_tick_set_cb(my_tick_get_cb);

    Serial.println("Setup done");
}

void loop()
{

    lv_task_handler(); /* let the GUI do its work */
    delay(5);
    //**********************ClickButton Start*************

    MD_UISwitch::keyResult_t k = S.read();

    switch (k)
    {
    case MD_UISwitch::KEY_NULL: /* Serial.print("KEY_NULL"); */
        break;
    case MD_UISwitch::KEY_UP: /*Serial.print("\nKEY_UP "); */
        break;
    case MD_UISwitch::KEY_DOWN: /* Serial.print("\n\nKEY_DOWN ");*/
        break;
    case MD_UISwitch::KEY_PRESS:
        Serial.println("********************* A KEY_PRESS ");
        ButtonPressed = 1;
        break;
    case MD_UISwitch::KEY_DPRESS:
        Serial.println("KEY_DOUBLE ");
        break;
    case MD_UISwitch::KEY_LONGPRESS:
        Serial.println("KEY_LONG   ");
        break;
    case MD_UISwitch::KEY_RPTPRESS: /*Serial.print("\nKEY_REPEAT ");*/
        break;
    default: /*Serial.print("\nKEY_UNKNWN ");*/
        break;
    }

    //**********************ClickButton End***************

    //**********************Rotary Start***************
    uint8_t x = R.read();
    if (x)
    {
        if (x == DIR_CW)
        {
            Serial.print("NEXT ");
            ++RotaryCount;
            Serial.println(RotaryCount);
        }
        else
        {
            Serial.print("PREV ");
            --RotaryCount;
            Serial.println(RotaryCount);
        }
    }
    //**********************Rotary End***************

}

void my_encoder_read(lv_indev_t *drv, lv_indev_data_t *data)
{
    static int lastBtn;
    static int32_t last_diff = 0;

    int32_t diff = RotaryCount;
    int btn_state = 0;

    if (ButtonPressed == 1)
    {
        data->state = LV_INDEV_STATE_PR;
        ButtonPressed = 0;
        Serial.println("Button Pressed and Variable RESET to 0");
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Serial.print("diff: ");
    Serial.println(diff);

    Serial.print("diff - last_diff: ");
    Serial.println(diff - last_diff);*/

    data->enc_diff = diff - last_diff;

    last_diff = diff;

    if (lastBtn != btn_state)
    {
        lastBtn = btn_state;
    }
    // Serial.println();
    // return false;
}
