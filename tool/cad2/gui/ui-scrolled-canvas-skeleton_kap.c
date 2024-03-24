#include "ui-scrolled-canvas-skeleton_kap.h"

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
  printf("zoom is: %lf\n", zoom_level);
  maincanvas_drawable = maincanvas->window; // drawable drawing area window //
  //  maincanvas_drawable = GTK_LAYOUT(maincanvas)->bin_window; // drawable layout window //
  maincanvas_cs = gdk_cairo_create(maincanvas_drawable); // corresponding cairo state //

  // call main drawing function //
  /*We have made two rectangles just that, should add more, not thatimportant now*/
  // Set line width
  cairo_set_line_width(maincanvas_cs, 2);

  // Set fill color (if you want filled rectangles)
  // cairo_set_source_rgb(maincanvas_cs, red, green, blue); // Replace red, green, blue with your color values

  // Draw rectangles
  // Rectangle coordinates: x, y, width, height
  /*Draw the window black*/
  cairo_set_source_rgb(maincanvas_cs, 0.0, 0.0, 0.0); // Black color
  cairo_rectangle(maincanvas_cs, 0, 0, maincanvasWidth, maincanvasHeight);
  cairo_fill(maincanvas_cs);

  /*Create a white frame around the black window*/
  cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // White color
  cairo_rectangle(maincanvas_cs, 0, 0, maincanvasWidth, maincanvasHeight);
  cairo_stroke(maincanvas_cs); // Draw outline only

  /*Create rectangles inside the window*/
  cairo_set_source_rgb(maincanvas_cs, 1.0, 0.0, 0.0); // Red color
  cairo_rectangle(maincanvas_cs, 10*zoom_level-maincanvasOx, 10*zoom_level-maincanvasOy, 100*zoom_level, 50*zoom_level);
  cairo_fill(maincanvas_cs);
  cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // White color
  cairo_rectangle(maincanvas_cs, 10*zoom_level-maincanvasOx, 10*zoom_level-maincanvasOy, 100*zoom_level, 50*zoom_level);
  cairo_stroke(maincanvas_cs);

  cairo_set_source_rgb(maincanvas_cs, 0.0, 1.0, 0.0); // Green color
  cairo_rectangle(maincanvas_cs, 200*zoom_level-maincanvasOx, 200*zoom_level-maincanvasOy, 50*zoom_level, 100*zoom_level);
  cairo_fill(maincanvas_cs);
  cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // White color
  cairo_rectangle(maincanvas_cs, 200*zoom_level-maincanvasOx, 200*zoom_level-maincanvasOy, 50*zoom_level, 100*zoom_level);
  cairo_stroke(maincanvas_cs);

  cairo_set_source_rgb(maincanvas_cs, 0.0, 0.0, 1.0); // Blue color
  cairo_rectangle(maincanvas_cs, 100*zoom_level-maincanvasOx, 100*zoom_level-maincanvasOy, 50*zoom_level, 100*zoom_level);
  cairo_fill(maincanvas_cs);
  cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // White color
  cairo_rectangle(maincanvas_cs, 100*zoom_level-maincanvasOx, 100*zoom_level-maincanvasOy, 50*zoom_level, 100*zoom_level);
  cairo_stroke(maincanvas_cs);

  cairo_set_source_rgb(maincanvas_cs, 0.0, 1.0, 1.0); //Light blue color
  cairo_rectangle(maincanvas_cs, 350*zoom_level-maincanvasOx, 15*zoom_level-maincanvasOy, 150*zoom_level, 40*zoom_level);
  cairo_fill(maincanvas_cs);
  cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // White color
  cairo_rectangle(maincanvas_cs, 350*zoom_level-maincanvasOx, 15*zoom_level-maincanvasOy, 150*zoom_level, 40*zoom_level);
  cairo_stroke(maincanvas_cs);

  cairo_set_source_rgb(maincanvas_cs, 1.0, 0.0, 1.0); //Magenta color
  cairo_rectangle(maincanvas_cs, 15*zoom_level-maincanvasOx, 400*zoom_level-maincanvasOy, 120*zoom_level, 60*zoom_level);
  cairo_fill(maincanvas_cs);
  cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // White color
  cairo_rectangle(maincanvas_cs, 15*zoom_level-maincanvasOx, 400*zoom_level-maincanvasOy, 120*zoom_level, 60*zoom_level);
  cairo_stroke(maincanvas_cs);

  //Apply cliping
  cairo_rectangle(maincanvas_cs, 0, 0, maincanvasWidth, maincanvasHeight);
  cairo_clip(maincanvas_cs);

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
  // Create a new GtkFixed container widget
  /*maincanvas_fixed = gtk_fixed_new();
  gtk_widget_set_size_request(maincanvas_fixed, newmaincanvasWidth, newmaincanvasHeight);
  printf("(%d, %d)\n", newmaincanvasWidth, newmaincanvasHeight);*/
  // Adjust the size request of maincanvas within the fixed layout container
  //gtk_widget_set_size_request(maincanvas, newmaincanvasWidth, newmaincanvasHeight);

  // Redraw the display when the window is resized
  gtk_widget_queue_draw(widget);
}

