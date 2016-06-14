// suffix tree.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "suffix tree.h"
#include <tchar.h>
#include <windows.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
char SABLON[1024], SABLON2[1024],SABLON3[1024],SABLON4[1024],SABLON6[1024];
int sufix_test1=0, subsir_test1=0;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Cuvant_Nou(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SUFFIXTREE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SUFFIXTREE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUFFIXTREE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SUFFIXTREE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
bool LoadAndBlitBitmap(LPCSTR szFileName, HDC hWinDC)
{
	// Load the bitmap image file
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// Verify that the image was loaded
	if (hBitmap == NULL) {
		::MessageBox(NULL, __T("LoadImage Failed"), __T("Error"), MB_OK);
		return false;
	}

	// Create a device context that is compatible with the window
	HDC hLocalDC;
	hLocalDC = ::CreateCompatibleDC(hWinDC);
	// Verify that the device context was created
	if (hLocalDC == NULL) {
		::MessageBox(NULL, __T("CreateCompatibleDC Failed"), __T("Error"), MB_OK);
		return false;
	}

	// Get the bitmap's parameters and verify the get
	BITMAP qBitmap;
	int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qBitmap));
	if (!iReturn)
	{
		::MessageBox(NULL, __T("GetObject Failed"), __T("Error"), MB_OK);
		return false;
	}

	// Select the loaded bitmap into the device context
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
	if (hOldBmp == NULL) {
		::MessageBox(NULL, __T("SelectObject Failed"), __T("Error"), MB_OK);
		return false;
	}

	// Blit the dc which holds the bitmap onto the window's dc
	BOOL qRetBlit = ::BitBlt(hWinDC, 0, 0, qBitmap.bmWidth, qBitmap.bmHeight,
		hLocalDC, 0, 0, SRCCOPY);
	if (!qRetBlit) {
		::MessageBox(NULL, __T("Blit Failed"), __T("Error"), MB_OK);
		return false;
	}

	// Unitialize and deallocate resources
	::SelectObject(hLocalDC, hOldBmp);
	::DeleteDC(hLocalDC);
	::DeleteObject(hBitmap);
	return true;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_box:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_TEXTBOX), hWnd, Cuvant_Nou);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		LoadAndBlitBitmap(__T("suffix_tree.bmp"), hdc);
		EndPaint(hWnd, &ps);
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



char* suffix(char *cuv,int m,int in)
{
	char buff[33];
	int i,n,j;
	n=strlen(cuv);
	j=0;
	
	for(i=in;i<in+m;i++)
	{
		buff[j]=cuv[i];
		j++;
	}
	buff[j]='\0';
	return buff;
	
}



void program(char * cuv, char * cuv2,HWND hwnd)
{
	if (strlen(cuv) == 0 && strlen(cuv2)==0)
	{
		MessageBox(hwnd, "Introduceti primul cuvant si al doilea cuvant" , NULL, NULL);
	}
	else if (strlen(cuv) == 0 && strlen(cuv2) != 0)
	{
		MessageBox(hwnd, "Introduceti primul cuvant " , NULL, NULL);
	}
	else if (strlen(cuv2) == 0 && strlen(cuv) != 0)
	{
		MessageBox(hwnd, "Introduceti al doilea cuvant ", NULL, NULL);
	}
	char suf1[33],suf2[33];
	//scanf("%s",cuv);
	int n,i,index,start,j,m,ok;
	n=strlen(cuv);
	index=n-1;
	//pt cuv1
	printf("cuv1:");
	ok=0;
	for(i=n-1;i>=0;i--)
	{
		for(start=0;start<n-i;start++)
		{
			strcpy(suf1,suffix(cuv,i,start));

			for(j=start+1;j<=n-i;j++)
			{
				strcpy(suf2,suffix(cuv,i,j));
				if(strcmp(suf1,suf2)==0)
				{
					ok=1;
					SetDlgItemText(hwnd,IDC_EDIT5,suf1);
					//printf("%s\t%s\n",suf1,suf2);
					break;
				}
			}
			if(ok==1)
				break;
		}
		if(ok==1)
				break;
	}
	//asta de sus e buna;
	//pt cuv2
	printf("cuv2:");
	m=strlen(cuv2);
	ok=0;
	for(i=m-1;i>=0;i--)
	{
		for(start=0;start<m-i;start++)
		{
			strcpy(suf1,suffix(cuv2,i,start));

			for(j=start+1;j<=m-i;j++)
			{
				strcpy(suf2,suffix(cuv2,i,j));
				if(strcmp(suf1,suf2)==0)
				{
					//printf("%s\t%s\n",suf1,suf2);
					SetDlgItemText(hwnd,IDC_EDIT2,suf1);
					ok=1;
					break;
				}
			}
			if(ok==1)
				break;
		}
		if(ok==1)
				break;
	}
	//asta de sus e buna;
	printf("comun:");
	ok=0;
	int min;
	if(m<n)
		min=m;
	else
		min=n;
	index=min-1;
	for(i=min-1;i>=0;i--)
	{
		for(start=0;start<n-i;start++)
		{
			strcpy(suf1,suffix(cuv,i,start));
			for(j=0;j<=m-i;j++)
			{
				strcpy(suf2,suffix(cuv2,i,j));
				if(strcmp(suf1,suf2)==0)
				{
					ok=1;
					SetDlgItemText(hwnd,IDC_EDIT3,suf1);

					break;		
				}
			}
			if(ok==1)
				break;
		}
		if(ok==1)
				break;
	}
}

