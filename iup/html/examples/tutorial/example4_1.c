#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iup.h>
#include <iup_config.h>
#include <im.h>
#include <im_image.h>
#include <im_convert.h>
#include <iupim.h>


/********************************** Utilities *****************************************/


const char* str_filetitle(const char *file_name)
{
  /* Start at the last character */
  int len = (int)strlen(file_name);
  int offset = len - 1;
  while (offset != 0)
  {
    if (file_name[offset] == '\\' || file_name[offset] == '/')
    {
      offset++;
      break;
    }
    offset--;
  }
  return file_name + offset;
}

static void show_file_error(int error)
{
  switch (error)
  {
  case IM_ERR_OPEN:
    IupMessage("File Error", "Error Opening File.");
    break;
  case IM_ERR_MEM:
    IupMessage("File Error", "Insuficient memory.");
    break;
  case IM_ERR_ACCESS:
    IupMessage("File Error", "Error Accessing File.");
    break;
  case IM_ERR_DATA:
    IupMessage("File Error", "Image type not Suported.");
    break;
  case IM_ERR_FORMAT:
    IupMessage("File Error", "Invalid Format.");
    break;
  case IM_ERR_COMPRESS:
    IupMessage("File Error", "Invalid or unsupported compression.");
    break;
  default:
    IupMessage("File Error", "Unknown Error.");
  }
}

imImage* read_file(const char* filename)
{
  int error;
  imImage* image = imFileImageLoadBitmap(filename, 0, &error);
  if (error) 
    show_file_error(error);
  else
  {
    if (image->color_space != IM_RGB)
    {
      imImage* new_image = imImageCreateBased(image, -1, -1, IM_RGB, -1);
      imConvertColorSpace(image, new_image);
      imImageDestroy(image);

      image = new_image;
    }
  }
  return image;
}

int write_file(const char* filename, const imImage* image)
{
  const char* format = imImageGetAttribString(image, "FileFormat");
  int error = imFileImageSave(filename, format, image);
  if (error)
  {
    show_file_error(error);
    return 0;
  }
  return 1;
}

void new_file(Ihandle* ih, imImage* image)
{
  Ihandle* dlg = IupGetDialog(ih);
  Ihandle* canvas = IupGetDialogChild(dlg, "CANVAS");
  imImage* old_image = (imImage*)IupGetAttribute(canvas, "IMAGE");

  IupSetAttribute(dlg, "TITLE", "Untitled - Simple Paint");
  IupSetAttribute(canvas, "FILENAME", NULL);
  IupSetAttribute(canvas, "DIRTY", "NO");

  IupSetAttribute(canvas, "IMAGE", (char*)image);

  IupUpdate(canvas);

  if (old_image)
    imImageDestroy(old_image);
}

void check_new_file(Ihandle* dlg)
{
  Ihandle* canvas = IupGetDialogChild(dlg, "CANVAS");
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
  if (!image)
  {
    Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
    int width = IupConfigGetVariableIntDef(config, "NewImage", "Width", 640);
    int height = IupConfigGetVariableIntDef(config, "NewImage", "Height", 480);

    image = imImageCreate(width, height, IM_RGB, IM_BYTE);

    new_file(dlg, image);
  }
}

void open_file(Ihandle* ih, const char* filename)
{
  imImage* image = read_file(filename);
  if (image)
  {
    Ihandle* dlg = IupGetDialog(ih);
    Ihandle* canvas = IupGetDialogChild(dlg, "CANVAS");
    Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
    imImage* old_image = (imImage*)IupGetAttribute(canvas, "IMAGE");

    IupSetfAttribute(dlg, "TITLE", "%s - Simple Paint", str_filetitle(filename));
    IupSetStrAttribute(canvas, "FILENAME", filename);
    IupSetAttribute(canvas, "DIRTY", "NO");
    IupSetAttribute(canvas, "IMAGE", (char*)image);

    IupUpdate(canvas);

    if (old_image)
      imImageDestroy(old_image);

    IupConfigRecentUpdate(config, filename);
  }
}

void save_file(Ihandle* canvas)
{
  char* filename = IupGetAttribute(canvas, "FILENAME");
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
  if (write_file(filename, image))
    IupSetAttribute(canvas, "DIRTY", "NO");
}

