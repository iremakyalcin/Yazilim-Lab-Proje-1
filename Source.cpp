#include "graphics.h"     //graphics.h başlık dosyası grafik işlemleri için kullanılır.
#include <iostream>        // genel kütüphane     

using namespace std;

int undoCount = 1;      //geri alma işlemi sayacı

enum Tools { squareShape, circleShape, rectangleShape, triangleShape, polygonShape, lineShape,
			pencilTool, eraserTool, bucketTool, textTool, colorPallete, undoTool, redoTool,
			saveTool, loadTool, deleteTool, quitTool,
	};   //kullanılan araçları temsil eden enum türü

//mouse tıklamalarını algılayan ve işleyen işlevlerin prototipleri
Tools ToolSelection(int *xy);
int* clickCheck();
//paletleri gösteren işlev
void Palletes();
//seçili aracın girsel göstergelerini çizen işlev
void SelectionIndicator(Tools selection);
//çizimleri kaydetmeye ve yüklemeye yardımcı olan işlevşerin prototipleri
void Save();
void Load();
//geri alma ve yeniden yapma işlemlerini yöneten işlevlerin prototipleri
void Undo(int size);
void Redo(int size);

//şekil sınıfı tanımı ve yöntemleri
class Shape {
private:
	int fillColor;
protected:
	int *points;
	int coordinates;
	int maxx, maxy;
public:
	Shape() : points(nullptr), coordinates(0), fillColor(WHITE) {
		maxx = getmaxx();
		maxy = getmaxy();
	}
	//arabirim için kullanılan işlev
	void Interface() {
		readimagefile("canvas.jpg", 0, 0, maxx, maxy);
	}
	//panel için kullanılan işlev
	void Panel() {
		readimagefile("panel.jpg", 0, 0, 135, 670);   //panel görüntüsünü yükler
		readimagefile("UndoRedo.jpg", 910, 1, 1070, 65);  //geri alma, yeniden yapma görüntüsünü yükler
		setcolor(BLACK);
		circle(92, 327, 15);  //daire çiz
		setfillstyle(SOLID_FILL, fillColor);
		floodfill(92, 327, BLACK); //renk seçici simge
	}
	void Pencil() {
		setcolor(fillColor);  //kalem rengini ayarla
		coordinates = 2;     //koordinat sayısını belirle
		points = new int[coordinates];  //Koordinatlar için dinamik bellek tahsisi
		while (1) {
			delay(10); //fare tıklamalarını kontrol eder
			if (GetAsyncKeyState(VK_LBUTTON)) {    //sol fare tıklanması
				getmouseclick(WM_MOUSEMOVE, points[0], points[1]);   //farenin konumunu alır
				if (points[0] > 135 && points[0] < maxx && points[1] > 0 && points[1] < maxy) {  //sadece tuval içinde çizim yapar
					circle(points[0], points[1], 3); //küçük daire çizer
					setfillstyle(SOLID_FILL, fillColor);  //daireyi doldurur (seçilen renkle)
					floodfill(points[0], points[1], fillColor);
				}
			}
			if (GetAsyncKeyState(VK_RBUTTON)) { //sağ fareye tıklanırsa döngüyü kır
				break;
			}
		}
		delete[] points; //dinamik belleği serbest bırak
	}
	void Eraser() {
		setcolor(WHITE); //silgi rengini beyaz yapar
		coordinates = 2;  //koordinat sayısını belirler
		points = new int[coordinates];  //koordşnatlar için dinamik bellek tahsisi
		while (1) {
			delay(10);  //fare tıklamalarını kontrol eder
			if (GetAsyncKeyState(VK_LBUTTON)) {  //sol fare tıklaması durumunda
				getmouseclick(WM_MOUSEMOVE, points[0], points[1]); //farenin konumunu al
				if (points[0] > 135 && points[0] < maxx && points[1] > 0 && points[1] < maxy) { //sadece tuval içinde silme yapar
					circle(points[0], points[1], 20);  //büyük daire çizer 
					setfillstyle(SOLID_FILL, WHITE); //daireyi doldur (beyaz renkle)
					floodfill(points[0], points[1], WHITE);
				}
			}
			if (GetAsyncKeyState(VK_RBUTTON)) { //sağ fare tılanırsa döngüyü kır
				break;
			}
		}
		delete[] points;  //ddinamik belleği serbest bırak
	}
	void Bucket(int *xy) {
		coordinates = 2; //koordinat sayısını belirler
		points = new int[coordinates];  //koordinatlar için dinamik bellek tahsisi
		points[0] = xy[0]; points[1] = xy[1];//fare tıklamasının koordinatlarını alır
		setfillstyle(SOLID_FILL, fillColor); //dolgu rengini belirler
		floodfill(points[0], points[1], BLACK); //belirtilen noktadan başlayarak doldur (sadece boş bölgeleri)
		delete[] points;   //dinamik belleği serbest bırak
	}
	void selectColor() {  
		Palletes();  //renk paletini çağırır
		coordinates = 2; //kooridmnat sayısını belirler
		points = new int[coordinates];  //koordinatlar için dinamik bellek tahsisi
		clearmouseclick(WM_LBUTTONDOWN);  //sol fare tıklamalarını temizler
		while (!ismouseclick(WM_LBUTTONDOWN)) {}//sol fare düğmesine tıklanana kadar bekleyin 
		getmouseclick(WM_LBUTTONDOWN, points[0], points[1]); //sol fare düğmesi tıklanınca tıklama koordinatlarını alır
		if (points[0] > 5 && points[0] < 130 && points[1] > 360 && points[1] < 630)
			fillColor = getpixel(points[0], points[1]); //seçilen rengi al
		setfillstyle(SOLID_FILL, fillColor); //dolum rengini ayaralr
		floodfill(92, 327, BLACK);  //seçilen renkle doldurur
		delete[] points;  //dinamik belleği serbest bırakır
	}
	void Text(int *xy) {
		points = new int[coordinates];  //koordinatlar için dinamik bellek tahsisi
		char str[50] = {NULL}; //metin verisini saklamak için karakter dizisi
		points[0] = xy[0]; points[1] = xy[1]; //başalangıç koordinatlarını ayarla
		setbkcolor(WHITE);//arkaplan rengini beyaz yap
		setcolor(fillColor); //metin rengini seçilen renkle ayarla
		setfillstyle(0, RED); //dolum desenini belirler
		settextstyle(SIMPLEX_FONT, 0, 1);  //boyut 1
		str[0] = '\0'; //boş bir metin başlat
		int index = 0; //karakter dizisisini takip etmek için kullanılan indis
		do {
			char ch = getch(); //kullanıcıın girdiğini alır
			if (ch == '\b') {
				index--;
				str[index] = '\0';  //geri tuşuna basıldığında son karekteri siler
			}
			else {
				str[index] = ch; //kullanıcının girdiği kaarakteri metin dizisine ekler
				index++; //metni ekranda gösterir
			}
			outtextxy(points[0], points[1], str);
		} while (!ismouseclick(WM_LBUTTONDOWN));   //sol fare düğmesi tıklanana kadar döngüyü devam ettirir.
	}
	virtual void Draw(int* xy) = 0;
	virtual void getShape() = 0;
	virtual ~Shape() {
		delete[]points;
	}
};