typedef struct lista{
	char cuvant[33];
	int frecventa;
	lista*next;
};

lista *primul, *first;

typedef struct nod{
	char *cuvant;
	nod**next;
	int fii;
};

nod *root, *root2;

int tree(char *cuv, nod *p)
{
	int m,j,n,i,k,rez;
	char buf1[2],*buf2;
	nod *q,*b;
	if(p==0)
		return 0;
	q=(nod*)malloc(sizeof(nod));
	b=(nod*)malloc(sizeof(nod));
	m=strlen(p->cuvant);
	n=strlen(cuv);
	i=0;
	for(j=0;j<m;j++)
	{
		if(p->cuvant[j]!=cuv[i])
			break;
		i++;
	}
	if(i==0)
		return 0;
	if(i==m)//reformulez cuvantul si mai fac inca o legatura pt nodul asta()
	{// in cazul in care am parcurs tot cuvantul din nodul asta
		int boom;
		
		
		if(p->fii==0)
		{
			
			p->cuvant[m]='\0';
			


			p->fii++;
			p->next=(nod**)malloc(sizeof(nod*)*(p->fii+1));
			buf1[0]='$';
			buf1[1]='\0';
			q->cuvant=(char*)malloc(sizeof(char)*2);
			strcpy(q->cuvant,buf1);
			q->fii=0;
			q->next=0;
			boom=p->fii;
			p->fii++;
			k=0;
			buf2=(char*)malloc(sizeof(char)*(n-m));
			for(j=i;j<n;j++)
			{
				buf2[k]=cuv[j];
				k++;
			}
			buf2[k]='\0';
			b->cuvant=(char*)malloc(sizeof(char)*(strlen(buf2)));
			b->fii=0;
			b->next=0;
		}
		else
		{
			boom=p->fii;
			int var;
			var=i;
			k=0;
			buf2=(char*)malloc(sizeof(char)*(n-m));
			for(j=i;j<n;j++)
			{
				buf2[k]=cuv[j];
				k++;
			}
			buf2[k]='\0';
			for(i=0;i<boom;i++)
			{
				rez=tree(buf2,p->next[i]);
				if(rez==1 || rez==2)
					return 1;
			}
			q->cuvant=(char*)malloc(sizeof(char)*(strlen(buf2)));
			strcpy(q->cuvant,buf2);
			q->fii=0;
			q->next=0;
			p->next=(nod**)realloc(p->next,sizeof(nod*)*(p->fii+1));
			p->next[p->fii]=q;
			p->fii++;
		}
		return 1;
	}
	if(i>0 && i<m)
	{//aici sa mai verifici inca o data
		
		int ind;
		ind=i;
		buf2=(char*)malloc(sizeof(char)*(m-ind));
		j=0;
		for(i=ind;i<m;i++)
		{
			buf2[j]=p->cuvant[i];
			j++;
		}
		buf2[j]='\0';
		p->cuvant[ind]='\0';
		
		q->cuvant=(char*)malloc(sizeof(char)*(strlen(buf2)));
		strcpy(q->cuvant,buf2);
		q->fii=p->fii;
		if(q->fii!=0)
			q->next=(nod**)realloc(q->next,sizeof(nod*)*(q->fii+1));//asta e cheia
		
		for(i=0;i<p->fii;i++)
			q->next[i]=p->next[i];
	//	printf("face asta: %s",p->cuvant);
		if(p->fii==0)
			p->next=(nod**)malloc(sizeof(nod*)*2);
		else
			p->next=(nod**)realloc(p->next,sizeof(nod*)*2);
	//	printf("gata");
		p->next[0]=q;//aici da acces violation
		//printf("%d",p->fii);
		p->fii++;
		p->next[1]=b;
		p->fii++;
		j=0;
		for(i=ind;i<n;i++)
		{
			buf2[j]=cuv[i];
			j++;
		}
		buf2[j]='\0';
		b->cuvant=(char*)malloc(sizeof(char)*(strlen(buf2)));
		strcpy(b->cuvant,buf2);
		b->fii=0;
		b->next=0;
		return 2;
	}
}


