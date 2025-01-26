#include <windows.h>
#include "Crypto.cpp"

#pragma Secret

char *dataName;
char *textName;
bool isSaved;

char *new_URL() {
  char *temp = (char*) malloc(MAX_PATH);
  temp[0] = '\0';
  return temp;
}

char *changeExt(char *fileName, char *newExt) {
  char *temp = new_URL();
  int i = 0;
  while (true) {
    if (fileName[i] == '.') {
      break;
    } else {
      temp[i] = fileName[i];
    }
    i++;
  }
  
  temp[i] = fileName[i];
  temp[i+1] = newExt[0];
  temp[i+2] = newExt[1];
  temp[i+3] = newExt[2];
  temp[i+4] = '\0';
  
  return temp;
}

bool saveFile(char* fileName, char* text, int length) {
  HANDLE file = CreateFile(fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  DWORD write;
  bool success = WriteFile(file, text, length, &write, NULL);
  CloseHandle(file);
  return success;
}

void reset(HWND frame) {
  textName = new_URL();
  dataName = new_URL();
  isSaved = false;
  
  SetWindowText(GetDlgItem(frame, -1), "");
  SetWindowText(GetDlgItem(frame, -2), "");
}
bool open(HWND frame, int component, char* filter, char* defExt) {
  bool success = false;
  try {
    char *fileName = new_URL();
    
    OPENFILENAME fileChooser;
    ZeroMemory(&fileChooser, sizeof(OPENFILENAME));
    
    fileChooser.lStructSize = sizeof(OPENFILENAME);
    fileChooser.hwndOwner = frame;
    fileChooser.lpstrFilter = filter;
    fileChooser.lpstrFile = fileName;
    fileChooser.nMaxFile = MAX_PATH;
    fileChooser.lpstrDefExt = defExt;
    fileChooser.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    
    if (GetOpenFileName(&fileChooser)) {
      textName = fileName;
      dataName = changeExt(textName, "dat");
      
      HANDLE file = CreateFile(textName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
      char* text = (char*) malloc(GetFileSize(file, NULL));
      DWORD read;
      if (ReadFile(file, text, GetFileSize(file, NULL), &read, NULL)) {
        text[GetFileSize(file, NULL)] = '\0';
        SetWindowText(GetDlgItem(frame, component), text);
        success = true;
      }
      CloseHandle(file);
    }
  } catch (int e) {
    MessageBox(frame, "Error al abrir el archivo", "Secret", MB_OK | MB_ICONERROR);
  }
  return success;
}

bool save(HWND frame, int component, bool forceAsk) {
  bool success = false;
  try {
    if (!isSaved || forceAsk) {
      char *fileName = new_URL();
      
      OPENFILENAME fileChooser;
      ZeroMemory(&fileChooser, sizeof(OPENFILENAME));
      
      fileChooser.lStructSize = sizeof(OPENFILENAME);
      fileChooser.hwndOwner = frame;
      fileChooser.lpstrFilter = "Text Files (*.txt)\0*.txt\0";
      fileChooser.lpstrFile = fileName;
      fileChooser.nMaxFile = MAX_PATH;
      fileChooser.lpstrDefExt = "txt";
      fileChooser.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
      
      if (GetSaveFileName(&fileChooser)) {
        textName = fileName;
        dataName = changeExt(textName, "dat");
      }
    }
    char *text = (char*) malloc(GetWindowTextLength(GetDlgItem(frame, component)) + 1);
    GetWindowText(GetDlgItem(frame, component), text, GetWindowTextLength(GetDlgItem(frame, component)) + 1);
    success = saveFile(textName, text, GetWindowTextLength(GetDlgItem(frame, component)) + 1);
  } catch (int e) {
    MessageBox(frame, "Error al guardar el archivo", "Secret", MB_OK | MB_ICONERROR);
  }
  return success;
}

void getSecret(HWND frame, int from, int to, char* filter, char* defExt) {
  if (open(frame, from, filter, defExt)) {
    isSaved = false;
    
    LPSTR text;
    text = (LPSTR) GlobalAlloc(GPTR, GetWindowTextLength(GetDlgItem(frame, from)) + 1);
    GetWindowText(GetDlgItem(frame, from), text, GetWindowTextLength(GetDlgItem(frame, from)) + 1);
    SetWindowText(GetDlgItem(frame, to), text);
  }  
}

void setSecret(HWND frame, int from, int to) {
  LPSTR text;
  text = (LPSTR) GlobalAlloc(GPTR, GetWindowTextLength(GetDlgItem(frame, from)) + 1);
  GetWindowText(GetDlgItem(frame, from), text, GetWindowTextLength(GetDlgItem(frame, from)) + 1);
  SetWindowText(GetDlgItem(frame, to), crypto(text));
  if (isSaved) {
    GetWindowText(GetDlgItem(frame, to), text, GetWindowTextLength(GetDlgItem(frame, to)) + 1);
    try {
      saveFile(dataName, text, GetWindowTextLength(GetDlgItem(frame, to)) + 1);
    } catch (int e) {
      MessageBox(frame, "Error al guardar el archivo", "Secret", MB_OK | MB_ICONERROR);
    }
  }
}

HMENU getMenu (){
  HMENU menuBar = CreateMenu();
  HMENU mFile = CreateMenu();
  HMENU mEdit = CreateMenu();
  HMENU mHelp = CreateMenu();
  
  AppendMenu(mFile, MF_STRING, 00, "&Nuevo");
  AppendMenu(mFile, MF_STRING, 01, "&Abrir");
  //AppendMenu(mFile, MF_STRING, 02, "&Desencriptar");
  AppendMenu(mFile, MF_STRING, 03, "&Guardar");
  AppendMenu(mFile, MF_STRING, 04, "G&uardar como...");
  AppendMenu(mFile, MF_SEPARATOR, false, NULL);
  AppendMenu(mFile, MF_STRING, 05, "&Salir");
  
  AppendMenu(mEdit, MF_STRING, 10, "&Copiar");
  AppendMenu(mEdit, MF_STRING, 11, "C&ortar");
  AppendMenu(mEdit, MF_STRING, 12, "&Pegar");
  
  AppendMenu(mHelp, MF_STRING, 20, "&Ayuda en Secret");
  AppendMenu(mHelp, MF_SEPARATOR, false, NULL);
  AppendMenu(mHelp, MF_STRING, 21, "A&cerca de Secret");
  
  AppendMenu(menuBar, MF_STRING | MF_POPUP, (UINT) mFile, "&Archivo");
  AppendMenu(menuBar, MF_STRING | MF_POPUP, (UINT) mEdit, "&Edición");
  AppendMenu(menuBar, MF_STRING | MF_POPUP, (UINT) mHelp, "A&yuda");
  
  return menuBar;
}

LRESULT CALLBACK msgHandler(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
    case WM_CREATE:
      CreateWindow(
        "EDIT", "",
        WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN,
        0, 0, 542, 225,
        window, (HMENU) -1, ((LPCREATESTRUCT) lParam)->hInstance, NULL
      );
      SendDlgItemMessage(window, -1, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(true, 0));
      CreateWindow(
        "EDIT", "",
        WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN | ES_READONLY,
        0, 225, 542, 75,
        window, (HMENU) -2, ((LPCREATESTRUCT) lParam)->hInstance, NULL
      );
      SendDlgItemMessage(window, -2, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(true, 0));
      CreateWindow(
        "BUTTON", "Encriptar",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        452, 310, 80, 20,
        window, (HMENU) 30, ((LPCREATESTRUCT) lParam)->hInstance, NULL
      );
      SendDlgItemMessage(window, 30, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(true, 0));
      reset(window);
      break;
    case WM_COMMAND:
      switch (wParam) {
        case 00:
          reset(window);
          break;
        case 01:
          isSaved = open(window, -1, "Archivos de texto (*.txt)\0*.txt\0", "txt");
          break;
        case 02:
          getSecret(window, -2, -1, "Archivos encriptados (*.dat)\0*.dat\0", "dat");
          break;
        case 03:
          isSaved = save(window, -1, false);
          break;
        case 04:
          isSaved = save(window, -1, true);
          break;
        case 05:
          exit(0);
          break;
        case 10:
          SendDlgItemMessage(window, -1, WM_COPY, 0, 0);
          break;
        case 11:
          SendDlgItemMessage(window, -1, WM_CUT, 0, 0);
          break;
        case 12:
          SendDlgItemMessage(window, -1, WM_PASTE, 0, 0);
          break;
        case 20:
          ShellExecute(window, "open", "Help\\Help.htm", "", "", SW_SHOW);
          break;
        case 21:
          MessageBox(window, "Desarrollado por Wil C", "Acerca de Secret", MB_OK | MB_ICONINFORMATION);
          break;
        case 30:
          setSecret(window, -1, -2);
          break;
      }
      break;  
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(window, msg, wParam, lParam);
  }
  return 0;
}

int WINAPI WinMain(HINSTANCE thisInstance, HINSTANCE prevInstance, LPSTR arg, int funsterStil) {
  HWND window;
  MSG msg;
  WNDCLASSEX winClass;

  winClass.hInstance = thisInstance;
  winClass.lpszClassName = "Secret";
  winClass.lpfnWndProc = msgHandler;
  winClass.style = CS_DBLCLKS;
  winClass.cbSize = sizeof(WNDCLASSEX);
  winClass.hIcon = LoadIcon(thisInstance, "icon");
  winClass.hIconSm = LoadIcon(thisInstance, "icon");
  winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  winClass.lpszMenuName = NULL;
  winClass.cbClsExtra = 0;
  winClass.cbWndExtra = 0;
  winClass.hbrBackground = (HBRUSH) COLOR_WINDOW;

  if (!RegisterClassEx(&winClass)) {
    return 0;
  }

  window = CreateWindowEx(0, "Secret", "Secret", 0xca0000, CW_USEDEFAULT, CW_USEDEFAULT, 550, 400, HWND_DESKTOP, getMenu(), thisInstance, NULL);
  ShowWindow(window, funsterStil);

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