class RectangleShape : public Shape {   //
public:
	RectangleShape() {
		coordinates = 4; //koordinat sayısını belirle (2 çift x ve y kooridnatı)
		points = new int[coordinates];  //koordinatlar için dibnamik bellek tahsisi
	}
	void Draw(int *xy) {
		setcolor(BLACK); //çizgi rengini siyah yapar
		points[0] = xy[0]; points[1] = xy[1];//başlangıç koordinatlarını ayarlar
		clearmouseclick(WM_LBUTTONDOWN); //sol fare düğmesi tıklamalarını temizler 
		while (!ismouseclick(WM_LBUTTONDOWN)) {}  //sol fare düğmesi tıjklanana kadar bekle
		getmouseclick(WM_LBUTTONDOWN, points[2], points[3]); //ikinci tıklama koordinatalrını alır
		rectangle(points[0], points[1], points[2], points[3]);   //dikdörtgen çiz
	}
	void getShape() {
		rectangle(points[0], points[1], points[2], points[3]);  //dikdörtgeni ekranda göster
	}
};

class Square : public RectangleShape {
public:
	void Draw(int *xy) {
		setcolor(BLACK); //çizgi rengini siyah yap
		points[0] = xy[0]; points[1] = xy[1]; //başlangıç koordinatlarını ayarla
		int length;
		clearmouseclick(WM_LBUTTONDOWN); // sol fare düğmesi tıklamalarını temizle
		while (!ismouseclick(WM_LBUTTONDOWN)) {} //sol fare düğmesi tıklanana kadar bekle
		getmouseclick(WM_LBUTTONDOWN, points[2], points[3]);  //ikinci tıklama koordinatlarını al
		length = points[2] - points[0];
		if (points[0] > points[2])
			rectangle(points[0], points[1], points[0] + length, points[1] - length);
		else
			rectangle(points[0], points[1], points[0] + length, points[1] + length);
	}
	void getShape() {
		rectangle(points[0], points[1], points[2], points[3]); //kare çizer ve ekranda gösterir
	}
};