void saveas_file(Ihandle* canvas, const char* filename)
{
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
  if (write_file(filename, image))
  {
    Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
  
    IupSetfAttribute(IupGetDialog(canvas), "TITLE", "%s - Simple Paint", str_filetitle(filename));
    IupSetStrAttribute(canvas, "FILENAME", filename);
    IupSetAttribute(canvas, "DIRTY", "NO");

    IupConfigRecentUpdate(config, filename);
  }
}

int save_check(Ihandle* ih)
{
  Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
  if (IupGetInt(canvas, "DIRTY"))
  {
    switch (IupAlarm("Warning", "File not saved! Save it now?", "Yes", "No", "Cancel"))
    {
    case 1:  /* save the changes and continue */
      save_file(canvas);
      break;
    case 2:  /* ignore the changes and continue */
      break;
    case 3:  /* cancel */
      return 0;  
    }
  }
  return 1;
}

void toggle_bar_visibility(Ihandle* item, Ihandle* ih)
{
  if (IupGetInt(item, "VALUE"))
  {
    IupSetAttribute(ih, "FLOATING", "YES");
    IupSetAttribute(ih, "VISIBLE", "NO");
    IupSetAttribute(item, "VALUE", "OFF");
  }
  else
  {
    IupSetAttribute(ih, "FLOATING", "NO");
    IupSetAttribute(ih, "VISIBLE", "YES");
    IupSetAttribute(item, "VALUE", "ON");
  }

  IupRefresh(ih);  /* refresh the dialog layout */
}


/********************************** Callbacks *****************************************/


int dropfiles_cb(Ihandle* ih, const char* filename)
{
  if (save_check(ih))
    open_file(ih, filename);

  return IUP_DEFAULT;
}

int file_menu_open_cb(Ihandle* ih)
{
  Ihandle* item_revert = IupGetDialogChild(ih, "ITEM_REVERT");
  Ihandle* item_save = IupGetDialogChild(ih, "ITEM_SAVE");
  Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
  char* filename = IupGetAttribute(canvas, "FILENAME");
  int dirty = IupGetInt(canvas, "DIRTY");

  if (dirty)
    IupSetAttribute(item_save, "ACTIVE", "YES");
  else
    IupSetAttribute(item_save, "ACTIVE", "NO");

  if (dirty && filename)
    IupSetAttribute(item_revert, "ACTIVE", "YES");
  else
    IupSetAttribute(item_revert, "ACTIVE", "NO");
  return IUP_DEFAULT;
}

int edit_menu_open_cb(Ihandle* ih)
{
  Ihandle *clipboard = IupClipboard(); 

  Ihandle *item_paste = IupGetDialogChild(ih, "ITEM_PASTE");

  if (!IupGetInt(clipboard, "IMAGEAVAILABLE"))
    IupSetAttribute(item_paste, "ACTIVE", "NO");
  else
    IupSetAttribute(item_paste, "ACTIVE", "YES");

  IupDestroy(clipboard);
  return IUP_DEFAULT;
}

int config_recent_cb(Ihandle* ih)
{
  if (save_check(ih))
  {
    char* filename = IupGetAttribute(ih, "TITLE");
    open_file(ih, filename);
  }
  return IUP_DEFAULT;
}

int item_new_action_cb(Ihandle* item_new)
{
  if (save_check(item_new))
  {
    Ihandle* canvas = IupGetDialogChild(item_new, "CANVAS");
    Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
    int width = IupConfigGetVariableIntDef(config, "NewImage", "Width", 640);
    int height = IupConfigGetVariableIntDef(config, "NewImage", "Height", 480);

    if (IupGetParam("New Image", NULL, NULL, "Width: %i[1,]\nHeight: %i[1,]\n", &width, &height, NULL))
    {
      imImage* image = imImageCreate(width, height, IM_RGB, IM_BYTE);

      IupConfigSetVariableInt(config, "NewImage", "Width", width);
      IupConfigSetVariableInt(config, "NewImage", "Height", height);

      new_file(item_new, image);
    }
  }

  return IUP_DEFAULT;
}

