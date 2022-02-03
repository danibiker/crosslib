#ifndef UIIMAGEENCODER_H
#define UIIMAGEENCODER_H

#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>             // Must appear before jpeglib, as that uses FILE without including this!
#include <setjmp.h>

#include <jpeglib.h>
#include <jerror.h>            // Gets us the various ERREXIT macros
#include "SDL_imagesave/IMG_saveend.h"
#include <iostream>

using namespace std;


/* =============================================================================
 *  jpeg compressor destination manager implementation
 */

/** The size of the buffer used when writing jpeg images
 */
#define OUTPUT_BUFFER_SIZE 4096

/** A custom jpeg_destination_mgr structure that includes the extra fields
 *  needed to perform the write.
 */
typedef struct {
    struct jpeg_destination_mgr pub;  //!< Standard jpeg destination manager.

    SDL_RWops *rwop;                  //!< The RWops the jpeg is being written to.
    Uint8 buffer[OUTPUT_BUFFER_SIZE]; //!< The buffer used during the write process.
} sdlrw_dest_mgr;

/** Convenience pointer type to make code more readable
 */
typedef sdlrw_dest_mgr * sdlrw_dest_ptr;

/* =============================================================================
 *  jpeg compressor error handler implementation
 */

/** Structure needed to specify an error handler to libjpeg.
 */
typedef struct
{
    struct jpeg_error_mgr errmgr; //!< Standard jpeg error manager, bits of which we will override

    jmp_buf environ;              //!< setjmp/longjump environment, so we can handle errors gracefully.
} sdlrw_error_mgr;

/** Convenience pointer type to aid readability
 */
typedef sdlrw_error_mgr * sdlrw_error_ptr;


class UIImageEncoder
{
    public:
        /** Default constructor */
        UIImageEncoder();
        /** Default destructor */
        virtual ~UIImageEncoder();
        int IMG_SaveJPG(const char *filename, SDL_Surface *surf, int quality);
        int IMG_SaveJPG_RW(SDL_RWops *dest, SDL_Surface *surf, int quality);
        void setDontFreeOnSave(bool var){ dontFreeOnSave = var;}

    protected:
    private:
        static void init_destination(j_compress_ptr cinfo);
        static void jpeg_SDL_RW_dest(j_compress_ptr cinfo, SDL_RWops *rwop);
        static void term_destination(j_compress_ptr cinfo);
        static boolean empty_output_buffer(j_compress_ptr cinfo);
        static int is_usable_format(SDL_Surface *surf);
        static SDL_Surface * make_usable_format(SDL_Surface *surf);
        static void sdlrw_error_exit(j_common_ptr cinfo);
        bool dontFreeOnSave;
};

#endif // UIIMAGEENCODER_H
