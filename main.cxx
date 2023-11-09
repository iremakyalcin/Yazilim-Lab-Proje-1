#include "graphics.h"  //grafik kütüphanesini başlığını içe aktarır.
#include <iostream>
#include <math.h>
#include <stdlib.h>                     
#include <ctype.h>                       
using namespace std;

const double XBORDER = 150.0;                                                   // ekranın kartezyen x sınırı
const double YBORDER = 112.5;                                                   // ekranın kartezyen y sınırı

const double X_RAD = 2 * XBORDER / 10.0;                        // genişlik
const double Y_RAD = 2 * YBORDER / 12;                  // yükseklik

const double LEFT_X = -1.5 * X_RAD;                     //  sol x koordinat
const double LEFT_Y = 0;                                                // sol y kooridnat
const double RIGHT_X = 1.5 * X_RAD;                     // sağ x koordinat
const double RIGHT_Y = 0;                                               // sağ y koordinat

const double I_RADIUS = X_RAD / 2.8;                                    // irisin yarıçapı
const double P_RADIUS = X_RAD / 10;                                             // pupillanın yarıçapı

// Draws the new irises
void draw_iris(int x, int y); // yeni irisi çizer

int ytranslate
        (double y,                                              // kartezyen kooridnatalrını piksellere çevirir.
    double yborder);                            

int xtranslate
        (double x,                                              //kartezyen koordinatalrını piksellere çevirir.
    double xborder);                            


//gözleri çizer.
void draw_eyes();
void change_left_color(int x, int y);
void change_right_color(int x, int y);
void check_keys();

// fare ile elipsimn merkezinden çizilen çizginin dikey olup olmadığını kontrol eder.
bool check_vertical
        (int x,                                                 
    int y,                                                      
    int x1,                                                     
    int y1,                                                     
    double& x_intersect,                        
    double& y_intersect,                        
    int small_ellipse_x_radius, 
    int small_ellipse_y_radius);

// piksel koordinatalrını gözlerin yarıçapını döndürür.
void get_radius_eyes(int& tx_rad, int& ty_rad);

void update_graphics
        (int left_x_intersect,          // sol iris ve pupille kesişim noktasının x koordinatı 
    int left_y_intersect,               // sol iris ve pupille kesişim noktasının y koordinatı 
    int right_x_intersect,              // sağ iris ve pupille kesişim noktasının x koordinatı 
    int right_y_intersect,              // sağ iris ve pupille kesişim noktasının y koordinatı 
    int ti_rad,                                 // Radius, in pixels, of iris
    int tp_rad,                                 // radius, in pixels, of pupil
    int& t);                                            // Graphics page

void get_radius_iris_pupil
        (int& ti_rad,                                   // irisin piksel cinsinden yarıçapı
    int& tp_rad);                                       // pupillinin piksel cinsinde yarıçapı

void find_coordinates
        (int x,                                                 // farenin x koordinatı konumu
    int y,                                                      // farenin y koordinatı konumu
    int x1,                                                     // elipsin x koordinatı konumu
    int y1,                                                     // elipsin y koordinatı konumu
    double& x_intersect,                        // elipsin küçük elips ile kesitiği yerin x kooridnatı
    double& y_intersect,                        // elipsin küçük elips ile kesitiği yerin y kooridnatı
    int small_ellipse_x_radius, // iris ve pupilin merkez elipsinin x kooridnatı
    int small_ellipse_y_radius);// iris ve pupilin merkez elipsinin y kooridnatı


int left_color = GREEN; //sol renk
int right_color = GREEN; //sağ renl

