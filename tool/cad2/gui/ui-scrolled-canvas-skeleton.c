#include "ui-scrolled-canvas-skeleton.h"

static void expose(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  //  #ifdef DEBUGMODE
    {
      printf("DEBUG: Canvas Expose\n");
    }
  //  #endif  

  maincanvaspaint(widget, event, data);
}

// Expose-Event Paint Function for maincanvas //
static void maincanvaspaint(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  maincanvas_drawable = maincanvas->window; // drawable drawing area window //
  //  maincanvas_drawable = GTK_LAYOUT(maincanvas)->bin_window; // drawable layout window //
  maincanvas_cs = gdk_cairo_create(maincanvas_drawable); // corresponding cairo state //

  // call main drawing function //

  cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0);
  cairo_paint(maincanvas_cs);

  // Set dotted line style
  double dashes[] = {4.0, 4.0}; // 4 pixels on, 4 pixels off
  cairo_set_dash(maincanvas_cs, dashes, 2, 0); // 2 is the number of dashes in the pattern

  cairo_set_source_rgb(maincanvas_cs, 0.0, 0.0, 1.0);
  cairo_rectangle(maincanvas_cs, 0, 0, (maincanvasWidth-5) * current_scale, (maincanvasHeight-5) * current_scale );
  // cairo_stroke(maincanvas_cs);
  cairo_stroke(maincanvas_cs);

  // Draw rectangles to form the word "CAD"
  cairo_set_source_rgb(maincanvas_cs, 0.005, 0.160, 0.049); // Dark blue

  // Letter 'C'
  cairo_rectangle(maincanvas_cs, (50 + maincanvasOx) * current_scale, 50 * current_scale, 30 * current_scale, 150 * current_scale);
  cairo_rectangle(maincanvas_cs, (50 + maincanvasOx) * current_scale, 50  * current_scale, 90 * current_scale, 30 * current_scale);
  cairo_rectangle(maincanvas_cs, (50 + maincanvasOx) * current_scale, 170 * current_scale, 90 * current_scale, 30 * current_scale);
  cairo_fill(maincanvas_cs);

  // Letter 'A'
  cairo_set_source_rgb(maincanvas_cs, 0.195, 0.044, 0.006); // red
  cairo_rectangle(maincanvas_cs, (140 + maincanvasOx) * current_scale, 50 * current_scale, 30 * current_scale, 150 * current_scale);
  cairo_rectangle(maincanvas_cs, (210 + maincanvasOx) * current_scale, 50 * current_scale, 30 * current_scale, 150 * current_scale);
  cairo_rectangle(maincanvas_cs, (140 + maincanvasOx) * current_scale, 50 * current_scale, 70 * current_scale, 30 * current_scale);
  cairo_rectangle(maincanvas_cs, (140 + maincanvasOx) * current_scale, 120 * current_scale, 70 * current_scale, 30 * current_scale);
  cairo_fill(maincanvas_cs);

  cairo_set_source_rgb(maincanvas_cs, 0.005, 0.160, 0.049); // Dark blue
  // Letter 'D'
  cairo_rectangle(maincanvas_cs, (260 + maincanvasOx) * current_scale, 50 * current_scale, 30 * current_scale, 150 * current_scale);
  cairo_rectangle(maincanvas_cs, (330 + maincanvasOx) * current_scale, 50 * current_scale, 30 * current_scale, 150 * current_scale);
  cairo_rectangle(maincanvas_cs, (240 + maincanvasOx) * current_scale, 50 * current_scale, 90 * current_scale, 30 * current_scale);
  cairo_rectangle(maincanvas_cs, (240 + maincanvasOx) * current_scale, 170 * current_scale, 90 * current_scale, 30 * current_scale);

  cairo_fill(maincanvas_cs);

  cairo_set_source_rgb(maincanvas_cs, 0.004, 0.101, 0.229); // Dark blue
  // Letter 'D'
  cairo_rectangle(maincanvas_cs, (410 + maincanvasOx) * current_scale, 50  * current_scale, 30 * current_scale, 150 * current_scale);
  cairo_rectangle(maincanvas_cs, (450 + maincanvasOx) * current_scale, 50  * current_scale, 30 * current_scale, 150 * current_scale);
  cairo_rectangle(maincanvas_cs, (390 + maincanvasOx) * current_scale, 50  * current_scale, 110 * current_scale, 30 * current_scale);
  cairo_rectangle(maincanvas_cs, (390 + maincanvasOx) * current_scale, 170 * current_scale, 110 * current_scale, 30 * current_scale);
  cairo_fill(maincanvas_cs);

  // Add text
  cairo_select_font_face(maincanvas_cs, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(maincanvas_cs, 15);
  cairo_set_source_rgb(maincanvas_cs, 0.004, 0.101, 0.229); // Black
  cairo_move_to(maincanvas_cs, 0, 545);
  cairo_show_text(maincanvas_cs, "Designer: Dimitrios Tsalapatas");
  cairo_fill(maincanvas_cs);

  cairo_destroy(maincanvas_cs);
}

