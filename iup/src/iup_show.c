/** \file
 * \brief show/popup/hide/map
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdlib.h>
#include <stdarg.h>

#include "iup.h"

#include "iup_object.h"
#include "iup_layout.h"
#include "iup_attrib.h"
#include "iup_class.h"
#include "iup_dialog.h"
#include "iup_menu.h"
#include "iup_assert.h"
#include "iup_str.h"
#include "iup_drv.h"
#include "iup_drvfont.h"


void IupUnmap(Ihandle *ih)
{
  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return;

  /* Invalid if it is NOT mapped. */
  if (!ih->handle)
    return;

  /* unmap children */
  {
    Ihandle* child = ih->firstchild;
    while (child)
    {
      IupUnmap(child);
      child = child->brother;
    }
  }

  /* only call UNMAP_CB for controls that have a native representation */
  if (ih->iclass->nativetype != IUP_TYPEVOID)
  {
    Icallback unmap_cb = IupGetCallback(ih, "UNMAP_CB");
    if (unmap_cb) unmap_cb(ih);
  }

  /* unmap from the native system */
  iupClassObjectUnMap(ih);
  ih->handle = NULL;
}

int IupMap(Ihandle* ih)
{
  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return IUP_INVALID;

  /* calculate position and size for all children */
  if (ih->iclass->nativetype == IUP_TYPEDIALOG)
    iupLayoutCompute(ih);

  if (ih->parent && !(ih->parent->handle))
    return IUP_ERROR;

  /* already mapped */
  if (ih->handle)
  {
    if (ih->iclass->nativetype == IUP_TYPEDIALOG)
      iupLayoutUpdate(ih);

    return IUP_NOERROR;
  }
    
  /* map to the native system */
  if (iupClassObjectMap(ih) == IUP_ERROR)
  {
    iupERROR("Error during IupMap.");
    return IUP_ERROR;
  }

  /* update FONT, must be the before several others, so we do it here */
  if (ih->iclass->nativetype != IUP_TYPEVOID &&
      ih->iclass->nativetype != IUP_TYPEIMAGE &&
      ih->iclass->nativetype != IUP_TYPEMENU)
    iupUpdateStandardFontAttrib(ih);

  /* ensure attributes default values, at this time only the ones that need to be set after map */
  iupClassObjectEnsureDefaultAttributes(ih);

  /* updates the defined attributes from the hash table to the native system. */
  iupAttribUpdate(ih); 

  /* updates inheritable attributes defined in the parent tree */
  iupAttribUpdateFromParent(ih);

  /* map children independent from childtype */
  {
    Ihandle* child = ih->firstchild;
    while (child)
    {
      if (IupMap(child) == IUP_ERROR)
        return IUP_ERROR;

      child = child->brother;
    }
  }

  /* updates the defined attributes from the hash table to the native system. */
  if (ih->iclass->childtype!=IUP_CHILDNONE)
    iupAttribUpdateChildren(ih);

  /* moves and resizes the elements to reflect the layout computation */
  /* if the dialog is visible will be reflected in the user interface */
  if (ih->iclass->nativetype == IUP_TYPEDIALOG)
    iupLayoutUpdate(ih);

  /* only call MAP_CB for controls that have a native representation */
  if (ih->iclass->nativetype != IUP_TYPEVOID)
  {
    Icallback map_cb = IupGetCallback(ih, "MAP_CB");
    if (map_cb) map_cb(ih);
  }

  return IUP_NOERROR;
}

int IupPopup(Ihandle *ih, int x, int y)
{
  int ret;

  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return IUP_INVALID;

  if (ih->iclass->nativetype != IUP_TYPEDIALOG && 
      ih->iclass->nativetype != IUP_TYPEMENU)
  {
    iupERROR("Must be a menu or dialog in IupPopup.");
    return IUP_INVALID;
  }

  ret = IupMap(ih);
  if (ret == IUP_ERROR) 
    return ret;

  if (ih->iclass->nativetype == IUP_TYPEDIALOG)
    ret = iupDialogPopup(ih, x, y);
  else
    ret = iupMenuPopup(ih, x, y);

  if (ret != IUP_NOERROR) 
  {
    iupERROR("Error during IupPopup.");
    return ret;
  }

  return IUP_NOERROR;
}

int IupShowXY(Ihandle *ih, int x, int y)
{
  int ret;

  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return IUP_INVALID;

  if (ih->iclass->nativetype != IUP_TYPEDIALOG)
  {
    iupERROR("Must be a dialog in IupShowXY.");
    return IUP_INVALID;
  }

  ret = IupMap(ih);
  if (ret == IUP_ERROR) 
    return ret;

  ret = iupDialogShowXY(ih, x, y);
  if (ret != IUP_NOERROR) 
  {
    iupERROR("Error during IupShowXY.");
    return ret;
  }

  return IUP_NOERROR;
}

int IupShow(Ihandle* ih)
{
  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return IUP_INVALID;

  if (ih->iclass->nativetype != IUP_TYPEDIALOG)
    IupSetAttribute(ih, "VISIBLE", "YES");
  else   
  {
    int ret = IupMap(ih);
    if (ret == IUP_ERROR) 
      return ret;
      
    ret = iupDialogShowXY(ih, IUP_CURRENT, IUP_CURRENT);
    if (ret != IUP_NOERROR) 
    {
      iupERROR("Error during IupShow.");
      return ret;
    }
  }

  return IUP_NOERROR;
}

int IupHide(Ihandle* ih)
{
  iupASSERT(iupObjectCheck(ih));
  if (!iupObjectCheck(ih))
    return IUP_INVALID; 

  if (ih->iclass->nativetype != IUP_TYPEDIALOG)
    IupSetAttribute(ih, "VISIBLE", "NO");
  else if (ih->handle)
    iupDialogHide(ih);

  return IUP_NOERROR;
}