static gboolean maincanvasvscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
  // Get the adjustment of the horizontal scrollbar
  GtkAdjustment *adjustment = gtk_range_get_adjustment(range);
    
  // Get the current values of the adjustment
  double lower = gtk_adjustment_get_lower(adjustment);
  double upper = gtk_adjustment_get_upper(adjustment);
  double page_size = gtk_adjustment_get_page_size(adjustment);

  gtk_adjustment_set_lower(adjustment, lower = -550.0);
  gtk_adjustment_set_upper(adjustment, upper = 1100.0);
  gtk_adjustment_set_page_size(adjustment, page_size = 550.0);

  // Calculate the maximum x-offset value
  double maxmaincanvasOy = upper;

  // Calculate the calibrated x-offset value based on the scrollbar's value
  int ivalue = (int)((value - lower) / page_size);

  //  #ifdef DEBUGMODE
    {
      printf("DEBUG: Vertical Scroll\n");
      printf("DEBUG: Scroll value = %.3f\n", value);
    }
  //  #endif 

  // Print debug information
  //printf("DEBUG: Horizontal Scroll\n");
  //printf("DEBUG: Scroll value = %.3f\n", value);
  //printf("DEBUG: Calculated ivalue = %d\n", ivalue);

  // Update the x-offset based on the scroll direction
  switch (scroll)
  {
    // Scroll down
    case GTK_SCROLL_STEP_BACKWARD:
      maincanvasOy -= ivalue;
      printf("Scroll down\n");
      break;

    // Scroll up
    case GTK_SCROLL_STEP_FORWARD:
      maincanvasOy += ivalue;
      printf("Scroll up\n");
      break;

    default:
      break;
  }

  // Perform bounds checking to prevent maincanvasOx from going out of bounds
  if (maincanvasOy < -550)
  {
    maincanvasOy = lower; // Limit minimum value
  }
  else if (maincanvasOy > maxmaincanvasOy)
  {
    maincanvasOy = maxmaincanvasOy; // Limit maximum value
  }

  // Configure the adjustment with the updated x-offset
  gtk_adjustment_set_value(adjustment, maincanvasOy);

  // Update the canvas display
  gtk_widget_queue_draw(maincanvas);
  
}

static gboolean maincanvashscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
  // Get the adjustment of the horizontal scrollbar
  GtkAdjustment *adjustment = gtk_range_get_adjustment(range);
    
  // Get the current values of the adjustment
  double lower = gtk_adjustment_get_lower(adjustment);
  double upper = gtk_adjustment_get_upper(adjustment);
  double page_size = gtk_adjustment_get_page_size(adjustment);

  gtk_adjustment_set_lower(adjustment, lower = -550.0);
  gtk_adjustment_set_upper(adjustment, upper = 1100.0);
  gtk_adjustment_set_page_size(adjustment, page_size = 550.0);

  // Calculate the maximum x-offset value
  double maxmaincanvasOx = upper;

  // Calculate the calibrated x-offset value based on the scrollbar's value
  int ivalue = (int)((value - lower) / page_size);

  // Print debug information
  printf("DEBUG: Horizontal Scroll\n");
  printf("DEBUG: Scroll value = %.3f\n", value);
  printf("DEBUG: Calculated ivalue = %d\n", ivalue);

  // Update the x-offset based on the scroll direction
  switch (scroll)
  {
    // Scroll to the left
    case GTK_SCROLL_STEP_BACKWARD:
      maincanvasOx -= ivalue;
      printf("Scroll to the left\n");
      break;

    // Scroll to the right
    case GTK_SCROLL_STEP_FORWARD:
      maincanvasOx += ivalue;
      printf("Scroll to the right\n");
      break;

    default:
      break;
  }

  // Perform bounds checking to prevent maincanvasOx from going out of bounds
  if (maincanvasOx < -550)
  {
    maincanvasOx = lower; // Limit minimum value
  }
  else if (maincanvasOx > maxmaincanvasOx)
  {
    maincanvasOx = maxmaincanvasOx; // Limit maximum value
  }

  // Configure the adjustment with the updated x-offset
  gtk_adjustment_set_value(adjustment, maincanvasOx);

  // Update the canvas display
  gtk_widget_queue_draw(maincanvas);
}




