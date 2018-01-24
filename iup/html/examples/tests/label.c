#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iup.h"
#include "iupim.h"


#define TEST_IMAGE_SIZE 20

static unsigned char image_data_8 [TEST_IMAGE_SIZE*TEST_IMAGE_SIZE] = 
{
  5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
  5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
  5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
  5,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,0,0,0,5, 
  5,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,0,0,0,5, 
  5,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,0,0,0,5, 
  5,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,0,0,0,5, 
  5,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,0,0,0,5, 
  5,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,0,0,0,5, 
  5,0,0,0,3,3,3,3,3,3,4,4,4,4,4,4,0,0,0,5, 
  5,0,0,0,3,3,3,3,3,3,4,4,4,4,4,4,0,0,0,5, 
  5,0,0,0,3,3,3,3,3,3,4,4,4,4,4,4,0,0,0,5, 
  5,0,0,0,3,3,3,3,3,3,4,4,4,4,4,4,0,0,0,5, 
  5,0,0,0,3,3,3,3,3,3,4,4,4,4,4,4,0,0,0,5, 
  5,0,0,0,3,3,3,3,3,3,4,4,4,4,4,4,0,0,0,5,
  5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
  5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
  5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
  5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
};

static unsigned char image_data_24 [TEST_IMAGE_SIZE*TEST_IMAGE_SIZE*3] = 
{
  000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
  000,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000,
  000,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000,
  000,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000,
  000,000,000,255,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000, 
  000,000,000,255,255,255,255,255,255,255,255,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000,
  000,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000,
  000,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000,
  000,000,000,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,000,000,000,
  000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
};

static unsigned char image_data_32 [TEST_IMAGE_SIZE*TEST_IMAGE_SIZE*4] = 
{
  000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255, 
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,000,000,255,255,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,255,255,255,192,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,
  000,000,000,255,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,255,
  000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,000,000,000,255,
};

static int button_cb(Ihandle *ih, int but, int pressed, int x, int y, char* status)
{
  printf("BUTTON_CB(but=%c (%d), x=%d, y=%d [%s])\n", (char)but, pressed, x, y, status);
  return IUP_DEFAULT;
}

static int enter_cb(Ihandle *ih)
{
  Ihandle *box = IupGetChild(ih, 0);
  if (IupGetInt(box, "ACTIVE"))
    IupSetAttribute(box, "ACTIVE", "NO");
  else
    IupSetAttribute(box, "ACTIVE", "YES");
  return IUP_DEFAULT;
}

static int leavewindow_cb(Ihandle *ih)
{
  printf("LEAVEWINDOW_CB(%s)\n", IupGetAttribute(ih, "TITLE"));
  return IUP_DEFAULT;
}

static int enterwindow_cb(Ihandle *ih)
{
  printf("ENTERWINDOW_CB(%s)\n", IupGetAttribute(ih, "TITLE"));
  return IUP_DEFAULT;
}

