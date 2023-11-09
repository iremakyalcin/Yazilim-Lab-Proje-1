

#include <windows.h>        
#include <windowsx.h>      
#include <iostream>
#include <sstream>         
#include <string>          
#include "winbgi.h"         
#include "winbgitypes.h"    



std::ostringstream bgiout;
static int font_weight[] =
{
    FW_BOLD,    // DefaultFont
    FW_NORMAL,  // TriplexFont
    FW_NORMAL,  // SmallFont
    FW_NORMAL,  // SansSerifFont
    FW_NORMAL,  // GothicFont
    FW_NORMAL,  // ScriptFont
    FW_NORMAL,  // SimplexFont
    FW_NORMAL,  // TriplexScriptFont
    FW_NORMAL,  // ComplexFont
    FW_NORMAL,  // EuropeanFont
    FW_BOLD     // BoldFont
};
static int font_family[] =
{
    FIXED_PITCH|FF_DONTCARE,     // DefaultFont
    VARIABLE_PITCH|FF_ROMAN,     // TriplexFont
    VARIABLE_PITCH|FF_MODERN,    // SmallFont
    VARIABLE_PITCH|FF_DONTCARE,  // SansSerifFont
    VARIABLE_PITCH|FF_SWISS,     // GothicFont
    VARIABLE_PITCH|FF_SCRIPT,    // ScriptFont
    VARIABLE_PITCH|FF_DONTCARE,  // SimplexFont
    VARIABLE_PITCH|FF_SCRIPT,    // TriplexScriptFont
    VARIABLE_PITCH|FF_DONTCARE,  // ComplexFont
    VARIABLE_PITCH|FF_DONTCARE,  // EuropeanFont
    VARIABLE_PITCH|FF_DONTCARE   // BoldFont
};
static char* font_name[] =
{
    "Console",          // DefaultFont
    "Times New Roman",  // TriplexFont
    "Small Fonts",      // SmallFont
    "MS Sans Serif",    // SansSerifFont
    "Arial",            // GothicFont
    "Script",           // ScriptFont
    "Times New Roman",  // SimplexFont
    "Script",           // TriplexScriptFont
    "Courier New",      // ComplexFont
    "Times New Roman",  // EuropeanFont
    "Courier New Bold", // BoldFont
};

static struct { int width; int height; } font_metrics[][11] = {
    {{0,0},{8,8},{16,16},{24,24},{32,32},{40,40},{48,48},{56,56},{64,64},{72,72},{80,80}}, // DefaultFont
    {{0,0},{13,18},{14,20},{16,23},{22,31},{29,41},{36,51},{44,62},{55,77},{66,93},{88,124}}, // TriplexFont
    {{0,0},{3,5},{4,6},{4,6},{6,9},{8,12},{10,15},{12,18},{15,22},{18,27},{24,36}}, // SmallFont
    {{0,0},{11,19},{12,21},{14,24},{19,32},{25,42},{31,53},{38,64},{47,80},{57,96},{76,128}}, // SansSerifFont
    {{0,0},{13,19},{14,21},{16,24},{22,32},{29,42},{36,53},{44,64},{55,80},{66,96},{88,128}}, // GothicFont

    // These may not be 100% correct
    {{0,0},{11,19},{12,21},{14,24},{19,32},{25,42},{31,53},{38,64},{47,80},{57,96},{76,128}}, // ScriptFont
    {{0,0},{11,19},{12,21},{14,24},{19,32},{25,42},{31,53},{38,64},{47,80},{57,96},{76,128}}, // SimplexFont
    {{0,0},{13,18},{14,20},{16,23},{22,31},{29,41},{36,51},{44,62},{55,77},{66,93},{88,124}}, // TriplexScriptFont
    {{0,0},{11,19},{12,21},{14,24},{19,32},{25,42},{31,53},{38,64},{47,80},{57,96},{76,128}}, // ComplexFont
    {{0,0},{11,19},{12,21},{14,24},{19,32},{25,42},{31,53},{38,64},{47,80},{57,96},{76,128}}, // EuropeanFont
    {{0,0},{11,19},{12,21},{14,24},{19,32},{25,42},{31,53},{38,64},{47,80},{57,96},{76,128}} // BoldFont
};
static UINT horiz_align[3] = {TA_LEFT, TA_CENTER, TA_RIGHT};
static UINT vert_align[3] = {TA_BOTTOM, TA_BASELINE, TA_TOP};


static void set_align(WindowData* pWndData)
{
    UINT alignment = pWndData->alignment;

    
    if (pWndData->textInfo.direction == HORIZ_DIR)
    {
	alignment |= horiz_align[pWndData->textInfo.horiz];
	alignment |= vert_align[pWndData->textInfo.vert];
    }
    else
    {
	alignment |= horiz_align[pWndData->textInfo.vert];
	alignment |= vert_align[pWndData->textInfo.horiz];
    }

  
    for ( int i = 0; i < MAX_PAGES; i++ )
	SetTextAlign(pWndData->hDC[i], alignment);
}