class Circle : public Shape {
	int radius;
public:
	Circle() {
		coordinates = 4; //koordinat saysını belirler
		points = new int[coordinates]; //koordinatlar için dinbamik bellek tahsisi
		radius = 0;// başlangıç yarıçağoını sıfıra ayarla
	}
	void Draw(int *xy) {
		setcolor(BLACK); //çizgi rengini siyah yapar
		points[0] = xy[0]; points[1] = xy[1]; //başlangıç koordinatlarını ayarlar.
		clearmouseclick(WM_LBUTTONDOWN); //sol fare düğmesi tıklamalrınu temizler
		while (!ismouseclick(WM_LBUTTONDOWN)) {} //sol fare düğmesi tıklanana kadar bekle
		getmouseclick(WM_LBUTTONDOWN, points[2], points[3]); //ikinci tıklama kooridnatlarını alır
		radius = points[2] - points[0]; //yarıçap hesaplar
		circle(points[0], points[1], radius); //çember çizer
	}
	void getShape() {
		circle(points[0], points[1], radius); //çemberi ekrnada gösterir
	}
};

class LineShape : public Shape {
public:
	LineShape() {
		coordinates = 4; //koordinat saysını belirler
		points = new int[coordinates]; //koordinatlar için dinamik bellek tahsisi
	}
	void Draw(int *xy) {
		setcolor(BLACK); //çizgi rengibi siyah yap
		points[0] = xy[0]; points[1] = xy[1]; //başlangıç koordinatlarını ayarla
		clearmouseclick(WM_LBUTTONDOWN); //sol fare düğmesi tıklamalarını temzle
		while (!ismouseclick(WM_LBUTTONDOWN)) {}// sol fare düğmesine tıklanana kadar bekle
		getmouseclick(WM_LBUTTONDOWN, points[2], points[3]); //ikinci tıklama koordimnatalrını al
		line(points[0], points[1], points[2], points[3]);//çizgi çizer
	}
	void getShape() {
		line(points[0], points[1], points[2], points[3]);v//çizgiyi ekranda gösterir
	}
};

class Polygon : public Shape {
public:
	Polygon() {}
	void Draw(int *xy) {
		points[0] = xy[0]; points[1] = xy[1]; //başlangıç koordinatalrını ayarlar
		clearmouseclick(WM_LBUTTONDOWN); //sol fare düğmesi tıklamalarını temizler
		while (!ismouseclick(WM_LBUTTONDOWN)) {} //sol fare düğmesitıklanana kadar bekle
		getmouseclick(WM_LBUTTONDOWN, points[2], points[3]); //ikinci tıklama kooridnatlarını alır
		while (!ismouseclick(WM_LBUTTONDOWN)) {}//üçüncü tıklamayı bekler
		getmouseclick(WM_LBUTTONDOWN, points[4], points[5]); //üçüncü tıklama koordinatalrını alır
	}
};

class Triangle : public Polygon {
public:
	Triangle() {
		coordinates = 8; //koordinat sayısını belirle (3 x ve 3y)
		points = new int[coordinates];// koordinatlar için dinamik bellek tahsisi
	}
	void Draw(int *xy) {
		setcolor(BLACK); //çizgi rengini siyah yapar
		this->Polygon::Draw(xy);  //alt sınıfın draw işlevini çağırarak üç nokta seçimi yapar
		points[6] = points[0]; //üçgeni tamamlamak için başlangıç noktasını kullanır
		points[7] = points[1];
		drawpoly(coordinates / 2, points); //üçgeni çiz
	}
	void getShape() {
		drawpoly(coordinates / 2, points); //üçgeni ekranda göster
	}
};

