

void showMenu()
{
  uint8_t event;
  uint8_t eventExit = 0;
  while(1)
  {
    do
    {
      u8g2.firstPage();
      do
      {
        drawMenu(&current_state);  
        u8g2.setFont(u8g2_font_helvB10_tr);  
        u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entry_list[destination_state.position].name))/2,u8g2.getDisplayHeight()-5);
        u8g2.print(menu_entry_list[destination_state.position].name);    
        delay(10);
      } while( u8g2.nextPage() );
      //TRACE();
      event = getMenuButton();
      if ( event == 1 )
        toRightMenu(&destination_state);
      if ( event == 2 )
        toLeftMenu(&destination_state);
      if ( event == 3 )
      {
        if(menu_entry_list[destination_state.position].name == "Clock Setup") {
#if defined(ENABLE_CONNECT_CLOUD)
          handleUpdateTime();
#endif
          break;
        }
        else if(menu_entry_list[destination_state.position].name == "Home")
        {
          eventExit = 1;
          Serial.println("Home");
          break;
        }
        else if(menu_entry_list[destination_state.position].name == "About")
        {
          ShowInformation();
          break;
        }
        else if(menu_entry_list[destination_state.position].name == "Control Devices")
        {
          handleControlDevices();
          break;
        }
        else {
          break;
        }
      }
    } while (towardsMenu(&current_state, &destination_state) );
    /* Check if exit menu*/
    if(eventExit == 1)
    {
      break;
    }
  }
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

void ShowInformation(void)
{
  uint8_t event;
  while(1) {

    //picture loop
    u8g2.firstPage(); 
    do {
        // Show title.
        u8g2.drawBox(0, 0, 127, 13);
        u8g2.setColorIndex(0);
        u8g2.setFont(u8g2_font_helvB10_tr);
        u8g2.setCursor(25, 12);
        u8g2.print("Information");
        u8g2.setColorIndex(1);
        
        u8g2.setFont(u8g2_font_4x6_tr);
        u8g2.setCursor(0, 20);
        u8g2.print("TRUONG DH SPKT HO CHI MINH");
        u8g2.setCursor(0, 28);
        u8g2.print("MSSV: 13119017");
        u8g2.setCursor(0, 36);
        u8g2.print("Author: PHAM DUY KHANH");
        u8g2.setCursor(0, 44);
        u8g2.print("Project: SMART CLASS");
        u8g2.setCursor(0, 52);
        u8g2.print("Version: V1.0");
        u8g2.setCursor(0, 60);
        u8g2.print("Press SEL to return to Menu");
    } 
    while( u8g2.nextPage() );
    event = getMenuButton();
    if ( event == 3) {
      break;
    }
  }
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


// Draw a red button
void drawBtn(uint8_t x, uint8_t y, uint8_t state) {
  //Draw frame
  u8g2.drawFrame(x, y, BUTTON_W, BUTTON_H);
  //Draw on or off
  if (state == 0) {
    u8g2.drawBox(x + BUTTON_W/2, y, BUTTON_W/2, BUTTON_H);
    
  }
  else if (state == 1){
    u8g2.drawBox(x, y, BUTTON_W/2, BUTTON_H);
  }
}

void toDownButton(struct menu_state *state)
{
  if ( menu_button_list[state->position + 1].name != NULL )
  {
    state->position++;
    state->frame_position += (int16_t)BUTTON_H + (int16_t)BUTTON_GAP;
    //Serial.println(state->position);
  } 
}

void toUpButton(struct menu_state *state)
{
  if ( state->position > 0 )
  {
    state->position--;
    state->frame_position -= (int16_t)BUTTON_H + (int16_t)BUTTON_GAP;
    Serial.println(state->position);
  }
}

void drawMenuButton(struct menu_state *state) {
  uint8_t i;
  uint8_t y;
  uint8_t x;
  i = 0;
  y = 15;
  x = 21;
  u8g2.firstPage();
  do
  {
    // Show title.
    u8g2.drawBox(0, 0, 127, 13);
    u8g2.setColorIndex(0);
    u8g2.setFont(u8g2_font_helvB10_tr);
    u8g2.setCursor(10, 12);
    u8g2.print("Control Device");
    u8g2.setColorIndex(1);
   
    u8g2.setFont(u8g2_font_4x6_tr);
    // Show menu list of buttons
    while( menu_button_list[i].name  != NULL)  
    {
      u8g2.setCursor(0, x);
      u8g2.print(menu_button_list[i].name);
      if(menu_button_list[i].type == MENU_BUTTON) {
        drawBtn(113, y, menu_button_list[i].state);
      } 
      else {
        //Do nothing
      }
      i++;
      y +=  8;
      x += 8;
    }
    u8g2.drawFrame(0, state->frame_position, 128, BUTTON_H + 2);
    delay(10);
  } while( u8g2.nextPage() );
}