// Size-Allocate Event Function for maincanvas // 
static void resizemaincanvas(GtkWidget *widget, GdkRectangle *gdkrect, gpointer data)
{
  int newmaincanvasWidth;
  int newmaincanvasHeight;
  
  //  #ifdef DEBUGMODE
    {
      printf("DEBUG: Resize Main Canvas.\n");
      printf("DEBUG: Current Width, Height = (%d, %d), New Width, Height = (%d, %d)\n", maincanvasWidth, maincanvasHeight, gdkrect->width, gdkrect->height);
    }

  //  #endif

  // resize code //

    maincanvasWidth = gdkrect->width;
    maincanvasHeight = gdkrect->height;

    maincanvasOx = maincanvasOx * round(((double) newmaincanvasWidth)/((double) maincanvasWidth));
    maincanvasOy = maincanvasOy * round(((double) newmaincanvasHeight)/((double) maincanvasHeight));

    maincanvasWidth = newmaincanvasWidth;
    maincanvasHeight = newmaincanvasHeight;

    gtk_widget_queue_draw(widget);

}

static gboolean maincanvasvscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
  double maxmaincanvasOy;
  int ivalue; // calibrated, inter y-offset value, based on vertical scrollbar value //

  //  #ifdef DEBUGMODE
    {
      printf("DEBUG: Vertical Scroll\n");
      printf("DEBUG: Scroll value = %.3f\n", value);
    }
  //  #endif  

  // vertical scrollbar code here //

}

static gboolean maincanvashscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{

  //  #ifdef DEBUGMODE
    {
      printf("DEBUG: Horizontal Scroll\n");      
      printf("DEBUG: Scroll value = %.3f\n", value);
      
    }
  //  #endif  

  // horizontal scrollbar code here //

    adjust_scrollbar = gtk_range_get_adjustment(range);

    // Get the current values of the adjustment
    double low = gtk_adjustment_get_lower(adjust_scrollbar);
    double up = gtk_adjustment_get_upper(adjust_scrollbar);
    double page_size = gtk_adjustment_get_page_size(adjust_scrollbar);

    gtk_adjustment_set_lower(adjust_scrollbar, low = -550.0);
    gtk_adjustment_set_upper(adjust_scrollbar, up = 1100.0);
    gtk_adjustment_set_page_size(adjust_scrollbar, page_size = 550.0);
    
    double maxmaincanvasOx = up;
    // int ivalue = (int)((value - low) / page_size); // calibrated, inter x-offset value, based on horizontal scrollbar value //
    int ivalue = (int) round(value);

    // if(scroll == GTK_SCROLL_STEP_BACKWARD)
    // {
    //     maincanvasOx = maincanvasOx - ivalue;
    // }
    // else if(scroll == GTK_SCROLL_STEP_FORWARD)
    // {
    //     maincanvasOx = maincanvasOx + ivalue;
    // }
    maincanvasOx = ivalue;

    if (maincanvasOx < -550)
    {
        maincanvasOx = low; // Limit minimum value
    }
    else if (maincanvasOx > maxmaincanvasOx)
    {
        maincanvasOx = maxmaincanvasOx; // Limit maximum value
    }

    gtk_adjustment_set_value(adjust_scrollbar, maincanvasOx);

    printf("maincanvasOx = %d\n", maincanvasOx);

    gtk_widget_queue_draw(maincanvas);

}

static void scroll(GtkWidget *widget, GdkEventScroll *eev, gpointer data)
{
    double zoom_factor = 1.05;
  //  #ifdef DEBUGMODE
    {
      printf("DEBUG: Canvas Mouse Scroll\n");
    }
  //  #endif

  // mouse wheel (scroll) code here //

  if(eev->direction == GDK_SCROLL_UP)
  {
    current_scale = current_scale * zoom_factor;
  } 
  else if(eev->direction == GDK_SCROLL_DOWN)
  {
    current_scale = current_scale / zoom_factor;
  }
  else
  {
    return;
  }

  gtk_widget_queue_draw(widget);
    
}

void setupscrolladjustments()
{
  // setup horizontal and vertical scrollbar adjustments //

  // use gtk_adjustment_configure() //

    // setup horizontal and vertical scrollbar adjustments //
    scaledhpagesize =  current_scale * ((double) maincanvasWidth);
    scaledvpagesize =  current_scale * ((double) maincanvasHeight);
    hpagesize = ((double) maincanvasWidth);
    vpagesize = ((double) maincanvasHeight);
    hstep = 0.05 * hpagesize; // 5% of maincanvasWidth horizontal step // 
    vstep = 0.05 * vpagesize; // 5% of maincanvasHeight vertical step //

    gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvasvscrollbaradjustment), maincanvasOy, 0.0, scaledvpagesize, vstep, vstep, vpagesize);
    gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvashscrollbaradjustment), maincanvasOx, 0.0, scaledhpagesize, hstep, hstep, hpagesize);
}

static void mousebutton(GtkWidget *widget, GdkEventButton *eev, gpointer data)
{
  //  #ifdef DEBUGMODE
    {
      printf("DEBUG: Mouse Button %d Pressed\n", eev->button);
    }
  //  #endif

  if (eev->button == 1) // Left Mouse Button //
    {
      // code here //
      current_scale = 1.0;
        gtk_widget_queue_draw(widget);
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
  gtk_window_set_default_size(GTK_WINDOW(mainwindow), 300, 300); // default size //
  
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