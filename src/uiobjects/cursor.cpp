#include "cursor.h"

CursorGestor::CursorGestor(){
    cargarCursores();
}

CursorGestor::~CursorGestor(){
//    for (int i=0;i<arrayCursors.numCursors;i++){
//        SDL_FreeCursor(arrayCursors.Cursor[i]);
//	}
}

void CursorGestor::cargarCursores(){
    uint8_t hiddenCursorData = 0;
    arrayCursors.Cursor[cursor_hidden] = SDL_CreateCursor(&hiddenCursorData, &hiddenCursorData, 8, 1, 0, 0);
    arrayCursors.Cursor[cursor_arrow] = init_system_cursor(CURSORARROW);
    arrayCursors.Cursor[cursor_resize] = init_system_cursor(CURSORHRESIZE);
    arrayCursors.Cursor[cursor_hand] = init_system_cursor(CURSORHAND);
    arrayCursors.Cursor[cursor_wait] = init_system_cursor(CURSORWAIT);

    arrayCursors.numCursors = totalCursors;
}


/**
*
*/
SDL_Cursor * CursorGestor::init_system_cursor(const char *image[]){
  int i, row, col;
  Uint8 data[4*32];
  Uint8 mask[4*32];
  int hot_x, hot_y;
  i = -1;
  for ( row=0; row<32; ++row ) {
    for ( col=0; col<32; ++col ) {
      if ( col % 8 ) {
        data[i] <<= 1;
        mask[i] <<= 1;
      } else {
        ++i;
        data[i] = mask[i] = 0;
      }
      switch (image[4+row][col]) {
        case 'X':
          data[i] |= 0x01;
          break;
        case '.':
          mask[i] |= 0x01;
          break;
        case ' ':
          break;
      }
    }
  }
  sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
  return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}
