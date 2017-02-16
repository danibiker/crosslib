#include "uiimgencoder.h"

UIImageEncoder::UIImageEncoder(){
    dontFreeOnSave = false;
    //ctor
}

UIImageEncoder::~UIImageEncoder(){
    //dtor
}

/** Initialize destination, called by jpeg_start_compress before any data
 *  is actually written.
 *
 *  \param cinfo A pointer to the jpeg compressor instance.
 */
void UIImageEncoder::init_destination(j_compress_ptr cinfo)
{
    sdlrw_dest_ptr dest = (sdlrw_dest_ptr)cinfo -> dest;

    // We probably don't need to do this, as jpeg_SDL_RW_dest will set them,
    // but better to be safe than sorry...
    dest -> pub.next_output_byte = dest -> buffer;
    dest -> pub.free_in_buffer = OUTPUT_BUFFER_SIZE;
}

/** Empty the output buffer, called whenever buffer fills up.
 *  This function writes the whole contents of the buffer to the RWops
 *  specified when creating the destination manager, and then resets
 *  the buffer settings for another write.
 *
 *  \param cinfo A pointer to the jpeg compressor instance.
 *  \return Non-zero value indicating that the buffer has been written.
 */
boolean UIImageEncoder::empty_output_buffer(j_compress_ptr cinfo)

{
    sdlrw_dest_ptr dest = (sdlrw_dest_ptr)cinfo -> dest;

    // Write the whole buffer out, fall over if we can't write it all
    if(SDL_RWwrite(dest -> rwop, dest -> buffer, 1, OUTPUT_BUFFER_SIZE) != OUTPUT_BUFFER_SIZE) {
        ERREXIT(cinfo, JERR_FILE_WRITE);
    }

    // Reset the free space and next byte pointers
    dest -> pub.next_output_byte = dest -> buffer;
    dest -> pub.free_in_buffer = OUTPUT_BUFFER_SIZE;

    return true;
}


/** Terminate destination, called by jpeg_finish_compress after all data
 *  has been written. This will write any data remaining in the buffer to
 *  the RWops; it will not do any flushing, that is left up to the RWops
 *  to do for you.
 *
 *  \param cinfo A pointer to the jpeg compressor instance.
 */
void UIImageEncoder::term_destination(j_compress_ptr cinfo)
{
    sdlrw_dest_ptr dest = (sdlrw_dest_ptr)cinfo -> dest;

    // Work out whether we have any data left to write in the buffer
    size_t remain = OUTPUT_BUFFER_SIZE - dest -> pub.free_in_buffer;

    // Write any data remaining in the buffer
    if(remain > 0) {
        if( ((size_t)SDL_RWwrite(dest -> rwop, dest -> buffer, 1, remain)) != remain) {
            ERREXIT(cinfo, JERR_FILE_WRITE);
        }
    }

    // No need to do anything else - SDL should handle flushing if needed
}


/** Set up a SDL RW destination for a jpeg compressor. This will allow libjpeg
 *  to write data to any SDL RW destination.
 *
 *  \param cinfo A pointer to the jpeg compressor instance.
 *  \param rwop
 */
void UIImageEncoder::jpeg_SDL_RW_dest(j_compress_ptr cinfo, SDL_RWops *rwop)
{
    sdlrw_dest_ptr dest;

    // Create a new destination manager structure if this instance of the
    // compressor does not have it set already
    if(cinfo -> dest == NULL) {
        cinfo -> dest = (struct jpeg_destination_mgr *)(*cinfo -> mem -> alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT, sizeof(sdlrw_dest_mgr));
    }

    // And fill in our various fields...
    dest = (sdlrw_dest_ptr)cinfo -> dest;
    dest -> rwop                    = rwop;
    dest -> pub.init_destination    = UIImageEncoder::init_destination;
    dest -> pub.empty_output_buffer = UIImageEncoder::empty_output_buffer;
    dest -> pub.term_destination    = UIImageEncoder::term_destination;
    dest -> pub.next_output_byte    = dest -> buffer;
    dest -> pub.free_in_buffer      = OUTPUT_BUFFER_SIZE;
}

