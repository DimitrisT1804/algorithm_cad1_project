// Start of CAD 2 //

#include <gtk/gtk.h>
// #include <gtk-2.0/gtk/gtk.h>
// #include <gtk-4.0/gtk/gtk.h>
// #include <gtk-2.0/gdk/gdkx.h>
#include <gdk/gdk.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define DEBUGMODE

GtkWidget *maincanvas_scroll; // scrolled window container for main canvas //
GtkWidget *maincanvas; // main canvas widget // 
cairo_t *maincanvas_cs; // main canvas cairo state // 
GdkDrawable *maincanvas_drawable; // drawable layout window of maincanvas //

GtkWidget *maincanvas_fixed;

GtkWidget *maincanvashscrollbar; // horizontal scrollbar for maincanvas //
GtkWidget *maincanvasvscrollbar; // vertical scrollbar for maincanvas //

GtkObject *maincanvashscrollbaradjustment; // horizontal scrollbar adjustment //
GtkObject *maincanvasvscrollbaradjustment; // vertical scrollbar adjustment //

int reversey = 0; // reverse y-axis flag, place origin at bottom-left //

double scaledvpagesize, scaledhpagesize, vstep, hstep, vpagesize, hpagesize; // horizontal and vertical scrollbars adjustment parameters //

int maincanvasWidth = 550; // main canvas visible dimension - width //
int maincanvasHeight = 550; // main canvas visible dimension - height //
int maincanvasOx = 0; // main canvas visible origin translation - x offset //
int maincanvasOy = 0; // main canvas visible origin translation - y offset //

GtkWidget *mainwindow; // your main window //

GtkWidget *horizontal_box; // Declare a global variable for the horizontal box

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
void setup_horizontal_box();
void start_gui();