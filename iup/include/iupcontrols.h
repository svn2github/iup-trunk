/** \file
 * \brief initializes dial, gauge, colorbrowser, colorbar controls.
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUPCONTROLS_H 
#define __IUPCONTROLS_H

#ifdef __cplusplus
extern "C" {
#endif


int  IupControlsOpen(void);

Ihandle* IupColorbar(void);
Ihandle* IupCells(void);
Ihandle *IupColorBrowser(void);
Ihandle *IupGauge(void);
Ihandle *IupDial(const char* type);
Ihandle* IupMatrix(const char *action);
Ihandle* IupMatrixList(void);

/* IupMatrix utilities (DEPRECATED, use IupSetAttributeId2 functions). It will be removed in a future version. */
void  IupMatSetAttribute  (Ihandle* ih, const char* name, int lin, int col, const char* value);
void  IupMatStoreAttribute(Ihandle* ih, const char* name, int lin, int col, const char* value);
char* IupMatGetAttribute  (Ihandle* ih, const char* name, int lin, int col);
int   IupMatGetInt        (Ihandle* ih, const char* name, int lin, int col);
float IupMatGetFloat      (Ihandle* ih, const char* name, int lin, int col);
void  IupMatSetfAttribute (Ihandle* ih, const char* name, int lin, int col, const char* format, ...);

/* Used by IupColorbar */
#define IUP_PRIMARY -1
#define IUP_SECONDARY -2

/* available only when linking with "iupluamatrix" */
void IupMatrixSetFormula(Ihandle* ih, int col, const char* formula, const char* init);
void IupMatrixSetDynamic(Ihandle* ih, const char* init);


#ifdef __cplusplus
}
#endif

#endif