/* =============================================================================
 *  Pixel format handling code
 */

/** Determine whether the pixel format of the specified surface matches the
 *  needs of the jpeg writer. This function checks the pixel format specified
 *  to determine whether the surface is a 24 bit image, with pixels ordered in
 *  memory as R, G, B (8 bit values, red first, then green, then blue).
 *
 *  \param surf The surface to check for usable format.
 *  \return true if the format is usable, false otherwise.
 */
int UIImageEncoder::is_usable_format(SDL_Surface *surf)
{
    SDL_PixelFormat *fmt = surf -> format;

    return (fmt -> BytesPerPixel == 3 &&
            fmt -> Rmask == RMASK24 &&
            fmt -> Gmask == GMASK24 &&
            fmt -> Bmask == BMASK24 &&
            fmt -> Aloss == 8); // Aloss is 8 when there is no alpha channel to use.
}


/** Create a copy of an SDL surface using a bit format that can be fed straight
 *  to libjpeg. This will create a surface in 24 bit R, G, B in bigendian format
 *  so that libjpeg can process it directly, avoiding the need for on-the-fly
 *  format conversion shenanigans.
 *
 *  \param surf The surface to convert.
 *  \return A pointer to a copy of the surface in the usable format. The caller must
 *          free this surface when done with it.
 */
SDL_Surface * UIImageEncoder::make_usable_format(SDL_Surface *surf)
{
    // This is the pixel format for a 24 bit, 3 byte per pixel, RGB image in bigendian form
    // rows of an image in this format can be pushed straight into libjpeg as needed
    SDL_PixelFormat pf_temp = { NULL, 24, 3,
                                0, 0, 0, 8,
                                RSHIFT24, GSHIFT24, BSHIFT24, 0,
                                RMASK24, GMASK24, BMASK24, 0,
                                0, 255 };

    // Make a copy of our errant surface
    SDL_Surface *rgb_surf = SDL_ConvertSurface(surf, &pf_temp, SDL_SWSURFACE);

    return rgb_surf;
}


/** Fatal error handler function. This is called when libjpeg encounters a fatal
 *  error during processing, and it longjmp()s to the error handling code in
 *  IMG_SaveJPG_RW().
 *
 *  \param cinfo A pointer to the jpeg compressor instance that encountered an error.
 *  \note DOES NOT RETURN
 */
void UIImageEncoder::sdlrw_error_exit(j_common_ptr cinfo)
{
//    sdlrw_error_ptr err = (sdlrw_error_ptr)cinfo -> err;
//    longjmp(err->environ, 1);
}


/* =============================================================================
 *  exposed code
 */

/** Write the specified surface to a file at the requested quality.
 *  This function will discard any alpha channel (jpeg does not support alpha).
 *  If the specified surface is palletised, a temporary truecolour version of
 *  it will be created and written. This does not support greyscale jpegs.
 *
 *  \param filename The name of the file to save to.
 *  \param surf     The surface to write as a jpeg.
 *  \param quality  The jpeg quality, in the range 0 to 100. If the specified value
 *                  exceeds this range, it will be clamped.
 *  \return -1 on error, 0 if the jpeg was saved successfully.
 */
int UIImageEncoder::IMG_SaveJPG(const char *filename, SDL_Surface *surf, int quality)
{
    SDL_RWops *out;

    // Open a RWops so we can write to a file using IMG_SaveJPG_RW
    if(!(out = SDL_RWFromFile(filename, "wb"))) {
        return (-1);
    }

    // Write the jpeg out...
    int result = IMG_SaveJPG_RW(out, surf, quality);

    // and we're done
    SDL_RWclose(out);
    return result;
}