void radacina(char *cuv)
{
	int i,n,rez;
	n=root->fii;
	nod *p;
	rez=0;
	for(i=0;i<n;i++)
	{
		if(root->next[i]!=0)
		{
			rez=tree(cuv,root->next[i]);
			if(rez==1 || rez==2)
				break;
		}
	}
	
	if(rez==0 || root->fii==0)
	{
		p=(nod*)malloc(sizeof(nod));
		p->cuvant=(char*)malloc(sizeof(char)*(strlen(cuv)));
		strcpy(p->cuvant,cuv);
		p->fii=0;
		p->next=0;
		root->next=(nod**)realloc(root->next,sizeof(nod*)*(root->fii+1));
		root->next[n]=p;
		root->fii++;
		
	}
}

void radacina2(char *cuv)
{
	int i,n,rez;
	n=root2->fii;
	nod *p;
	rez=0;
	for(i=0;i<n;i++)
	{
		if(root2->next[i]!=0)
		{
			rez=tree(cuv,root2->next[i]);
			if(rez==1 || rez==2)
				break;
		}
	}
	
	if(rez==0 || root2->fii==0)
	{
		p=(nod*)malloc(sizeof(nod));
		p->cuvant=(char*)malloc(sizeof(char)*(strlen(cuv)));
		strcpy(p->cuvant,cuv);
		p->fii=0;
		p->next=0;
		root2->next=(nod**)realloc(root2->next,sizeof(nod*)*(root2->fii+1));
		root2->next[n]=p;
		root2->fii++;	
	}
}

void suffix2(char *cuv,int m)
{
	char buff[33];
	int i,n,j;
	n=strlen(cuv);
	j=0;
	for(i=m;i<n;i++)
	{
		buff[j]=cuv[i];
		j++;
	}
	buff[j]='\0';
	printf("%s\n",buff);
	radacina2(buff);
}




void suffix(char *cuv,int m)
{
	char buff[33];
	int i,n,j;
	n=strlen(cuv);
	j=0;
	for(i=m;i<n;i++)
	{
		buff[j]=cuv[i];
		j++;
	}
	buff[j]='\0';
	printf("%s\n",buff);
	radacina(buff);
}



void crearelista(nod*p,lista*q)
{
	if(p==0)
		return;
	lista*aux,*nou;
	if(q==0)
	{
		nou=(lista*)malloc(sizeof(lista));
		strcpy(nou->cuvant,p->cuvant);
		nou->frecventa=1;
		nou->next=0;
		primul=nou;
		return;

	}
	aux=q;
	while(aux!=0)
	{
		
		if(strcmp(p->cuvant,aux->cuvant)==0)
		{
			aux->frecventa++;
			return;
		}
		
		if(aux->next==0)
			break;
		aux=aux->next;
	}
	nou=(lista*)malloc(sizeof(lista));
	strcpy(nou->cuvant,p->cuvant);
	nou->frecventa=1;
	nou->next=0;
	aux->next=nou;
	return;

}

