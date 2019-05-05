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


struct menu_state current_state = { ICON_BGAP, ICON_BGAP, 0 };
struct menu_state destination_state = { ICON_BGAP, ICON_BGAP, 0 };