/** Write the specified surface to a SDL RWops data source at the requested quality.
 *  This function will discard any alpha channel (jpeg does not support alpha).
 *  If the specified surface is palletised, a temporary truecolour version of
 *  it will be created and written. This does not support greyscale jpegs.
 *
 *  \param dest    The SDL_RWops to write the surface to.
 *  \param surf    The surface to write as a jpeg.
 *  \param quality The jpeg quality, in the range 0 to 100. If the specified value
 *                 exceeds this range, it will be clamped.
 *  \return -1 on error, 0 if the jpeg was saved successfully.
 */
int UIImageEncoder::IMG_SaveJPG_RW(SDL_RWops *dest, SDL_Surface *surf, int quality)
{
    struct jpeg_compress_struct cinfo;
    sdlrw_error_mgr jerr;
    SDL_Surface *outsurf = surf;
    Uint8 *line;
    int y;
//    int start;         // rw position on invoking this function, for error handling
    int isRGB;         // will be set to true if the surface is already in a usable form

    // Clamp the quality
    if(quality < 0)
        quality = 0;
    if(quality > 100)
        quality = 100;

    // Do nothing if we have no destination or surface
    if(!dest) {
        SDL_SetError("No destination RWops specified.");
        return -1;
    }

    if(!surf) {
        SDL_SetError("No surface specified.");
        return -1;
    }

    // Determine whether the surface is in a usable format, and convert it if not
    // In reality, this is probably going to end up making the converted image in
    // 99.9% of cases on modern machines.
    isRGB = is_usable_format(surf);
    if(!isRGB && !(outsurf = make_usable_format(surf))) {
        SDL_SetError("Unable to create temporary surface");
        return -1;
    }

    // Determine the current position in the RW so we can restore it on errors
//    start = SDL_RWtell(dest);

    // Set up error handling that plays nice with SDL
    cinfo.err = jpeg_std_error(&jerr.errmgr);
    jerr.errmgr.error_exit = sdlrw_error_exit;
#ifdef SUPPRESS_WARNINGS
    jerr.errmgr.output_message = output_no_message;
#endif



    // And mark where we want to come out if there is a fatal error
//    if(setjmp(jerr.environ)) {

//        // something bad happened in libjpeg or the dest manager, clean up and give up.
//        jpeg_destroy_compress(&cinfo);
//        if(!isRGB) SDL_FreeSurface(outsurf);
//
//        // Restore the position of the RWops to the location it was at when we started
//        SDL_RWseek(dest, start, RW_SEEK_SET);
//        SDL_SetError("JPEG saving error, giving up.");
//        return -1;
//    }

    // Make a compressor, and set it to use a SDL_RWops as the output
    jpeg_create_compress(&cinfo);
    jpeg_SDL_RW_dest(&cinfo, dest);

    // Set up the important compressor settings
    cinfo.in_color_space   = JCS_RGB;
    cinfo.image_width      = surf -> w;
    cinfo.image_height     = surf -> h;
    cinfo.input_components = 3;
    cinfo.dct_method       = JDCT_FLOAT;

    // And finish off the settings and start the compressor
    jpeg_set_defaults    (&cinfo);
    jpeg_set_quality     (&cinfo, quality, TRUE);
    jpeg_start_compress  (&cinfo, TRUE);

    // SDL does this lock/write/unlock when writing BMP so I'm fairly sure it's safe
    if(SDL_MUSTLOCK(outsurf)) SDL_LockSurface(outsurf);

    // Write out the jpeg...
    for(y = 0, line = (Uint8 *)outsurf->pixels; y < outsurf -> h; y++, line += outsurf->pitch) {
        jpeg_write_scanlines(&cinfo, &line, 1);
    }

    if(SDL_MUSTLOCK(outsurf)) SDL_UnlockSurface(outsurf);

    // Free the surface if we allocated a copy earlier
    if(!isRGB && !dontFreeOnSave) SDL_FreeSurface(outsurf);

    // And done here...
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    return 0;
}

