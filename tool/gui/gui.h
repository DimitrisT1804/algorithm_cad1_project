#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <limits.h>

// #define DEBUGGUI

extern GtkWidget *maincanvas_scroll; // scrolled window container for main canvas //
extern GtkWidget *maincanvas; // main canvas widget // 
extern cairo_t *maincanvas_cs; // main canvas cairo state // 
extern GdkDrawable *maincanvas_drawable; // drawable layout window of maincanvas //
extern GtkWidget *maincanvas_fixed;
extern GtkWidget *maincanvashscrollbar; // horizontal scrollbar for maincanvas //
extern GtkWidget *maincanvasvscrollbar; // vertical scrollbar for maincanvas //
extern GtkObject *maincanvashscrollbaradjustment; // horizontal scrollbar adjustment //
extern GtkObject *maincanvasvscrollbaradjustment; // vertical scrollbar adjustment //

extern int reversey; // reverse y-axis flag, place origin at bottom-left //

extern double scaledvpagesize, scaledhpagesize, vstep, hstep, vpagesize, hpagesize; // horizontal and vertical scrollbars adjustment parameters //

extern int maincanvasWidth; // main canvas visible dimension - width //
extern int maincanvasHeight; // main canvas visible dimension - height //
extern int maincanvasOx; // main canvas visible origin translation - x offset //
extern int maincanvasOy; // main canvas visible origin translation - y offset //

extern GtkWidget *mainwindow; // your main window //

extern double current_scale; // zoom factor for main canvas //

extern GtkAdjustment *adjust_scrollbar;

// *** Function Prototypes *** //

static void expose(GtkWidget *, GdkEventExpose *, gpointer);
static void maincanvaspaint(GtkWidget *, GdkEventExpose *, gpointer);
static void resizemaincanvas(GtkWidget *, GdkRectangle *, gpointer);
static gboolean maincanvasvscroll(GtkRange *, GtkScrollType, gdouble, gpointer);
static gboolean maincanvashscroll(GtkRange *, GtkScrollType, gdouble, gpointer);
static void scroll(GtkWidget *, GdkEventScroll *, gpointer);
void setupscrolladjustments();
static void mousebutton(GtkWidget *, GdkEventButton *, gpointer);
static void quitaction();
void start_gui();
double max_double(double a, double b);