static void set_font(WindowData* pWndData)
{
    int mindex;
    double xscale, yscale;
    HFONT hFont;
    
    
    if(pWndData->textInfo.charsize == 0)
    {
	xscale = pWndData->t_scale[0] / pWndData->t_scale[1];
	yscale = pWndData->t_scale[2] / pWndData->t_scale[3];
		
	
	if (pWndData->textInfo.font == 0)
	    mindex = 0;
	else
	    mindex = 4;
    }
    else
    {
	xscale = 1.0;
	yscale = 1.0;
	mindex = pWndData->textInfo.charsize;
    }

    
    hFont = CreateFont(
	int(font_metrics[pWndData->textInfo.font][mindex].height * yscale), 
	int(font_metrics[pWndData->textInfo.font][mindex].width  * xscale),
	pWndData->textInfo.direction * 900,
	(pWndData->textInfo.direction & 1) * 900,
	font_weight[pWndData->textInfo.font],
	FALSE,
	FALSE,
	FALSE,
	DEFAULT_CHARSET,
	OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS,
	DEFAULT_QUALITY,
	font_family[pWndData->textInfo.font],
	font_name[pWndData->textInfo.font]
	);

    
    for ( int i = 0; i < MAX_PAGES; i++ )
	SelectObject( pWndData->hDC[i], hFont );
}


void gettextsettings(struct textsettingstype *texttypeinfo)
{
    
    if (!texttypeinfo)
	return;
	
    WindowData* pWndData = BGI__GetWindowDataPtr( );

    *texttypeinfo = pWndData->textInfo;	
}



void outtext(char *textstring)
{
    HDC hDC = BGI__GetWinbgiDC( );
    WindowData* pWndData = BGI__GetWindowDataPtr( );

   
    POINT cp;
    GetCurrentPositionEx( hDC, &cp );

    
    if (pWndData->alignment != TA_UPDATECP)
    {
	pWndData->alignment = TA_UPDATECP;
	set_align(pWndData);
    }

    TextOut(hDC, 0, 0, (LPCTSTR)textstring, strlen(textstring));
    BGI__ReleaseWinbgiDC( );
    RefreshWindow( NULL );
    
}


void outtextxy(int x, int y, char *textstring)
{
    HDC hDC = BGI__GetWinbgiDC( );
    WindowData* pWndData = BGI__GetWindowDataPtr( );

  
    if (pWndData->alignment != TA_NOUPDATECP)
    {
	pWndData->alignment = TA_NOUPDATECP;
	set_align(pWndData);
    }

    TextOut(hDC, x, y, (LPCTSTR)textstring, strlen(textstring));
    BGI__ReleaseWinbgiDC( );

    RefreshWindow( NULL );

}




void settextjustify(int horiz, int vert)
{
    WindowData* pWndData = BGI__GetWindowDataPtr( );

    pWndData->textInfo.horiz = horiz;
    pWndData->textInfo.vert  = vert;

    WaitForSingleObject(pWndData->hDCMutex, 5000);
    set_align(pWndData);
    ReleaseMutex(pWndData->hDCMutex);
}



void settextstyle(int font, int direction, int charsize)
{
    WindowData* pWndData = BGI__GetWindowDataPtr( );

    pWndData->textInfo.font = font;
    pWndData->textInfo.direction = direction;
    pWndData->textInfo.charsize = charsize;

    WaitForSingleObject(pWndData->hDCMutex, 5000);
    set_font(pWndData);
    ReleaseMutex(pWndData->hDCMutex);
}


void setusercharsize(int multx, int divx, int multy, int divy)
{
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    pWndData->t_scale[0] = multx;
    pWndData->t_scale[1] = divx;
    pWndData->t_scale[2] = multy;
    pWndData->t_scale[3] = divy;

    WaitForSingleObject(pWndData->hDCMutex, 5000);
    set_font(pWndData);
    ReleaseMutex(pWndData->hDCMutex);
}


int textheight(char *textstring)
{
    HDC hDC = BGI__GetWinbgiDC( );
    SIZE tb;

    GetTextExtentPoint32(hDC, (LPCTSTR) textstring, strlen(textstring), &tb);
    BGI__ReleaseWinbgiDC( );

    return tb.cy;
}


int textwidth(char *textstring)
{
    HDC hDC = BGI__GetWinbgiDC( );
    SIZE tb;

    GetTextExtentPoint32(hDC, (LPCTSTR) textstring, strlen(textstring), &tb);
    BGI__ReleaseWinbgiDC( );

    return tb.cx;
}

void outstreamxy(int x, int y, std::ostringstream& out)
{
    std::string all, line;
    int i;
    int startx = x;

    all = out.str( );
    out.str("");

    moveto(x,y);
    for (i = 0; i < all.length( ); i++)
    {
	if (all[i] == '\n')
	{
	    if (line.length( ) > 0)
	    	outtext((char *) line.c_str( ));
	    y += textheight("X");
	    x = startx;
	    line.clear( );
	    moveto(x,y);
	}
	else
	    line += all[i];
    }
    if (line.length( ) > 0)
	outtext((char *) line.c_str( ));
}

void outstream(std::ostringstream& out)
{
    outstreamxy(getx( ), gety( ), out);
}