int item_open_action_cb(Ihandle* item_open)
{
  Ihandle *filedlg;

  if (!save_check(item_open))
    return IUP_DEFAULT;

  filedlg = IupFileDlg();
  IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
  IupSetAttribute(filedlg, "EXTFILTER", "Image Files|*.bmp;*.jpg;*.png;*.tif;*.tga|All Files|*.*|");
  IupSetAttributeHandle(filedlg, "PARENTDIALOG", IupGetDialog(item_open));

  IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);
  if (IupGetInt(filedlg, "STATUS") != -1)
  {
    char* filename = IupGetAttribute(filedlg, "VALUE");
    open_file(item_open, filename);
  }

  IupDestroy(filedlg);
  return IUP_DEFAULT;
}

int item_saveas_action_cb(Ihandle* item_saveas)
{
  Ihandle* canvas = IupGetDialogChild(item_saveas, "CANVAS");
  Ihandle *filedlg = IupFileDlg();
  IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
  IupSetAttribute(filedlg, "EXTFILTER", "Image Files|*.bmp;*.jpg;*.png;*.tif;*.tga|All Files|*.*|");
  IupSetAttributeHandle(filedlg, "PARENTDIALOG", IupGetDialog(item_saveas));
  IupSetStrAttribute(filedlg, "FILE", IupGetAttribute(canvas, "FILENAME"));

  IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

  if (IupGetInt(filedlg, "STATUS") != -1)
  {
    char* filename = IupGetAttribute(filedlg, "VALUE");
    saveas_file(canvas, filename);
  }

  IupDestroy(filedlg);
  return IUP_DEFAULT;
}

int item_save_action_cb(Ihandle* item_save)
{
  Ihandle* canvas = IupGetDialogChild(item_save, "CANVAS");
  char* filename = IupGetAttribute(canvas, "FILENAME");
  if (!filename)
    item_saveas_action_cb(item_save);
  else   
  {
    /* test again because in can be called using the hot key */
    int dirty = IupGetInt(canvas, "DIRTY");
    if (dirty)
      save_file(canvas);
  }
  return IUP_DEFAULT;
}

int item_revert_action_cb(Ihandle* item_revert)
{
  Ihandle* canvas = IupGetDialogChild(item_revert, "CANVAS");
  char* filename = IupGetAttribute(canvas, "FILENAME");
  open_file(item_revert, filename);
  return IUP_DEFAULT;
}

int item_exit_action_cb(Ihandle* item_exit)
{
  Ihandle* dlg = IupGetDialog(item_exit);
  Ihandle* config = (Ihandle*)IupGetAttribute(dlg, "CONFIG");
  Ihandle* canvas = IupGetDialogChild(dlg, "CANVAS");
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");

  if (!save_check(item_exit))
    return IUP_IGNORE;  /* to abort the CLOSE_CB callback */

  if (image)
    imImageDestroy(image);

  IupConfigDialogClosed(config, dlg, "MainWindow");
  IupConfigSave(config);
  IupDestroy(config);
  return IUP_CLOSE;
}

int item_copy_action_cb(Ihandle* item_copy) 
{
  Ihandle* canvas = IupGetDialogChild(item_copy, "CANVAS");
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
  Ihandle *clipboard = IupClipboard();
  IupSetAttribute(clipboard, "NATIVEIMAGE", (char*)IupGetImageNativeHandle(image));
  IupDestroy(clipboard);
  return IUP_DEFAULT;
}

int item_paste_action_cb(Ihandle* item_paste) 
{
  if (save_check(item_paste))
  {
    Ihandle* canvas = IupGetDialogChild(item_paste, "CANVAS");
    imImage* old_image = (imImage*)IupGetAttribute(canvas, "IMAGE");

    Ihandle *clipboard = IupClipboard();
    imImage* image = IupGetNativeHandleImage(IupGetAttribute(clipboard, "NATIVEIMAGE"));

    if (image->color_space != IM_RGB)
    {
      imImage* new_image = imImageCreateBased(image, -1, -1, IM_RGB, -1);
      imConvertColorSpace(image, new_image);
      imImageDestroy(image);

      image = new_image;
    }

    imImageSetAttribString(image, "FileFormat", "JPEG");

    IupSetAttribute(canvas, "DIRTY", "Yes");
    IupSetAttribute(canvas, "IMAGE", (char*)image);
    IupSetAttribute(canvas, "FILENAME", NULL);
    IupSetAttribute(IupGetDialog(canvas), "TITLE", "Untitled - Simple Paint");

    IupUpdate(canvas);

    if (old_image)
      imImageDestroy(old_image);

    IupDestroy(clipboard);
  }
  return IUP_IGNORE;  /* replace system processing for the hot key, to correctly parse line feed */
}