void afisare(nod*p)
{
	if(p!=0)
	{
		//printf("\t%s-%d\n",p->cuvant,p->fii);
		crearelista(p,primul);
		int n, i;
		n=p->fii;
		for(i=0;i<n;i++)
			afisare(p->next[i]);
	}
	return;
}


void dubiu(nod*p)
{
	if(p!=0)
	{
		printf("\t%s\n",p->cuvant);
		//crearelista(p,primul);
		int n, i;
		n=p->fii;
		for(i=0;i<n;i++)
		{
			//printf("-%d",i);
			dubiu(p->next[i]);}
	}
	return;
}

void afisarelista(lista*p)
{
	lista*aux;
	aux=p;
	while(aux!=0)
	{
		printf("%s %d\n",aux->cuvant,aux->frecventa);
		aux=aux->next;
	}
}


void celmailungsubsirrepetitiv(lista*p)
{
	lista*aux,*aux2;
	int i,max,lung=0,v[33],n,j,m;
	aux=p;
	aux2=p;
	i=0;
	while(aux!=0)
	{
		v[i]=strlen(aux->cuvant);
		i++;
		aux=aux->next;
	}
	n=i;
	for(int k=0;k<n;k++)
	{
		aux=p;
		max=0;
		for(i=0;i<n;i++)
			if(max<v[i])
			{
				max=v[i];
				m=i;
			}

		for(j=0;j<m;j++)
				aux=aux->next;
		if(aux->frecventa<=1)
			v[m]=0;
		else
			break;
	
	}
	printf("cel mai lung sir repetitiv: %s %d",aux->cuvant,aux->frecventa);
}


void subsir(nod*p,char *cuv,HWND hwnd)
{
	int n,m,i,j,k;
	char buf[33];
	n=strlen(cuv);
	if(p==0)
	{
		//printf("nu e subsir");
		return;
	}
	int ok=0;
	m=strlen(p->cuvant);
	for(i=0;i<m;i++)
	{
		if(cuv[i]!=p->cuvant[i])
		{
			ok=1;
			break;}
		if(i==n-1)
			break;
	}
	if(i==0)
	{
	
		return;
	}
	if(i==n-1)
	{
		subsir_test1=1;
		SetDlgItemText(hwnd,IDC_EDIT8,"Da, este un subsir!");
	}
	if(i==m)
	{
		k=0;
		for(j=m;j<=n-m;j++)
		{
			buf[k]=cuv[j];
			k++;
		}
		buf[k]='\0';
		for(i=0;i<p->fii;i++)
			subsir(p->next[i],buf,hwnd);
	}
}

void verifsufix(nod*p,char *cuv,HWND hwnd)
{
	int n,m,i,j,k;
	char buf[33];
	n=strlen(cuv);
	if(p==0)
	{
		//printf("nu e subsir");
		return;
	}
	int ok=1;
	m=strlen(p->cuvant);
	for(i=0;i<m;i++)
	{
		if(cuv[i]!=p->cuvant[i])
		{
			ok=0;
			break;}
		//if(i==n-1)
			//break;
	}
	if(i==0)
	{
		
		return;
	}
	if(strcmp(cuv,p->cuvant)==0)
	{
		sufix_test1=1;
		SetDlgItemText(hwnd,IDC_EDIT11,"Da, este un suffix");
	}
		//printf("intradevar e suffix");
	
	if(i==m)
	{
		k=0;
		for(j=m;j<=n-m;j++)
		{
			buf[k]=cuv[j];
			k++;
		}
		buf[k]='\0';
		for(i=0;i<p->fii;i++)
			verifsufix(p->next[i],buf,hwnd);
	}
}


