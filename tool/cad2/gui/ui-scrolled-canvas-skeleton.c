// Start of CAD 2 //
#include "ui-scrolled-canvas-skeleton.h"

static void expose(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
   #ifdef DEBUGMODE
    {
      printf("DEBUG: Canvas Expose\n");
    }
   #endif  

  maincanvaspaint(widget, event, data);
}

// Expose-Event Paint Function for maincanvas //
static void maincanvaspaint(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  maincanvas_drawable = maincanvas->window; // drawable drawing area window //
  //  maincanvas_drawable = GTK_LAYOUT(maincanvas)->bin_window; // drawable layout window //
  maincanvas_cs = gdk_cairo_create(maincanvas_drawable); // corresponding cairo state //

  // call main drawing function //

  cairo_destroy(maincanvas_cs);
}

// Size-Allocate Event Function for maincanvas // 
static void resizemaincanvas(GtkWidget *widget, GdkRectangle *gdkrect, gpointer data)
{
  int newmaincanvasWidth;
  int newmaincanvasHeight;
  
   #ifdef DEBUGMODE
    {
      printf("DEBUG: Resize Main Canvas.\n");
      printf("DEBUG: Current Width, Height = (%d, %d), New Width, Height = (%d, %d)\n", maincanvasWidth, maincanvasHeight, gdkrect->width, gdkrect->height);
    }
   #endif

  // resize code //

}

static gboolean maincanvasvscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
  double maxmaincanvasOy;
  int ivalue; // calibrated, inter y-offset value, based on vertical scrollbar value //

   #ifdef DEBUGMODE
    {
      printf("DEBUG: Vertical Scroll\n");
      printf("DEBUG: Scroll value = %.3f\n", value);
    }
   #endif  

  // vertical scrollbar code here //

}

static gboolean maincanvashscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
  double maxmaincanvasOx;
  int ivalue; // calibrated, inter x-offset value, based on horizontal scrollbar value //

   #ifdef DEBUGMODE
    {
      printf("DEBUG: Horizontal Scroll\n");      
      printf("DEBUG: Scroll value = %.3f\n", value);
    }
   #endif  

  // horizontal scrollbar code here //

}

static void scroll(GtkWidget *widget, GdkEventScroll *eev, gpointer data)
{
   #ifdef DEBUGMODE
    {
      printf("DEBUG: Canvas Mouse Scroll\n");
    }
   #endif

  // mouse wheel (scroll) code here //
}

void setupscrolladjustments()
{
  // setup horizontal and vertical scrollbar adjustments //

  // setup horizontal and vertical scrollbar adjustments //
  // scaledhpagesize =  zoomscale * ((double) maincanvasWidth);
  // scaledvpagesize =  zoomscale * ((double) maincanvasHeight);
  hpagesize = ((double) maincanvasWidth);
  vpagesize = ((double) maincanvasHeight);
  hstep = 0.05 * hpagesize; // 5% of maincanvasWidth horizontal step // 
  vstep = 0.05 * vpagesize; // 5% of maincanvasHeight vertical step //

  gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvasvscrollbaradjustment), maincanvasOy, 0.0, scaledvpagesize, vstep, vstep, vpagesize);
  gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvashscrollbaradjustment), maincanvasOx, 0.0, scaledhpagesize, hstep, hstep, hpagesize);
}

static void mousebutton(GtkWidget *widget, GdkEventButton *eev, gpointer data)
{
   #ifdef DEBUGMODE
    {
      printf("DEBUG: Mouse Button %d Pressed\n", eev->button);
    }
   #endif

  if (eev->button == 1) // Left Mouse Button //
    {
      // code here //
    }

  if (eev->button == 3) // Right Mouse Button //
    {
      // code here //
    }
  
}

static void quitaction()
{
  #ifdef DEBUGMODE
    {
      printf("DEBUG: Quit Action\n");
    }
  #endif
  
  printf("Thank you. Bye now.\n");
  gtk_main_quit();
}


void setup_horizontal_box() 
{
    // Create the horizontal box
    horizontal_box = gtk_hbox_new(FALSE, 5); // 5 is the spacing between widgets

    // Add widgets to the horizontal box (example buttons)
    GtkWidget *button1 = gtk_button_new_with_label("Button 1");
    GtkWidget *button2 = gtk_button_new_with_label("Button 2");
    GtkWidget *button3 = gtk_button_new_with_label("Button 3");

    gtk_box_pack_start(GTK_BOX(horizontal_box), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(horizontal_box), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(horizontal_box), button3, TRUE, TRUE, 0);
}

