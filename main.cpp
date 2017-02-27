#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"
#include "Functions.h"
#include "myLibs.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

int XSize = (int)((float)GetSystemMetrics(SM_CXSCREEN)*(float)0.9),
    YSize = (int)((float)GetSystemMetrics(SM_CYSCREEN)*(float)0.9),
    LenStep = LENSTEPMIN,
    LenSize = LenStep/3,
    showed = 0,
    choice = 0;
float FmultiLen = 1,
      gamma = 0,
      height = 100,
      ymax = 0,
      f = FmultiLen*LenStep;
bool changeLens = 0,
     firstLen = 0,
     showLine = 0,
     showLens = 0;

VOID WINAPI TBNotifications(HWND DialogHandle, WPARAM wParam, HWND hwndTrack, UINT iSelMin, UINT iSelMax);

sf::RenderWindow *globalwindow = NULL;

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
        choice = 1;
    if(IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
        choice = 2;
    if(IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
        choice = 3;

    switch(uMsg)
    {
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDCANCEL:
            SendMessage(hDlg, WM_CLOSE, 0, 0);
            return TRUE;
        case IDOK:
        {
            if (choice == 0)
            {
                MessageBox(NULL, TEXT("Proszę wybrać jeden z układów soczewek"), TEXT("  Informacja"), MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                sf::ContextSettings settings;
                settings.antialiasingLevel = 10;

                sf::RenderWindow window(sf::VideoMode(XSize, YSize), "Symulacja", sf::Style::Default, settings);
				globalwindow = &window;
                while(window.isOpen())
                {
                    sf::Event event;
					globalwindow = &window;
                    while(window.pollEvent(event))
                    {
                        if(event.type == sf::Event::Closed)
                        {
                            window.close();
                            globalwindow = NULL;

                        }
                    }
                    LenSize = LenStep/3;
                    f = FmultiLen*LenStep;
                    selectedOptions(choice, f, changeLens, firstLen);
                    Line FirstLine;
                    FirstLine.x1 = 0;
                    FirstLine.x2 = LenStep;
                    FirstLine.y1 = height-(LenStep*tan(gamma*PI/180));
                    FirstLine.y2 = height;
                    window.clear(sf::Color::White);
                    ymax = 0;

                    DrawLight(FirstLine, f, changeLens, ymax, showed, LenStep, XSize, YSize, window);
                    if(showLens)
                        DrawLens(ymax, changeLens, firstLen, LenStep, LenSize, XSize, YSize, window);
                    if(showLine)
                        OpticLine(XSize, YSize, window);

                    window.display();
                    showed = 1;
                }
                return TRUE;
            }
        }

        case IDC_CHECK1:
        {
            switch (HIWORD(wParam))
            {
            case BN_CLICKED:
                if (SendDlgItemMessage(hDlg, IDC_CHECK1, BM_GETCHECK, 0, 0))
                    showLine = 1;
                else
                    showLine = 0;
                break;
            }
        }
        break;

        case IDC_CHECK2:
        {
            switch (HIWORD(wParam))
            {
            case BN_CLICKED:
                if (SendDlgItemMessage(hDlg, IDC_CHECK2, BM_GETCHECK, 0, 0))
                    showLens = 1;
                else
                    showLens = 0;
                break;
            }
        }
        break;

        }
        break;
    case WM_HSCROLL:
    {
        TBNotifications(hDlg, wParam, (HWND) lParam, 0, 100);
        break;
    }
    case WM_CLOSE:
        if(MessageBox(hDlg, TEXT("Czy na pewno chcesz wyjść z programu?"), TEXT("  Wyjście z programu"), MB_ICONQUESTION | MB_YESNO) == IDYES)
        {
            DestroyWindow(hDlg);
		}
		if (globalwindow)
            {
                (*globalwindow).close();
                globalwindow = NULL;
            }
        return TRUE;

    case WM_DESTROY:
        PostQuitMessage(0);
        ExitProcess(0);
        return TRUE;
    }
    return FALSE;
}

VOID WINAPI TBNotifications(HWND DialogHandle, WPARAM wParam, HWND hwndTrack, UINT iSelMin, UINT iSelMax)
{
    DWORD dwPos;
    TCHAR ValueData[8];
    HWND LabelHandle1 = NULL;
    HWND LabelHandle2 = NULL;
    switch (LOWORD(wParam))
    {

    case TB_ENDTRACK:

        dwPos = SendMessage(hwndTrack, TBM_GETPOS, 0, 0);

        switch (GetWindowLong(hwndTrack, GWL_ID))
        {
        case IDC_SLIDER1:
            LenStep = (int)((float)dwPos/(float)100*(LENSTEPMAX-LENSTEPMIN))+LENSTEPMIN;
            LabelHandle1 = GetDlgItem(DialogHandle, IDC_STATIC1);
            break;
        case IDC_SLIDER2:
            FmultiLen = (int)((float)dwPos/(float)100*24)+1;
            LabelHandle2 = GetDlgItem(DialogHandle, IDC_STATIC2);
            break;
        }
        if (LabelHandle1)
        {
            _stprintf(ValueData, _T("%d"), (int)((float)dwPos/(float)100*(LENSTEPMAX-LENSTEPMIN))+LENSTEPMIN);
            SetWindowText(LabelHandle1, ValueData);
        }
        if (LabelHandle2)
        {
            _stprintf(ValueData, _T("%d"), (int)((float)dwPos/(float)100*49)+1);
            SetWindowText(LabelHandle2, ValueData);
        }

        if (dwPos > iSelMax)
            SendMessage(hwndTrack, TBM_SETPOS,
                        (WPARAM) TRUE,
                        (LPARAM) iSelMax);

        else if (dwPos < iSelMin)
            SendMessage(hwndTrack, TBM_SETPOS,
                        (WPARAM) TRUE,
                        (LPARAM) iSelMin);
        break;

    default:

        break;
    }
}


int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
{
    HWND hDlg;
    MSG msg;
    BOOL ret;

    InitCommonControls();
    hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
    ShowWindow(hDlg, nCmdShow);


    while((ret = GetMessage(&msg, 0, 0, 0)) != 0)
    {
        if(ret == -1)
            return -1;

        if(!IsDialogMessage(hDlg, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}