void program_arbore(char* cuvant, char*subs1, char * sufi, char*cuv_suf,HWND hwnd )
{
	if (strlen(cuvant) == 0)
	{
		MessageBox(hwnd, "Introduceti cuvantul ", NULL, NULL);
	}
	if (strlen(subs1) == 0)
	{
		MessageBox(hwnd, "Introduceti subsirul ", NULL, NULL);
	}
	if (strlen(sufi) == 0)
	{
		MessageBox(hwnd, "Introduceti cuvantul\ ce trebuie verificat\ ca si sufix ", NULL, NULL);
	}
	char cuv[33];
	int i,n;
	nod*p;
	p=(nod*)malloc(sizeof(nod));
	//printf("introduceti cuv:");
	//scanf("%s",cuv);
	strcpy(cuv,cuvant);
	n=strlen(cuv);
	cuv[n]='$';
	cuv[n+1]='\0';
	root=p;
	p->fii=0;
	p->next=0;
	for(i=n-1;i>=0;i--)
		suffix(cuv,i);
	//printf("\n\n");
	n=root->fii;
	primul=0;
	//for(i=0;i<n;i++)
	//{
		//printf("%d-");
	//dubiu(root->next[i]);
	//}
	//	afisare(root->next[i]);
	//afisarelista(primul);
	//celmailungsubsirrepetitiv(primul);

	char subs[33];
	strcpy(subs,subs1);

	//printf("subsir:");
	//scanf("%s",subs);
	subsir_test1=0;
	for(i=0;i<n;i++)
	{
		subsir(root->next[i],subs,hwnd);
	}
	if(subsir_test1==0)
		SetDlgItemText(hwnd,IDC_EDIT8,"NU, nu este un subsir!");
}

void doar_suffix(HWND hwnd,char*cuvant, char*suffi)
{
	char cuv[33];
	char sufli[33];
	int i,n,m;
	nod*p;
	p=(nod*)malloc(sizeof(nod));
	printf("introduceti cuv:");
	//scanf("%s",cuv);
	strcpy(cuv,cuvant);
	n=strlen(cuv);
	cuv[n]='$';
	cuv[n+1]='\0';
	root2=p;
	p->fii=0;
	
	p->next=0;
	for(i=n-1;i>=0;i--)
		suffix2(cuv,i);
	printf("\n\n");
	n=root2->fii;
	primul=0;
	strcpy(sufli,suffi);
	//printf("suffix:");
	//scanf("%s",sufi);
	m=strlen(sufli);
	sufli[m]='$';
	sufli[m+1]='\0';
	sufix_test1=0;
	for(i=0;i<n;i++)
	{
		verifsufix(root2->next[i],sufli,hwnd);
	}
	if(sufix_test1==0)
		SetDlgItemText(hwnd,IDC_EDIT11,"NU, nu este un suffix");
}


BOOL CALLBACK  Cuvant_Nou(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		return TRUE;
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				
			case IDC_BUTTON2:
			{
				int lungime=GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT1));
				if(lungime > 0)
				{
							// Now we allocate, and get the string into our buffer
					GetDlgItemText(hwnd,IDC_EDIT1,SABLON,lungime+1);
					//SetDlgItemText(hwnd,IDC_EDIT5,SABLON);
				}
					
			
				int lungime2=GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT4));
				if(lungime2 > 0)
				{
							// Now we allocate, and get the string into our buffer
					GetDlgItemText(hwnd,IDC_EDIT4,SABLON2,lungime2+1);
					//SetDlgItemText(hwnd,IDC_EDIT2,SABLON2);
					
				}
				
				{
							// Now we allocate, and get the string into our buffer
					GetDlgItemText(hwnd,IDC_EDIT6,SABLON3,33);
					GetDlgItemText(hwnd,IDC_EDIT7,SABLON4,33);
					GetDlgItemText(hwnd,IDC_EDIT9,SABLON6,33);
					//GetDlgItemText(hwnd,IDC_EDIT7,SABLON4,33);
					//SetDlgItemText(hwnd,IDC_EDIT5,SABLON);
				}

				program(SABLON,SABLON2,hwnd);
				program_arbore(SABLON4,SABLON3,SABLON6,SABLON4,hwnd);
				doar_suffix(hwnd,SABLON4,SABLON6);

			}
			break;
			case IDM_EXIT:
				EndDialog(hwnd, WM_DESTROY);
				return 1;
			break;
			case WM_DESTROY:
				EndDialog(hwnd, WM_DESTROY);
				return 1;
			break;
			}
		break;
		default:
			return FALSE;
			break;
	}
	return TRUE;
};