void start_gui()
{  
  // *** Local Variables *** //

  GtkWidget *maincanvashbox; // contains maincanvas and vertical scrollbar //
  GtkWidget *maincanvasvbox; // contains maincanvashbox and horizontal scrollbar //

  GtkWidget *hpane; // horizontal pane //
  GtkWidget *hpane2; // horizontal pane //

  GtkWidget *mainframe; // main canvas frame //
  GtkWidget *hierarchybrowserframe; // hierarchy browser frame //

  // *** Local Variables End *** //

  gtk_init(NULL,  NULL);

  mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title(GTK_WINDOW(mainwindow), "CADII - Assignment 2");
  gtk_window_set_default_size(GTK_WINDOW(mainwindow), 500, 500); // default size //
  
  g_signal_connect(G_OBJECT(mainwindow), "destroy", G_CALLBACK(quitaction), mainwindow);

  maincanvashbox = gtk_hbox_new(FALSE, 0);
  maincanvasvbox = gtk_vbox_new(FALSE, 0);

  // Frames //
  mainframe = gtk_frame_new("Layout View");
  hierarchybrowserframe = gtk_frame_new("Other Frame");

  // Canvas Drawing Area, Frames and Associated Callbacks //
  maincanvas = gtk_drawing_area_new();
  //  maincanvas = gtk_layout_new(NULL, NULL);

  gtk_widget_set_size_request(maincanvas, maincanvasWidth, maincanvasHeight);

  // Application Paintable and Double-Buffering Flags for maincanvas //
  //  gtk_widget_set_app_paintable(maincanvas, FALSE);
  //  gtk_widget_set_double_buffered(maincanvas, FALSE);

  // maincanvas Event Handlers //
  g_signal_connect(G_OBJECT(maincanvas), "expose-event", G_CALLBACK(expose), maincanvas);

  gtk_widget_add_events(maincanvas, GDK_SCROLL);
  gtk_widget_add_events(maincanvas, GDK_BUTTON_PRESS_MASK);

  g_signal_connect(G_OBJECT(maincanvas), "scroll-event", G_CALLBACK(scroll), maincanvas);
  g_signal_connect(G_OBJECT(maincanvas), "button-press-event", G_CALLBACK(mousebutton), maincanvas);
  
  maincanvasvscrollbaradjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  maincanvashscrollbaradjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // setup horizontal and vertical scrollbar adjustments //
  setupscrolladjustments();

  maincanvasvscrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(maincanvasvscrollbaradjustment));
  maincanvashscrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(maincanvashscrollbaradjustment));

  g_signal_connect(G_OBJECT(maincanvasvscrollbar), "change-value", G_CALLBACK(maincanvasvscroll), maincanvasvscrollbar);
  g_signal_connect(G_OBJECT(maincanvashscrollbar), "change-value", G_CALLBACK(maincanvashscroll), maincanvashscrollbar);

  // Two Boxes and one Pane Layout //

  gtk_box_pack_start(GTK_BOX(maincanvashbox), maincanvas, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(maincanvashbox), maincanvasvscrollbar, FALSE, FALSE, 0);

  gtk_box_set_child_packing(GTK_BOX(maincanvashbox), maincanvas, TRUE, TRUE, 0, GTK_PACK_START);
  gtk_box_set_child_packing(GTK_BOX(maincanvashbox), maincanvasvscrollbar, FALSE, FALSE, 0, GTK_PACK_END);

  gtk_box_pack_start(GTK_BOX(maincanvasvbox), maincanvashbox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(maincanvasvbox), maincanvashscrollbar, FALSE, FALSE, 0);

  gtk_box_set_child_packing(GTK_BOX(maincanvasvbox), maincanvashbox, TRUE, TRUE, 0, GTK_PACK_START);
  gtk_box_set_child_packing(GTK_BOX(maincanvasvbox), maincanvashscrollbar, FALSE, FALSE, 0, GTK_PACK_END);

  gtk_container_add(GTK_CONTAINER(mainframe), maincanvasvbox);

  g_signal_connect(G_OBJECT(maincanvas), "size-allocate", G_CALLBACK(resizemaincanvas), maincanvas);

  //gtk_container_add(GTK_CONTAINER(mainwindow), horizontal_box);
  // Add the horizontal box to the main window

  setup_horizontal_box();
  gtk_container_add(GTK_CONTAINER(mainframe), horizontal_box);

  hpane = gtk_hpaned_new();
  gtk_paned_add1(GTK_PANED(hpane), hierarchybrowserframe);
  gtk_paned_add2(GTK_PANED(hpane), mainframe);  

  gtk_container_add(GTK_CONTAINER(mainwindow), hpane);


  gtk_widget_show_all(mainwindow);

  gtk_main();

}

gint main (gint argc, gchar **argv)
{
  // you may call start_gui() from TCL, based on a TCL command //
  // alternatively, you may create a thread, but this is a lot more complex! //
  start_gui();

  return EXIT_SUCCESS;

}