


#include <windows.h>        
#include <windowsx.h>       
#include "winbgi.h"         
#include "winbgitypes.h"    


palettetype *getdefaultpalette( )
{
    static palettetype default_palette = { 16,
                       { BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY,
                         DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
                         LIGHTMAGENTA, YELLOW, WHITE } };

    return &default_palette;
}


void getpalette( palettetype *palette )
{

}


int getpalettesize( )
{
    return MAXCOLORS + 1;
}


void setallpalette( palettetype *palette )
{

}


void setpalette( int colornum, int color )
{

}


void setrgbpalette( int colornum, int red, int green, int blue )
{

}
