/*
  Icon configuration
  Width and height must match the icon font size
  GAP: Space between the icons
  BGAP: Gap between the display border and the cursor.
*/
#define ICON_WIDTH 32
#define ICON_HEIGHT 32
#define ICON_GAP 4
#define ICON_BGAP 16
#define ICON_Y 32 + ICON_GAP

#define BUTTON_GAP 2
#define BUTTON_BGAP 14

// Switch position and size
#define BUTTON_W 14
#define BUTTON_H 6

struct menu_entry_type
{
  const uint8_t *font;
  uint16_t icon;
  const char *name;
};

struct menu_state
{
  int16_t menu_start;    /* in pixel */
  int16_t frame_position;   /* in pixel */
  uint8_t position;     /* position, array index */
};

struct menu_entry_type menu_entry_list[] =
{
  { u8g2_font_open_iconic_embedded_4x_t, 65, "Clock Setup"},
  { u8g2_font_open_iconic_embedded_4x_t, 66, "Setting"},
  { u8g2_font_open_iconic_embedded_4x_t, 77, "Control Devices"},
  { u8g2_font_open_iconic_embedded_4x_t, 68, "Home"},
  { u8g2_font_open_iconic_embedded_4x_t, 69, "About"},
  { NULL, 0, NULL } 
};

enum _menu_type {
  MENU_BUTTON,
  MENU_EXIT
};

struct menu_type
{
  uint8_t type;
  uint8_t state;
  const char *name;
};

struct menu_type menu_button_list[] =
{
  { MENU_BUTTON, 1, "DEVICE 1"},
  { MENU_BUTTON, 0, "DEVICE 2"},
  { MENU_BUTTON, 0, "DEVICE 3"},
  { MENU_BUTTON, 1, "DEVICE 4"},
  { MENU_EXIT,   0, "Press SEL to return to Menu"},
  { NULL, 0, NULL} 
};


struct menu_state current_state = { ICON_BGAP, ICON_BGAP, 0 };
struct menu_state destination_state = { ICON_BGAP, ICON_BGAP, 0 };
struct menu_state currentBt_state = { BUTTON_BGAP, BUTTON_BGAP, 0 };