class Hexagon : public Polygon {
public:
	Hexagon() {
		coordinates = 100; //koordinat sayısını belirler (çok fazlaa, kullanılan koordinat sayısına göre düzenlenmeli)
		points = new int[coordinates]; //kooridnatlar için dinamik bellek tahsisi
	}
	void Draw(int *xy) {
		setcolor(BLACK); //çizgi rengini siyah yapar
		points[0] = xy[0]; points[1] = xy[1]; //başlangıç koordinatalrını ayarlar
		clearmouseclick(WM_RBUTTONDOWN); //sağ fare düğmesi tıklamalarını temziler
		clearmouseclick(WM_LBUTTONDOWN); //sol fare düğmesi tıklamalrını temizler
		int preX = 0, preY = 1;
		int posX = 2, posY = 3;
		do {
			while (!ismouseclick(WM_LBUTTONDOWN)) {} //sol fare düğmesi tıklanana kadar bekle
			getmouseclick(WM_LBUTTONDOWN, points[posX], points[posY]); //sağ fare düğmesi tıklanana kadar döngüyü devam ettirir.
			line(points[preX], points[preY], points[posX], points[posY]); //çokgeni tamamlamak için başlangıç noktasına döner.
			preX += 2; preY += 2; posX += 2; posY += 2; //çokgeni ekrnada gösterir.
		} while (!ismouseclick(WM_RBUTTONDOWN));
		line(points[0], points[1], points[preX], points[preY]);
	}
	void getShape() {
		drawpoly(coordinates / 2, points);
	}
};

Shape *objArr[50];

int main()
{
	initwindow(1080, 680, "Paint");
	int *xy, size = 0;
	Tools selection;
	Square sqObj;
	Circle cirObj;
	RectangleShape recObj;
	Triangle triObj;
	Hexagon hexObj;
	LineShape linObj;
	Shape *S;
	S = &sqObj;
	selection = squareShape;
	S->Interface();
	S->Panel();
/*	while (1) {
		int x, y;
		clearmouseclick(WM_LBUTTONDOWN);
		while (!ismouseclick(WM_LBUTTONDOWN)) {}
		getmouseclick(WM_LBUTTONDOWN, x, y);
		cout << x << ' ' << y << endl;
	}*/
	bool Quit = false;
	while (!Quit) {
		S->Panel();
		SelectionIndicator(selection);
		xy = clickCheck();
		if (xy[0] > 0 && xy[0] < 135 && xy[1] > 0 && xy[1] < getmaxy()) {
			selection = ToolSelection(xy);
		}
		else if (xy[0] > 915 && xy[0] < 1065 && xy[1] > 1 && xy[1] < 67) {
			selection = ToolSelection(xy);
		}
		else {
			if (selection == squareShape) {
				S = &sqObj;
				objArr[size] = &sqObj;
				S->Draw(xy);
			}
			else if (selection == circleShape) {
				S = &cirObj;
				objArr[size] = &cirObj;
				S->Draw(xy);
			}
			else if (selection == rectangleShape) {
				S = &recObj;
				objArr[size] = &recObj;
				S->Draw(xy);
			}
			else if (selection == triangleShape) {
				S = &triObj;
				objArr[size] = &triObj;
				S->Draw(xy);
			}
			else if (selection == polygonShape) {
				S = &hexObj;
				objArr[size] = &hexObj;
				S->Draw(xy);
			}
			else if (selection == lineShape) {
				S = &linObj;
				objArr[size] = &linObj;
				S->Draw(xy);
			}
			else if (selection == bucketTool) {
				S->Bucket(xy);
			}
			else if (selection == pencilTool) {
				S->Pencil();
			}
			else if (selection == textTool) {
				S->Text(xy);
			}
			else if (selection == eraserTool) {
				S->Eraser();
			}
			else if (selection == undoTool) {
				Undo(size);
			}
			else if (selection == redoTool) {
				Redo(size);
			}
		}
		if (selection == colorPallete) {
			S->selectColor();
		}
		else if (selection == saveTool) {
			Save();
		}
		else if (selection == loadTool) {
			Load();
		}
		else if (selection == deleteTool) {
			S->Interface();
			S->Panel();
		}
		else if (selection == quitTool) {
			Quit = true;
		}
		size++;
	}
}