int item_toolbar_action_cb(Ihandle* item_toolbar)
{
  Ihandle* canvas = IupGetDialogChild(item_toolbar, "CANVAS");
  Ihandle* toolbar = IupGetChild(IupGetParent(canvas), 0);
  Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");

  toggle_bar_visibility(item_toolbar, toolbar);

  IupConfigSetVariableStr(config, "MainWindow", "Toolbar", IupGetAttribute(item_toolbar, "VALUE"));
  return IUP_DEFAULT;
}

int item_statusbar_action_cb(Ihandle* item_statusbar)
{
  Ihandle* canvas = IupGetDialogChild(item_statusbar, "CANVAS");
  Ihandle* statusbar = IupGetBrother(canvas);
  Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");

  toggle_bar_visibility(item_statusbar, statusbar);

  IupConfigSetVariableStr(config, "MainWindow", "Statusbar", IupGetAttribute(item_statusbar, "VALUE"));
  return IUP_DEFAULT;
}

int item_help_action_cb(void)
{
  IupHelp("http://www.tecgraf.puc-rio.br/iup");
  return IUP_DEFAULT;
}

int item_about_action_cb(void)
{
  IupMessage("About", "   Simple Paint\n\nAutors:\n   Gustavo Lyrio\n   Antonio Scuri");
  return IUP_DEFAULT;
}


/********************************** Main *****************************************/


