#ifndef CURSOR_H_INCLUDED
#define CURSOR_H_INCLUDED

#include "Constant.h"
#include "SDL/SDL.h"

        //https://wiki.libsdl.org/SDL_CreateCursor
        /* Stolen from the mailing list */
        /* Creates a new mouse cursor from an XPM */
        /* XPM */
        static const char *CURSORARROW[] = {
          /* width height num_colors chars_per_pixel */
          "    32    32        3            1",
          /* colors */
          "X c #000000",
          ". c #ffffff",
          "  c None",
          /* pixels */
          "X                               ",
          "XX                              ",
          "X.X                             ",
          "X..X                            ",
          "X...X                           ",
          "X....X                          ",
          "X.....X                         ",
          "X......X                        ",
          "X.......X                       ",
          "X........X                      ",
          "X.....XXXXX                     ",
          "X..X..X                         ",
          "X.X X..X                        ",
          "XX  X..X                        ",
          "X    X..X                       ",
          "     X..X                       ",
          "      X..X                      ",
          "      X..X                      ",
          "       XX                       ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "0,0"
        };

        static const char *CURSORWAIT[] = {
          /* width height num_colors chars_per_pixel */
          "    32    32        3            1",
          /* colors */
          "X c #000000",
          ". c #ffffff",
          "  c None",
          /* pixels */
          "XXXXXXXXXXX                     ",
          "X.........X                     ",
          "X.........X                      ",
          " X.......X                      ",
          " X.......X                      ",
          "  X.....X                       ",
          "  X.....X                       ",
          " X.......X                      ",
          " X.......X                      ",
          "X.........X                     ",
          "X.........X                     ",
          "XXXXXXXXXXX                     ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "0,0"
        };

        static const char *CURSORHRESIZE[] = {
          /* width height num_colors chars_per_pixel */
          "    32    32        3            1",
          /* colors */
          "X c #000000",
          ". c #ffffff",
          "  c None",
          /* pixels */
          "   X              X             ",
          "  XX              XX            ",
          " XXX              XXX           ",
          "XXXXXXXXXXXXXXXXXXXXXX          ",
          " XXX              XXX           ",
          "  XX              XX            ",
          "   X              X             ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "                                ",
          "7,0"
        };

        static const char *CURSORHAND[] = {
           "    32    32        3            1",
            "X c #000000",
            ". c #ffffff",
            "  c None",
            "                                ",
            "      XX                        ",
            "     X..X                       ",
            "     X..X                       ",
            "     X..X                       ",
            "     X..X                       ",
            "     X..XXX                     ",
            "     X..X..XXX                  ",
            "     X..X..X..XX                ",
            "     X..X..X..X.X               ",
            " XXX X..X..X..X..X              ",
            " X..XX........X..X              ",
            " X...X...........X              ",
            "  X..X...........X              ",
            "   X.X...........X              ",
            "   X.............X              ",
            "    X............X              ",
            "    X...........X               ",
            "     X..........X               ",
            "     X..........X               ",
            "      X........X                ",
            "      X........X                ",
            "      XXXXXXXXXX                ",
            "                                ",
            "                                ",
            "                                ",
            "                                ",
            "                                ",
            "                                ",
            "                                ",
            "                                ",
            "                                ",
            "7,0"};


class CursorGestor{
    public :
        CursorGestor();
        ~CursorGestor();

        SDL_Cursor * getCursor(int i){
            return arrayCursors.Cursor[i];
        }

    private:
        void cargarCursores();
        SDL_Cursor * init_system_cursor(const char *image[]);

        struct my_struct_arrays {
          SDL_Cursor *Cursor[totalCursors];
          int numCursors;
        } arrayCursors;
};


#endif // CURSOR_H_INCLUDED