Tools ToolSelection(int* xy) {			// fare tıklamalarına göre araç seçimini belirleyen işlev
	setcolor(BLACK);
	static Tools previousSelection; //daha önceki seçimi saklamk için kullanılan bir değişken
	if (xy[0] > 5 && xy[0] < 77 && xy[1] > 37 && xy[1] < 96) { //fare tıklamalarının koordinatalrına göre seçilen aracı belirler
		previousSelection = squareShape;
		return squareShape;
	}
	else if (xy[0] > 66 && xy[0] < 130 && xy[1] > 37 && xy[1] < 96) {
		previousSelection = circleShape;
		return circleShape; // diğer araçalr için benzer koşıullar eklenir
	}
	else if (xy[0] > 5 && xy[0] < 77 && xy[1] > 100 && xy[1] < 150) {
		previousSelection = rectangleShape;
		return rectangleShape;
	}
	else if (xy[0] > 66 && xy[0] < 130 && xy[1] > 100 && xy[1] < 150) {
		previousSelection = triangleShape;
		return triangleShape;
	}
	else if (xy[0] > 5 && xy[0] < 77 && xy[1] > 151 && xy[1] < 210) {
		previousSelection = polygonShape;
		return polygonShape;
	}
	else if (xy[0] > 66 && xy[0] < 130 && xy[1] > 151 && xy[1] < 210) {
		previousSelection = lineShape;
		return lineShape;
	}
	else if (xy[0] > 5 && xy[0] < 60 && xy[1] > 250 && xy[1] < 300) {
		previousSelection = pencilTool;
		return pencilTool;
	}
	else if (xy[0] > 65 && xy[0] < 125 && xy[1] > 255 && xy[1] < 300) {
		previousSelection = eraserTool;
		return eraserTool;
	}
	else if (xy[0] > 5 && xy[0] < 65 && xy[1] > 307 && xy[1] < 355) {
		previousSelection = bucketTool;
		return bucketTool;
	}
	else if (xy[0] > 65 && xy[0] < 125 && xy[1] > 309 && xy[1] < 355) {
		previousSelection = colorPallete;
		return colorPallete;
	}
	else if(xy[0] > 49 && xy[0] < 85 && xy[1] > 367 && xy[1] < 410){
		previousSelection = textTool;
		return textTool;
	}
	else if (xy[0] > 914 && xy[0] < 987 && xy[1] > 1 && xy[1] < 67) {
		previousSelection = undoTool;
		return undoTool;
	}
	else if (xy[0] > 989 && xy[0] < 1065 && xy[1] > 1 && xy[1] < 67) {
		previousSelection = redoTool;
		return redoTool;
	}
	else if (xy[0] > 7 && xy[0] < 65 && xy[1] > 416 && xy[1] < 467) {
		previousSelection = saveTool;
		return saveTool;
	}
	else if (xy[0] > 67 && xy[0] < 127 && xy[1] > 416 && xy[1] < 467) {
		previousSelection = loadTool;
		return loadTool;
	}
	else if (xy[0] > 15 && xy[0] < 121 && xy[1] > 499 && xy[1] < 540) {
		previousSelection = deleteTool;
		return deleteTool;
	}
	else if (xy[0] > 30 && xy[0] < 101 && xy[1] > 606 && xy[1] < 640) {
		previousSelection = quitTool;
		return quitTool;
	}
	else {
		return previousSelection; //hiçbir araç seçilmemişse önceki seçimi döndürür.
	}
}

int* clickCheck() {
	int *xy = new int[2];
	clearmouseclick(WM_LBUTTONDOWN);
	while (!ismouseclick(WM_LBUTTONDOWN)) {}
	getmouseclick(WM_LBUTTONDOWN, xy[0], xy[1]);
	return xy; //fare tıklamasının koordinatalrını döndür.
}