/*static gboolean maincanvashscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
  double maxmaincanvasOx;
  int ivalue; // calibrated, inter x-offset value, based on horizontal scrollbar value //

  // Get the range of the horizontal scrollbar
  GtkAdjustment *adjustment = gtk_range_get_adjustment(GTK_RANGE(range));
  double lower = gtk_adjustment_get_lower(adjustment);
  double upper = gtk_adjustment_get_upper(adjustment);
  double page_size = gtk_adjustment_get_page_size(adjustment);
  //printf("pg_sz: %lf\n", page_size);
  // Assign the maximum x-offset value
  maxmaincanvasOx = upper;

  // Calculate the calibrated x-offset value based on the scrollbar's value
  ivalue = (int)((value - lower) / page_size);
  printf("value = %lf, lower = %lf, ivalue = %d\n", value, lower, ivalue);

  //  #ifdef DEBUGMODE
    {
      printf("DEBUG: Horizontal Scroll\n");      
      printf("DEBUG: Scroll value = %.3f\n", value);
    }
  //  #endif  

  // horizontal scrollbar code here //
  switch(scroll)
  {
    //Scroll to the left
    case GTK_SCROLL_STEP_BACKWARD:
      maincanvasOx -= 1;
      //printf("lower = %lf, upper = %lf\n", lower, upper);
      gtk_adjustment_configure(adjustment, maincanvasOx, lower, upper, 1, 0, 10);
      gtk_widget_queue_draw(maincanvas);
      printf("Scroll to the left\n");
      break;
    
    //Scroll to the right
    case GTK_SCROLL_STEP_FORWARD:
      maincanvasOx += 1;
      gtk_adjustment_configure(adjustment, maincanvasOx, lower, upper, 1, 0, 10);
      gtk_widget_queue_draw(maincanvas);
      printf("Scroll to the right\n");
      break;
    
    default:
      break;
  }*/

  // Perform bounds checking to prevent maincanvasOx from going out of bounds
  /*if (maincanvasOx < 0)
  {
    maincanvasOx = 0; // Limit minimum value
  }
  else if (maincanvasOx > maxmaincanvasOx)
  {
    maincanvasOx = maxmaincanvasOx; // Limit maximum value
  }*/
/*
}*/

//Seems ok let's get it
static void scroll(GtkWidget *widget, GdkEventScroll *eev, gpointer data)
{
  //zoom factor helps us zoom-in or zoom-out times or divisions based on this number
  double zoom_factor = 1.05;

  //  #ifdef DEBUGMODE
    {
      printf("DEBUG: Canvas Mouse Scroll\n");
    }
  //  #endif

  // mouse wheel (scroll) code here //
  if(eev->direction == GDK_SCROLL_UP)
  {
    (zoom_level) = (zoom_level)*zoom_factor;
    printf("zoom-in: %lf\n", zoom_level);
    gtk_widget_queue_draw(widget);
  }
  else if(eev->direction == GDK_SCROLL_DOWN)
  {
    (zoom_level) = (zoom_level)/zoom_factor;
    printf("zoom-out: %lf\n", zoom_level);
    gtk_widget_queue_draw(widget);
  }
}

void setupscrolladjustments()
{
  // setup horizontal and vertical scrollbar adjustments //

  // use gtk_adjustment_configure() //
  //gtk_adjustment_configure(0, 0, 0, 0, maincanvasOx, 0, 0);
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
      zoom_level = 1.0;
      printf("zoom back to the original level: %lf\n", zoom_level);
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