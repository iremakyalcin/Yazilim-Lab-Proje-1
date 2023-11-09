


#include <windows.h>        
#include <windowsx.h>       
#include "winbgi.h"        
#include "winbgitypes.h"    


static bool MouseKindInRange( int kind )
{
    return ( (kind >= WM_MOUSEFIRST) && (kind <= WM_MOUSELAST) );
}



bool ismouseclick( int kind )
{
    WindowData *pWndData = BGI__GetWindowDataPtr( );
    return ( MouseKindInRange( kind ) && pWndData->clicks[kind - WM_MOUSEFIRST].size( ) );
}

void clearmouseclick( int kind )
{
    WindowData *pWndData = BGI__GetWindowDataPtr( );

    /
    if ( MouseKindInRange( kind ) && pWndData->clicks[kind - WM_MOUSEFIRST].size( ) )
        pWndData->clicks[kind - WM_MOUSEFIRST].pop( );
}

void getmouseclick( int kind, int& x, int& y )
{
    WindowData *pWndData = BGI__GetWindowDataPtr( );
    POINTS where; /
    
   
    if ( !MouseKindInRange( kind ) )
        return;

    
    if ( MouseKindInRange( kind ) && pWndData->clicks[kind - WM_MOUSEFIRST].size( ) )
    {
	where = pWndData->clicks[kind - WM_MOUSEFIRST].front( );
        pWndData->clicks[kind - WM_MOUSEFIRST].pop( );
        x = where.x;
        y = where.y;
    }
    else
    {
        x = y = NO_CLICK;
    }
}

void setmousequeuestatus( int kind, bool status )
{
    if ( MouseKindInRange( kind ) )
	BGI__GetWindowDataPtr( )->mouse_queuing[kind - WM_MOUSEFIRST] = status;
}


int mousex( )
{
    WindowData *pWndData = BGI__GetWindowDataPtr( );
    return pWndData->mouse.x;
}



int mousey( )
{
    WindowData *pWndData = BGI__GetWindowDataPtr( );
    return pWndData->mouse.y;
}


void registermousehandler( int kind, void h( int, int ) )
{
    WindowData *pWndData = BGI__GetWindowDataPtr( );
    if ( MouseKindInRange( kind ) )
        pWndData->mouse_handlers[kind - WM_MOUSEFIRST] = h;
}
