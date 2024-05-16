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
#include "../tcl/tcl.h"
#include "../structs/structs.h"

#include <tcl/tcl.h>
// #include <tcl.h>

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

extern GtkWidget *buttons; // buttons container //
extern GtkWidget *button1; // button 1 //
extern GtkWidget *list_cells_button; // button 1 //
extern GtkWidget *hierarchybrowserframe; // hierarchy browser frame //
extern GtkWidget *random_place_button_2;
extern GtkWidget *report_hpwl_button;

extern int reversey; // reverse y-axis flag, place origin at bottom-left //

extern double scaledvpagesize, scaledhpagesize, vstep, hstep, vpagesize, hpagesize; // horizontal and vertical scrollbars adjustment parameters //

extern int maincanvasWidth; // main canvas visible dimension - width //
extern int maincanvasHeight; // main canvas visible dimension - height //
extern int maincanvasOx; // main canvas visible origin translation - x offset //
extern int maincanvasOy; // main canvas visible origin translation - y offset //

extern GtkWidget *mainwindow; // your main window //

extern double current_scale; // zoom factor for main canvas //

extern GtkAdjustment *adjust_scrollbar;

extern double offset_x;
extern double offset_y;

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
static void flowbuttonclicked(GtkButton *button, gpointer data);
static void create_buttons_frame();
static void list_cells_button_clicked(GtkButton *button, gpointer data);
double translate_um_to_pixels(double um);
void find_cell_pos(double mouse_x, double mouse_y);