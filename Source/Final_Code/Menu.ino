#include "Menu.h"

void showMenu()
{
  do
  {
    u8g2.clearBuffer();
    drawMenu(&current_state);  
    u8g2.setFont(u8g2_font_helvB10_tr);  
    u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entry_list[destination_state.position].name))/2,u8g2.getDisplayHeight()-5);
    u8g2.print(menu_entry_list[destination_state.position].name);    
    u8g2.sendBuffer();
    delay(10);
//    event = u8g2.getMenuEvent();
//    if ( event == U8X8_MSG_GPIO_MENU_NEXT )
//      toRightMenu(&destination_state);
//    if ( event == U8X8_MSG_GPIO_MENU_PREV )
//      toLeftMenu(&destination_state);
//    if ( event == U8X8_MSG_GPIO_MENU_SELECT )
//    {
//      u8g2.setFont(u8g2_font_helvB10_tr);  
//      u8g2.userInterfaceMessage("Selection:", menu_entry_list[destination_state.position].name, "", " Ok ");
//    }
    if(MenuStatus == 0)
    {
      break;
    }
  } while (1);
}


void drawMenu(struct menu_state *state)
{
  int16_t x;
  uint8_t i;
  x = state->menu_start;
  i = 0;
  while( menu_entry_list[i].icon > 0 )  
  {
    if ( x >= -ICON_WIDTH && x < u8g2.getDisplayWidth() )
    {
      u8g2.setFont(menu_entry_list[i].font);
      u8g2.drawGlyph(x, ICON_Y, menu_entry_list[i].icon );
    }
    i++;
    x += ICON_WIDTH + ICON_GAP;
  }
  u8g2.drawFrame(state->frame_position-1, ICON_Y-ICON_HEIGHT-1, ICON_WIDTH+2, ICON_WIDTH+2);
  u8g2.drawFrame(state->frame_position-2, ICON_Y-ICON_HEIGHT-2, ICON_WIDTH+4, ICON_WIDTH+4);
  u8g2.drawFrame(state->frame_position-3, ICON_Y-ICON_HEIGHT-3, ICON_WIDTH+6, ICON_WIDTH+6);
}

void toRightMenu(struct menu_state *state)
{
  if ( menu_entry_list[state->position+1].font != NULL )
  {
    if ( (int16_t)state->frame_position+ 2*(int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth() )
    {
      state->position++;
      state->frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
    }
    else
    {
      state->position++;      
      state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
    }
  }
}

void toLeftMenu(struct menu_state *state)
{
  if ( state->position > 0 )
  {
    if ( (int16_t)state->frame_position >= (int16_t)ICON_BGAP+(int16_t)ICON_WIDTH+ (int16_t)ICON_GAP )
    {
      state->position--;
      state->frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
    }    
    else
    {
      state->position--; 
      state->frame_position = ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
      
    }
  }
}

uint8_t towards_int16(int16_t *current, int16_t dest)
{
  if ( *current < dest )
  {
    (*current)++;
    return 1;
  }
  else if ( *current > dest )
  {
    (*current)--;
    return 1;
  }
  return 0;
}

uint8_t towardsMenu(struct menu_state *current, struct menu_state *destination)
{
  uint8_t r = 0;
  r |= towards_int16( &(current->frame_position), destination->frame_position);
  r |= towards_int16( &(current->frame_position), destination->frame_position);
  r |= towards_int16( &(current->menu_start), destination->menu_start);
  r |= towards_int16( &(current->menu_start), destination->menu_start);
  return r;
}