//windows uygulamasının ana işlevi
int APIENTRY WinMain
( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{

        cout << "Moving Eyes, Copyright (C) 1998 Grant Macklem.\n" << endl;
   cout << "Press 'Q' to quit" << endl;

   initwindow(601, 451); //grafik pğeneceresini başalatır.
    registermousehandler(WM_MOUSEMOVE, draw_iris); //fare hareketlerini işlemek için işlryiciyi kaydeder
    registermousehandler(WM_LBUTTONDOWN, change_left_color); //sol rengi değiştirmek için işleyiciti kaydeder.
    registermousehandler(WM_RBUTTONDOWN, change_right_color); //sağ rengi değiştirmek için işeyiciyi kaydeder.

        // gözleri çizer ancak bakılacak ilk konumu sağlamak gerekir.
   draw_iris(2,2);

        for(;;) 
                        
   {
                check_keys(); //tışlara basılıp basılmadığını kontrol eder.
                delay(10); //küçük bir gecikme ekler.
   }

}

void draw_eyes()  // gözlerin merkezini ve yarıçağplarını piksel koordinatalrına dönüştürür.
{
   int x1 = xtranslate(LEFT_X, XBORDER);
   int y1 = ytranslate(LEFT_Y, YBORDER);
   int x2 = xtranslate(RIGHT_X, XBORDER);
   int y2 = ytranslate(RIGHT_Y, YBORDER);
   int tx_rad = xtranslate(X_RAD, XBORDER) - xtranslate(0, XBORDER);
   int ty_rad = ytranslate(Y_RAD, YBORDER) - ytranslate(0, YBORDER);

   //gözleri beyaz renkte doldurur.
        setcolor(WHITE);
        fillellipse (x1, y1, tx_rad, ty_rad);
   fillellipse (x2, y2, tx_rad, ty_rad);
}

void change_left_color(int x, int y)
{
        left_color ++;                  // renk değerini bir arttırır.
        left_color %= 15;                       //göz renginin beyaz olmasına izin verilmez.
                                                                // renk 0 ile 15 arasında olduğundan emin olunur.
        draw_iris(mousex(),mousey());
}

void change_right_color(int x, int y)
{
        right_color ++;                         // renk değerini bir arttırır.
        right_color %= 15;              // göz renginin beyaz olmasına izin verilmez.
                                                                //  renk 0 ile 15 arasında olduğundan emin olunur.
        draw_iris(mousex(),mousey());
}

void draw_iris(int x, int y)
{
   int tx_rad, ty_rad, ti_rad, tp_rad;
   static int t = 0;            // grafik ekran
   // iris elipsinin piksel koordinatlarıdır.
   double small_ellipse_x_radius;
   double small_ellipse_y_radius;

    //her iki gözün merkezi piksel koordinatlarında
   int x1 = xtranslate(LEFT_X, XBORDER);
   int y1 = ytranslate(LEFT_Y, YBORDER);
   int x2 = xtranslate(RIGHT_X, XBORDER);
   int y2 = ytranslate(RIGHT_Y, YBORDER);
        // bu değişkenler göz merkezinden fareye çizilem çizginin daha küçük eliposi nerede kesiştiğini belirtir.(iris çizmek için)
   double left_x_intersect, left_y_intersect, right_x_intersect, right_y_intersect;
   //gözlerin yarıçaplarını alır.
   get_radius_eyes(tx_rad, ty_rad);
   get_radius_iris_pupil(ti_rad, tp_rad);

        small_ellipse_x_radius = tx_rad - ti_rad;
        small_ellipse_y_radius = ty_rad - ti_rad;

        // Sol Göz
        find_coordinates(x, y, x1, y1, left_x_intersect, left_y_intersect,
        small_ellipse_x_radius, small_ellipse_y_radius);
   // Saş gÖZ
        find_coordinates(x, y, x2, y2, right_x_intersect, right_y_intersect,
        small_ellipse_x_radius, small_ellipse_y_radius);

        //grafikleri günceller
   update_graphics(left_x_intersect, left_y_intersect, right_x_intersect,
                                         right_y_intersect, ti_rad, tp_rad, t);

}


//fare ile elipsin merkezinden çizilen çizginin dikey olup olmadığını kontrol eder.
bool check_vertical
        (int x,                                                 // fare konumunun x kooridnatı
    int y,                                                      // fare konumunun y kooridnatı
    int x1,                                                     // elips konumunun x kooridnatı
    int y1,                                                     // elips konumunun y kooridnatı
    double& x_intersect,                        // irisin küçük elips ile kesiştiği yerin x koordinatı (geri dönüş değeri)
    double& y_intersect,                        // irisin küçük elips ile kesiştiği yerin y koordinatı (geri dönüş değeri)
    int small_ellipse_y_radius) // irisin ve pupilin mekez elipsinin y kooridnatı
{
        // her iki gözün merkezi piksel koordinatları
        if (x == x1)
   {
      x_intersect = x;

                if (abs(y1 - y) >= small_ellipse_y_radius)
      { // işaretçi gözün dışında
        if ((y1 - y) > 0)
                y_intersect = y1 - small_ellipse_y_radius;
         else
                y_intersect = y1 + small_ellipse_y_radius;
      }
      else      // işaretçi gözün içinde
        y_intersect = y;

      return true;
   }
   return false;
}

void find_coordinates
        (int x,                                                 // fare konumunun x kooridnatı
    int y,                                                      // fare konumunun y kooridnatı
    int x1,                                                     // elips konumunun x kooridnatı
    int y1,                                                     // elips konumunun y kooridnatı
    double& x_intersect,                        // irisin küçük elips ile kesiştiği yerin x koordinatı (geri dönüş değeri)
    double& y_intersect,                        // irisin küçük elips ile kesiştiği yerin y koordinatı (geri dönüş değeri)
    int small_ellipse_x_radius, //iris ve pupilin merkez elipsinin x koordinattı
    int small_ellipse_y_radius) //iris ve pupilin merkez elipsinin y koordinattı
{
        bool vertical;
        // kesme noktalarını hesaplamak için kesir değişkenleri
   double numerator, denominator, slope;
   //fare hattının merkezden geçip geçmediğini kontrol eder (dikey mi?)
        vertical = check_vertical(x, y, x1, y1, x_intersect, y_intersect,
        small_ellipse_y_radius);
   if (!vertical)
        {       // eğer dikey değilse merkezden freye çizilen çizginin eğimini hesaplar (piksel kooridnatlarında)
                slope = ((double)(y1 - y)) / ((double)(x - x1));

                // kesirleri hesaplar( piksel kooridnatlarında)
                numerator = pow(small_ellipse_x_radius, 2.0) * pow(small_ellipse_y_radius, 2.0);
           denominator = pow(small_ellipse_y_radius, 2.0) + pow(small_ellipse_x_radius, 2.0) * pow(slope, 2.0);

           //kesme noktalarını hesaplar (piksel koordinatlarında)
                x_intersect = sqrt(numerator / denominator);
           y_intersect = slope * x_intersect;

           if (x < x1)
                x_intersect = -(abs(x_intersect));
           else
                   x_intersect = abs(x_intersect);

           if (y < y1)
                y_intersect = -(abs(y_intersect));
           else
                   y_intersect = abs(y_intersect);

                x_intersect += x1;
                y_intersect += y1;

           if ( (pow(x - x1,2.0) / pow(small_ellipse_x_radius, 2.0) + pow(y1 - y, 2.0) / pow(small_ellipse_y_radius, 2.0)) < 1)
           {    // nokta göz elipsinin içinde.
                x_intersect = x;
              y_intersect = y;
           }
   }    // son
}

//gölzerin yarıçapını piksel kooridnatlarında döndürür.
void get_radius_eyes(int& tx_rad, int& ty_rad)
{
        // gözlerin yarıçapğını hesaplar ve verilen referanlsalra atar.
   tx_rad = xtranslate(X_RAD, XBORDER) - xtranslate(0, XBORDER);
   ty_rad = ytranslate(0, YBORDER) - ytranslate(Y_RAD, YBORDER);
}

//iris ve pupilin yarıçaplarını piksel kooridnatlarında döndürür.
void get_radius_iris_pupil(int& ti_rad, int& tp_rad)
{
        // iiris ve pupillinin yarıçaplarını hesaplar ve verilen referanlsara atar.
   ti_rad = xtranslate(I_RADIUS, XBORDER) - xtranslate(0, XBORDER);
   tp_rad = xtranslate(P_RADIUS, XBORDER) - xtranslate(0, XBORDER);
}

//grafikleri günceller.
void update_graphics(int left_x_intersect, int left_y_intersect,
        int right_x_intersect, int right_y_intersect, int ti_rad, int tp_rad, int& t)
{//grafik saysını değiştirir.
        int active_page = t % 2;

        setactivepage(active_page);
        clearviewport();
        setfillstyle( SOLID_FILL, WHITE );
   draw_eyes(); //gözleri yeniden çizer.
        // sol göz
   setcolor (left_color);
   setfillstyle( SOLID_FILL, left_color );
   fillellipse(left_x_intersect, left_y_intersect, ti_rad, ti_rad);

   // sol pupil
   setcolor (BLACK);
   setfillstyle( SOLID_FILL, BLACK );
   fillellipse(left_x_intersect, left_y_intersect, tp_rad, tp_rad);

        // sağ göz
   setcolor (right_color);
   setfillstyle( SOLID_FILL, right_color );
   fillellipse(right_x_intersect, right_y_intersect, ti_rad, ti_rad);

   // sağ pupil
   setcolor (BLACK);
   setfillstyle( SOLID_FILL, BLACK );
   fillellipse(right_x_intersect, right_y_intersect, tp_rad, tp_rad);

   t++; //grafik sayfa numarasınıu günceller.
   setvisualpage(active_page);
}

//klavyedeki tuşları kontrol eder.
void check_keys()
{
        int command;


        if (kbhit()== true)
   {
      command = toupper(getch());
        if (char(command) == 'Q')
        {
        cout << "Exiting..." << endl;
        exit (EXIT_SUCCESS);
      }
   }
}


//kullanıcı x koordinatını çeviren işlev

int xtranslate (double x, double xborder)
{
      //bu işlev grafik ekranın solundaki -x border (solunda) ile +x border (sağında) değerleini çizmek istediğimizi vasrayar.
    // bu işevin dönüş değeri, x parametresine krşılık gelen x piksel değerinş belirtir.

   double percent;              // kullanıcının tanımladığı kooridnatın bulunduğu x - yarım kğrenin yüzdesi (pozitif veya negatif)
                                                        // user defined coordinate is located
        int maxx = getmaxx();
   int x_value;                 // karşılık gelen grafik x- değeri

   percent = x / xborder;       // kullanıcınıun x- kooridangtında y- eksenine ne kadar uakta olduğunu tyğzde olarak bulur.
                                                                // garfik x- değerini hesaplar, ekranın ortasındaki x- değerimne yüzde olarak kullanıcı tanımlı x'in ne kadar dah auzak olduğunu ekleyerek.
   // ki eğer yüzde negatifse kullanıcı x- koordfinatı negatşfse , aşağıdaki adım çıkarma olacaktır.

   x_value = maxx/2 + percent * (maxx/2);

   return (int)x_value;
}


/// <summary>
/// kullşanıcı y koordinatını çeviren işlev
/// </summary>
/// <param name="y"></param>
/// <param name="yborder"></param>
/// <returns></returns>
int ytranslate (double y, double yborder)
{
       //bu işlev grafik ekranının altında -y border (ekranın altında) ,ile + y border (ekranın sütünde) değerlerini çzimelk istediğinizi varsayar.
    //bu işlevin dönüş deeğeri, y paremetresine karşılık gelern -y piksel değerini belkiritir.

   float percent;               // kullanıcının tanımladığı koordinatın x eksenine ne kadar uzakta olduğunu yüzde olarak bulur. y değerinin beklenenden zıt yçönde olduğu için negatif işarety gerekir.

                                                
        int maxy = getmaxy();
   int y_value;         //

   percent = -y / yborder;      
          //grafik -y değerini hesaplar, ekranın ortasındaki y- değeri üzerine yüzde eolarak kullanıcı tanımlı y'yi ne lkadar daha uzak olduğunu ekleyerek.
          // //ki yüzde negtifse kullanıcı y- kooridnatı negatifse aşağıdaki adım çıkarma oalcaktır.                                                   
                         
   y_value = maxy/2 + percent * (maxy/2);

   return (int)y_value;
}