Ihandle* create_main_dialog(Ihandle *config)
{
  Ihandle *dlg, *vbox, *canvas, *menu;
  Ihandle *sub_menu_file, *file_menu, *item_exit, *item_new, *item_open, *item_save, *item_saveas, *item_revert;
  Ihandle *sub_menu_edit, *edit_menu, *item_copy, *item_paste;
  Ihandle *btn_copy, *btn_paste, *btn_new, *btn_open, *btn_save;
  Ihandle *sub_menu_help, *help_menu, *item_help, *item_about;
  Ihandle *sub_menu_view, *view_menu, *item_toolbar, *item_statusbar;
  Ihandle *lbl_statusbar, *toolbar_hb, *recent_menu;

  canvas = IupCanvas(NULL);
  IupSetAttribute(canvas, "MULTILINE", "YES");
  IupSetAttribute(canvas, "EXPAND", "YES");
  IupSetAttribute(canvas, "NAME", "CANVAS");
  IupSetAttribute(canvas, "DIRTY", "NO");
/* TODO: IupSetCallback(canvas, "ACTION", (Icallback)canvas_action_cb); */
  IupSetCallback(canvas, "DROPFILES_CB", (Icallback)dropfiles_cb);

  lbl_statusbar = IupLabel("(0, 0) = [0   0   0]");
  IupSetAttribute(lbl_statusbar, "NAME", "STATUSBAR");
  IupSetAttribute(lbl_statusbar, "EXPAND", "HORIZONTAL");
  IupSetAttribute(lbl_statusbar, "PADDING", "10x5");

  item_new = IupItem("&New\tCtrl+N", NULL);
  IupSetAttribute(item_new, "IMAGE", "IUP_FileNew");
  IupSetCallback(item_new, "ACTION", (Icallback)item_new_action_cb);
  btn_new = IupButton(NULL, NULL);
  IupSetAttribute(btn_new, "IMAGE", "IUP_FileNew");
  IupSetAttribute(btn_new, "FLAT", "Yes");
  IupSetCallback(btn_new, "ACTION", (Icallback)item_new_action_cb);
  IupSetAttribute(btn_new, "TIP", "New (Ctrl+N)");
  IupSetAttribute(btn_new, "CANFOCUS", "No");

  item_open = IupItem("&Open...\tCtrl+O", NULL);
  IupSetAttribute(item_open, "IMAGE", "IUP_FileOpen");
  IupSetCallback(item_open, "ACTION", (Icallback)item_open_action_cb);
  btn_open = IupButton(NULL, NULL);
  IupSetAttribute(btn_open, "IMAGE", "IUP_FileOpen");
  IupSetAttribute(btn_open, "FLAT", "Yes");
  IupSetCallback(btn_open, "ACTION", (Icallback)item_open_action_cb);
  IupSetAttribute(btn_open, "TIP", "Open (Ctrl+O)");
  IupSetAttribute(btn_open, "CANFOCUS", "No");

  item_save = IupItem("&Save\tCtrl+S", NULL);
  IupSetAttribute(item_save, "NAME", "ITEM_SAVE");
  IupSetAttribute(item_save, "IMAGE", "IUP_FileSave");
  IupSetCallback(item_save, "ACTION", (Icallback)item_save_action_cb);
  btn_save = IupButton(NULL, NULL);
  IupSetAttribute(btn_save, "IMAGE", "IUP_FileSave");
  IupSetAttribute(btn_save, "FLAT", "Yes");
  IupSetCallback(btn_save, "ACTION", (Icallback)item_save_action_cb);
  IupSetAttribute(btn_save, "TIP", "Save (Ctrl+S)");
  IupSetAttribute(btn_save, "CANFOCUS", "No");

  item_saveas = IupItem("Save &As...", NULL);
  IupSetAttribute(item_saveas, "NAME", "ITEM_SAVEAS");
  IupSetCallback(item_saveas, "ACTION", (Icallback)item_saveas_action_cb);

  item_revert = IupItem("&Revert", NULL);
  IupSetAttribute(item_revert, "NAME", "ITEM_REVERT");
  IupSetCallback(item_revert, "ACTION", (Icallback)item_revert_action_cb);

  item_exit = IupItem("E&xit", NULL);
  IupSetCallback(item_exit, "ACTION", (Icallback)item_exit_action_cb);

  item_copy = IupItem("&Copy\tCtrl+C", NULL);
  IupSetAttribute(item_copy, "NAME", "ITEM_COPY");
  IupSetAttribute(item_copy, "IMAGE", "IUP_EditCopy");
  IupSetCallback(item_copy, "ACTION", (Icallback)item_copy_action_cb);
  btn_copy = IupButton(NULL, NULL);
  IupSetAttribute(btn_copy, "IMAGE", "IUP_EditCopy");
  IupSetAttribute(btn_copy, "FLAT", "Yes");
  IupSetCallback(btn_copy, "ACTION", (Icallback)item_copy_action_cb);
  IupSetAttribute(btn_copy, "TIP", "Copy (Ctrl+C)");
  IupSetAttribute(btn_copy, "CANFOCUS", "No");

  item_paste = IupItem("&Paste\tCtrl+V", NULL);
  IupSetAttribute(item_paste, "NAME", "ITEM_PASTE");
  IupSetAttribute(item_paste, "IMAGE", "IUP_EditPaste");
  IupSetCallback(item_paste, "ACTION", (Icallback)item_paste_action_cb);
  btn_paste = IupButton(NULL, NULL);
  IupSetAttribute(btn_paste, "IMAGE", "IUP_EditPaste");
  IupSetAttribute(btn_paste, "FLAT", "Yes");
  IupSetCallback(btn_paste, "ACTION", (Icallback)item_paste_action_cb);
  IupSetAttribute(btn_paste, "TIP", "Paste (Ctrl+V)");
  IupSetAttribute(btn_paste, "CANFOCUS", "No");

  item_toolbar = IupItem("&Toobar...", NULL);
  IupSetCallback(item_toolbar, "ACTION", (Icallback)item_toolbar_action_cb);
  IupSetAttribute(item_toolbar, "VALUE", "ON");

  item_statusbar = IupItem("&Statusbar...", NULL);
  IupSetCallback(item_statusbar, "ACTION", (Icallback)item_statusbar_action_cb);
  IupSetAttribute(item_statusbar, "VALUE", "ON");

  item_help = IupItem("&Help...", NULL);
  IupSetCallback(item_help, "ACTION", (Icallback)item_help_action_cb);

  item_about = IupItem("&About...", NULL);
  IupSetCallback(item_about, "ACTION", (Icallback)item_about_action_cb);

  recent_menu = IupMenu(NULL);

  file_menu = IupMenu(
    item_new,
    item_open,
    item_save,
    item_saveas,
    item_revert,
    IupSeparator(),
    IupSubmenu("Recent &Files", recent_menu),
    item_exit,
    NULL);
  edit_menu = IupMenu(
    item_copy,
    item_paste,
    IupSeparator(),
    NULL);
  view_menu = IupMenu(
    item_toolbar,
    item_statusbar,
    NULL);
  help_menu = IupMenu(
    item_help,
    item_about,
    NULL);

  IupSetCallback(file_menu, "OPEN_CB", (Icallback)file_menu_open_cb);
  IupSetCallback(edit_menu, "OPEN_CB", (Icallback)edit_menu_open_cb);

  sub_menu_file = IupSubmenu("&File", file_menu);
  sub_menu_edit = IupSubmenu("&Edit", edit_menu);
  sub_menu_view = IupSubmenu("&View", view_menu);
  sub_menu_help = IupSubmenu("&Help", help_menu);

  menu = IupMenu(
    sub_menu_file,
    sub_menu_edit,
    sub_menu_view,
    sub_menu_help,
    NULL);

  toolbar_hb = IupHbox(
    btn_new,
    btn_open,
    btn_save,
    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
    btn_copy,
    btn_paste,
    NULL);
  IupSetAttribute(toolbar_hb, "MARGIN", "5x5");
  IupSetAttribute(toolbar_hb, "GAP", "2");

  vbox = IupVbox(
    toolbar_hb,
    canvas,
    lbl_statusbar,
    NULL);

  dlg = IupDialog(vbox);
  IupSetAttributeHandle(dlg, "MENU", menu);
  IupSetAttribute(dlg, "SIZE", "HALFxHALF");
  IupSetCallback(dlg, "CLOSE_CB", (Icallback)item_exit_action_cb);
  IupSetCallback(dlg, "DROPFILES_CB", (Icallback)dropfiles_cb);

  IupSetCallback(dlg, "K_cN", (Icallback)item_new_action_cb);
  IupSetCallback(dlg, "K_cO", (Icallback)item_open_action_cb);
  IupSetCallback(dlg, "K_cS", (Icallback)item_save_action_cb);
  IupSetCallback(dlg, "K_cV", (Icallback)item_paste_action_cb);
  IupSetCallback(dlg, "K_cC", (Icallback)item_copy_action_cb);

  /* parent for pre-defined dialogs in closed functions (IupMessage and IupAlarm) */
  IupSetAttributeHandle(NULL, "PARENTDIALOG", dlg);

  /* Initialize variables from the configuration file */

  IupConfigRecentInit(config, recent_menu, config_recent_cb, 10);

  if (!IupConfigGetVariableIntDef(config, "MainWindow", "Toolbar", 1))
  {
    IupSetAttribute(item_toolbar, "VALUE", "OFF");

    IupSetAttribute(toolbar_hb, "FLOATING", "YES");
    IupSetAttribute(toolbar_hb, "VISIBLE", "NO");
  }

  if (!IupConfigGetVariableIntDef(config, "MainWindow", "Statusbar", 1))
  {
    IupSetAttribute(item_statusbar, "VALUE", "OFF");

    IupSetAttribute(lbl_statusbar, "FLOATING", "YES");
    IupSetAttribute(lbl_statusbar, "VISIBLE", "NO");
  }

  IupSetAttribute(dlg, "CONFIG", (char*)config);

  return dlg;
}

int main(int argc, char **argv)
{
  Ihandle *dlg;
  Ihandle *config;

  IupOpen(&argc, &argv);
  IupImageLibOpen();

  config = IupConfig();
  IupSetAttribute(config, "APP_NAME", "simple_paint");
  IupConfigLoad(config);

  dlg = create_main_dialog(config);

  /* show the dialog at the last position, with the last size */
  IupConfigDialogShow(config, dlg, "MainWindow");

  /* open a file from the command line (allow file association in Windows) */
  if (argc > 1 && argv[1])
  {
    const char* filename = argv[1];
    open_file(dlg, filename);
  }

  /* initialize the current file, if not loaded */
  check_new_file(dlg);

  IupMainLoop();

  IupClose();
  return EXIT_SUCCESS;
}