void LabelTest(void)
{
  Ihandle *dlg, *label, *image1, *image2, *image3, 
          *box1, *box2, *fill;

  box1 = IupVbox(NULL);
  IupSetAttribute(box1, "MARGIN", "5x5");
  IupSetAttribute(box1, "GAP", "5");
  IupSetAttribute(box1, "BGCOLOR", "75 150 170");  /* label must be transparent for BGCOLOR */
  IupSetAttribute(box1, "PADDING", "5x5");

  label = IupLabel(NULL);
  if (IupGetInt(NULL, "UTF8MODE"))
    IupSetAttribute(label, "TITLE", "Text Labels (çãõáóé)");
  else
    IupSetAttribute(label, "TITLE", "Text Labels (������)");
  //IupSetAttribute(label, "BGCOLOR", IupGetGlobal("DLGBGCOLOR"));
  IupSetAttribute(label, "PADDING", "0x0");
  IupSetAttribute(label, "TIP", "Text Label");
  IupAppend(box1, label);
//  IupSetCallback(label, "ENTERWINDOW_CB", (Icallback)enterwindow_cb);
//  IupSetCallback(label, "LEAVEWINDOW_CB", (Icallback)leavewindow_cb);
  IupSetAttribute(label, "PADDING", "20x20");

  label = IupLabel(NULL);
  IupSetAttribute(label, "SEPARATOR", "HORIZONTAL");
  IupAppend(box1, label);
//  IupSetCallback(label, "ENTERWINDOW_CB", (Icallback)enterwindow_cb);
//  IupSetCallback(label, "LEAVEWINDOW_CB", (Icallback)leavewindow_cb);

  label = IupLabel(NULL);
  IupSetAttribute(label, "TITLE", "Text &Label\nSecond Line");
  IupSetAttribute(label, "FGCOLOR", "0 0 255");
  IupSetAttribute(label, "RASTERSIZE", "150x70");
//  IupSetAttribute(label, "RASTERSIZE", "30x70");
//  IupSetAttribute(label, "WORDWRAP", "YES");
//  IupSetAttribute(label, "ELLIPSIS", "YES");
  IupSetAttribute(label, "ALIGNMENT", "ALEFT:ATOP");
  IupAppend(box1, label);
//  IupSetCallback(label, "ENTERWINDOW_CB", (Icallback)enterwindow_cb);
//  IupSetCallback(label, "LEAVEWINDOW_CB", (Icallback)leavewindow_cb);

  label = IupLabel(NULL);
  IupSetAttribute(label, "TITLE", "Text\nSecond Line");
  IupSetAttribute(label, "RASTERSIZE", "150x70");
  IupSetAttribute(label, "ALIGNMENT", "ACENTER:ACENTER");
  IupSetAttribute(label, "FONT", "Helvetica, Underline 14");
//  IupSetAttribute(label, "FONTSIZE", "14");
//  IupSetAttribute(label, "FONTSTYLE", "Italic");
//  IupSetAttribute(label, "FONTSTYLE", "Bold");
  IupAppend(box1, label);
//  IupSetCallback(label, "ENTERWINDOW_CB", (Icallback)enterwindow_cb);
//  IupSetCallback(label, "LEAVEWINDOW_CB", (Icallback)leavewindow_cb);

  label = IupLabel(NULL);
  IupSetAttribute(label, "TITLE", "Text\n<b>Second Line</b>");
  IupSetAttribute(label, "RASTERSIZE", "150x70");
  IupSetAttribute(label, "ALIGNMENT", "ARIGHT:ABOTTOM");
  IupSetAttribute(label, "MARKUP", "YES");
//  IupSetCallback(label, "ENTERWINDOW_CB", enter1_cb);
  IupAppend(box1, label);
//  IupSetCallback(label, "ENTERWINDOW_CB", (Icallback)enterwindow_cb);
//  IupSetCallback(label, "LEAVEWINDOW_CB", (Icallback)leavewindow_cb);

  box2 = IupVbox(NULL);
  IupSetAttribute(box2, "MARGIN", "5x5");
  IupSetAttribute(box2, "GAP", "5");
  IupSetAttribute(box2, "BGCOLOR", "75 150 170");
//  IupSetAttribute(box2, "PADDING", "5x5");

  image1 = IupImage(TEST_IMAGE_SIZE, TEST_IMAGE_SIZE, image_data_8);
  IupSetAttribute(image1, "0", "BGCOLOR");
  IupSetAttribute(image1, "1", "255 0 0"); 
  IupSetAttribute(image1, "2", "0 255 0");
  IupSetAttribute(image1, "3", "0 0 255"); 
  IupSetAttribute(image1, "4", "255 255 255"); 
  IupSetAttribute(image1, "5", "0 0 0"); 

  label = IupLabel(NULL);
  IupSetAttributeHandle(label, "IMAGE", image1);
  //IupSetAttribute(label, "IMAGE", "TECGRAF_BITMAP");
  //IupSetAttribute(label, "IMAGE", "../test/tecgraf.bmp");
  //IupSetAttribute(label, "IMAGE", "../test/file_large.xbm");
  //IupSetAttribute(label, "IMAGE", "gtk-open");
  //IupSetAttribute(label, "BGCOLOR", IupGetGlobal("DLGBGCOLOR"));
  IupSetAttribute(label, "PADDING", "0x0");
  IupSetAttribute(label, "TIP", "Image Label");
  IupAppend(box2, label);

  label = IupLabel(NULL);
  IupSetAttribute(label, "SEPARATOR", "HORIZONTAL");
  IupAppend(box2, label);

  fill = IupFill();
  IupSetAttribute(fill, "SIZE", "20");
  IupAppend(box2, fill);

  label = IupLabel(NULL);
  IupSetAttributeHandle(label, "IMAGE", image1);
  IupSetAttribute(label, "RASTERSIZE", "150x50");
  IupAppend(box2, label);

  image2 = IupImageRGB(TEST_IMAGE_SIZE, TEST_IMAGE_SIZE, image_data_24);

  label = IupLabel(NULL);
  IupSetAttributeHandle(label, "IMAGE", image2);
  IupSetAttribute(label, "RASTERSIZE", "150x50");
  IupSetAttribute(label, "ALIGNMENT", "ACENTER");
  IupAppend(box2, label);

  image3 = IupImageRGBA(TEST_IMAGE_SIZE, TEST_IMAGE_SIZE, image_data_32);

  label = IupLabel(NULL);
  IupSetAttributeHandle(label, "IMAGE", image3);
  IupSetAttribute(label, "RASTERSIZE", "150x50");
  IupSetAttribute(label, "ALIGNMENT", "ARIGHT");
  IupSetCallback(label, "BUTTON_CB", (Icallback)button_cb);
  IupAppend(box2, label);

  label = IupAnimatedLabel(NULL);
  //label = IupAnimatedLabel(IupLoadAnimation("D:\\tecgraf\\iup\\html\\examples\\tests\\loading.gif"));
  //IupSetAttributeHandle(label, "ANIMATION", IupLoadAnimation("D:\\tecgraf\iup\\html\\examples\\tests\\loading.gif"));
  IupSetAttribute(label, "ANIMATION", "IUP_CircleProgressAnimation");
  IupSetAttribute(label, "START", "Yes");
  IupAppend(box2, label);

  label = IupLabel(NULL);
  IupSetAttribute(label, "SEPARATOR", "VERTICAL");

  dlg = IupDialog(IupHbox(box1, label, box2, NULL));
  IupSetAttribute(dlg, "TITLE", "IupLabel Test");
//  IupSetAttribute(dlg, "BACKGROUND", "0 0 128");
//  IupSetAttributeHandle(dlg, "BACKGROUND", image2);
//  IupSetCallback(dlg, "ENTERWINDOW_CB", enter_cb);
//  IupSetAttribute(dlg, "FONT", "Helvetica, Bold 12");

  IupShow(dlg);
//  IupShowXY(dlg,IUP_LEFT,IUP_BOTTOM);
}

#ifndef BIG_TEST
int main(int argc, char* argv[])
{
  IupOpen(&argc, &argv);
  IupImageLibOpen();

  LabelTest();

  IupMainLoop();

  IupClose();

  return EXIT_SUCCESS;
}
#endif

