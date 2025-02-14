/******************************************************************
 Grafika komputerowa, œrodowisko MS Windows - program  przyk³adowy
 *****************************************************************/

#include <windows.h>
#include <gdiplus.h>
#include <cmath>;
using namespace Gdiplus;


//deklaracja funkcji obslugi okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//funkcja Main - dla Windows
 int WINAPI WinMain(HINSTANCE hInstance,
               HINSTANCE hPrevInstance,
               LPSTR     lpCmdLine,
               int       nCmdShow)
{
	MSG meldunek;		  //innymi slowy "komunikat"
	WNDCLASS nasza_klasa; //klasa g³ównego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";
	
	GdiplusStartupInput gdiplusParametry;// parametry GDI+; domyœlny konstruktor wype³nia strukturê odpowiednimi wartoœciami
	ULONG_PTR	gdiplusToken;			// tzw. token GDI+; wartoœæ uzyskiwana przy inicjowaniu i przekazywana do funkcji GdiplusShutdown
   
	// Inicjujemy GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusParametry, NULL);

	//Definiujemy klase g³ównego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	nasza_klasa.lpfnWndProc   = WndProc; //adres funkcji realizuj¹cej przetwarzanie meldunków 
 	nasza_klasa.cbClsExtra    = 0 ;
	nasza_klasa.cbWndExtra    = 0 ;
	nasza_klasa.hInstance     = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon         = 0;
	nasza_klasa.hCursor       = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName  = "Menu" ;
	nasza_klasa.lpszClassName = nazwa_klasy;

    //teraz rejestrujemy klasê okna g³ównego
    RegisterClass (&nasza_klasa);
	
	/*tworzymy okno g³ówne
	okno bêdzie mia³o zmienne rozmiary, listwê z tytu³em, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na ca³y ekran, po utworzeniu
	bêdzie widoczne na ekranie */
 	okno = CreateWindow(nazwa_klasy, "Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	
	/* wybór rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
   	ShowWindow (okno, nCmdShow) ;
    
	//odswiezamy zawartosc okna
	UpdateWindow (okno) ;

	// G£ÓWNA PÊTLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
     /* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
	 komunikatu WM_QUIT; dla wszystkich pozosta³ych komunikatów zwraca wartoœæ TRUE */
	{
		TranslateMessage(&meldunek); // wstêpna obróbka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu w³aœciwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}

	GdiplusShutdown(gdiplusToken);
	
	return (int)meldunek.wParam;
}

 int pozx = 275, pozy = 350;
 int obrot = 0;
/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunków kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc (HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;
    	
/* PONI¯SZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEGÓLNE MELDUNKI */
	switch (kod_meldunku) 
	{
	case WM_CREATE:  //meldunek wysy³any w momencie tworzenia okna
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Zapiszcz...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 200, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Informacja");
		SetMenu(okno, mGlowne);
		DrawMenuBar(okno);
		SetTimer(okno, 1, 50, NULL);

	case WM_COMMAND: //reakcje na wybór opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(okno, "Zapiszczeæ?", "Pisk", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(okno); //wysylamy meldunek WM_DESTROY
        		  break;
		case 200: MessageBox(okno, "Imiê i nazwisko:\nNumer indeksu: ", "Autor", MB_OK);
		}
		return 0;
	case WM_TIMER:
			obrot += 5;  
			InvalidateRect(okno, NULL, TRUE); 
			break;
	case WM_MOUSEMOVE: //reakcja na lewy przycisk myszki
		{
			pozx = LOWORD(lParam);
			pozy = HIWORD(lParam);

			InvalidateRect(okno, NULL, TRUE);
			return 0;
		}

	case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC kontekst;

			kontekst = BeginPaint(okno, &paint);
		
			// MIEJSCE NA KOD GDI

			HPEN pioro = CreatePen(PS_SOLID, 10, RGB(255,0,0));
			SelectObject(kontekst, pioro);
			
			DeleteObject(pioro);

			HPEN LinePen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0)); 
			SelectObject(kontekst, LinePen);

			HBRUSH FillBrush = CreateSolidBrush(RGB(0, 255, 0));
			SelectObject(kontekst, FillBrush);

			// Define the triangle's points
			POINT points[] = {
				{150, 150}, 
				{400, 150},  
				{400, 400}, 
				{150, 400}
			};
			Ellipse(kontekst, 150, 50, 400, 250);
			Polygon(kontekst, points, sizeof(points) / sizeof(points[0]));

			LinePen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			SelectObject(kontekst, LinePen);

			FillBrush = CreateSolidBrush(RGB(180, 90, 120));
			SelectObject(kontekst, FillBrush);

			Ellipse(kontekst, 200, 200, 350, 350);

			LinePen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
			SelectObject(kontekst, LinePen);

			
			float dlX = pozx - 275;
			float dlY = pozy - 275;
			float length = sqrt(dlX * dlX + dlY * dlY);
			if (length != 0) {
				dlX /= length; 
				dlY /= length;
			}
			float scale = 75 * 75 / sqrt(75 * 75 * dlX * dlX +75 * 75 * dlY * dlY);

			int arrowX = 275 + (dlX * scale);
			int arrowY = 275 + (dlY * scale);

			MoveToEx(kontekst, 275, 275, NULL);
			LineTo(kontekst, arrowX, arrowY);

			POINT p[] = {
				{arrowX - 10, arrowY - 10},
				{arrowX, arrowY},
				{arrowX + 10, arrowY - 10},
			};
			Polygon(kontekst, p, sizeof(p) / sizeof(p[0]));

			DeleteObject(LinePen);
			DeleteObject(FillBrush);

			// utworzenie obiektu umo¿liwiaj¹cego rysowanie przy u¿yciu GDI+
			// (od tego momentu nie mo¿na u¿ywaæ funkcji GDI
			Graphics grafika(kontekst);
			
			// MIEJSCE NA KOD GDI+
			grafika.TranslateTransform(275, 275);
			grafika.RotateTransform(obrot);
			Pen pi(Color(80, 80, 80), 5);
			grafika.DrawLine(&pi, 0, 0, 0, -75);  
			Point punkty[] = { {0, -75}, {-10, -65}, {10, -65} };
			grafika.DrawPolygon(&pi, punkty, 3);
			// utworzenie czcionki i wypisanie tekstu na ekranie
			FontFamily  fontFamily(L"Times New Roman");
			Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
			PointF      pointF(100.0f, 400.0f);
			SolidBrush  solidBrush(Color(255, 0, 0, 255));

			//grafika.DrawString(L"To jest tekst napisany za pomoc¹ GDI+.", -1, &font, pointF, &solidBrush);

			EndPaint(okno, &paint);

			return 0;
		}
  	
	case WM_DESTROY: //obowi¹zkowa obs³uga meldunku o zamkniêciu okna
		KillTimer(okno, 1);
		PostQuitMessage (0) ;
		return 0;
    
	default: //standardowa obs³uga pozosta³ych meldunków
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}