void Palletes() {
	readimagefile("pallete.jpg", 5, 360, 130, 630); //renk paletini ekranda gösterir.
	setcolor(BLACK);
	//renk seçeneklerini çemberlerle göster ve doldur.
	//her renk seçeneği için benzer çizimler eklenir.
	circle(29, 395, 18);
	setfillstyle(SOLID_FILL, BLACK);
	floodfill(29, 395, BLACK);

	circle(69, 395, 18);
	setfillstyle(SOLID_FILL, WHITE);
	floodfill(69, 395, BLACK);

	circle(109, 395, 18);
	setfillstyle(SOLID_FILL, BLUE);
	floodfill(109, 395, BLACK);

	circle(29, 445, 18);
	setfillstyle(SOLID_FILL, RED);
	floodfill(29, 445, BLACK);

	circle(69, 445, 18);
	setfillstyle(SOLID_FILL, MAGENTA);
	floodfill(69, 445, BLACK);

	circle(109, 445, 18);
	setfillstyle(SOLID_FILL, CYAN);
	floodfill(109, 445, BLACK);

	circle(29, 495, 18);
	setfillstyle(SOLID_FILL, BROWN);
	floodfill(29, 495, BLACK);

	circle(69, 495, 18);
	setfillstyle(SOLID_FILL, GREEN);
	floodfill(69, 495, BLACK);

	circle(109, 495, 18);
	setfillstyle(SOLID_FILL, YELLOW);
	floodfill(109, 495, BLACK);

	circle(29, 545, 18);
	setfillstyle(SOLID_FILL, DARKGRAY);
	floodfill(29, 545, BLACK);

	circle(69, 545, 18);
	setfillstyle(SOLID_FILL, LIGHTGREEN);
	floodfill(69, 545, BLACK);

	circle(109, 545, 18);
	setfillstyle(SOLID_FILL, LIGHTBLUE);
	floodfill(109, 545, BLACK);

	circle(29, 595, 18);
	setfillstyle(SOLID_FILL, LIGHTCYAN);
	floodfill(29, 595, BLACK);

	circle(69, 595, 18);
	setfillstyle(SOLID_FILL, LIGHTMAGENTA);
	floodfill(69, 595, BLACK);

	circle(109, 595, 18);
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	floodfill(109, 595, BLACK);
}

void SelectionIndicator(Tools selection) { //seçilen aracın görsel göstergesini çizmek için kulalnılır.
	
	if (selection == squareShape) {
		circle(35, 65, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(35, 65, BLACK);
	}
	else if (selection == circleShape) {
		circle(99, 65, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(99, 65, BLACK);
	}
	else if (selection == rectangleShape) {
		circle(38, 127, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(38, 127, BLACK);
	}
	else if (selection == triangleShape) {
		circle(99, 127, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(99, 127, BLACK);
	}
	else if (selection == polygonShape) {
		circle(38, 180, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(38, 180, BLACK);
	}
	else if (selection == lineShape) {
		circle(105, 175, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(105, 175, BLACK);
	}
	else if (selection == bucketTool) {
		circle(42, 330, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(42, 330, BLACK);
	}
	else if (selection == pencilTool) {
		circle(38, 280, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(38, 280, BLACK);
	}
	else if (selection == textTool) {
		circle(67, 390, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(67, 390, BLACK);
	}
	else if (selection == eraserTool) {
		circle(93, 280, 7);
		setfillstyle(SOLID_FILL, BLUE);
		floodfill(93, 280, BLACK);
	}
}

void Undo(int size) {
	objArr[0]->Interface();
	objArr[0]->Panel();
	for (int i = 0; i < size - undoCount; i++) {
		cout << i << endl;
		objArr[i]->getShape();
	}
	undoCount++;
}

void Redo(int size) {
	objArr[0]->Interface();
	objArr[0]->Panel();
	if (undoCount > 0) {
		for (int i = 0; i < size - 1 + undoCount; i++) {
			objArr[i]->getShape();
		}
		undoCount--;
	}
}

void Save() {
	writeimagefile("SaveFile.bmp", 136, 0, 1070, 670); //ekran görüntüsünü kaydeder.
}

void Load() {
	readimagefile("SaveFile.bmp", 136, 0, 1070, 670); //ekran  görüntüsünü yükler.
}