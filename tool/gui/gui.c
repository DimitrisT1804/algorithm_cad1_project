#include "gui.h"

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

int maincanvasWidth = 800; // main canvas visible dimension - width //
int maincanvasHeight = 650; // main canvas visible dimension - height //
int maincanvasOx = 0; // main canvas visible origin translation - x offset //
int maincanvasOy = 0; // main canvas visible origin translation - y offset //

GtkWidget *mainwindow; // your main window //

double current_scale = 1.0; // zoom factor for main canvas //

GtkAdjustment *adjust_scrollbar;

GtkWidget *buttons; // buttons container //
GtkWidget *button1; // button 1 //
GtkWidget *hierarchybrowserframe;
GtkWidget *list_cells_button; // button 1 //

double offset_x = 1.0;
double offset_y = 1.0;


double max_double(double a, double b)
{
    if (a > b) 
    {
        return a;
    } 
    else 
    {
        return b;
    }
}

static void expose(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    #ifdef DEBUGGUI
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
    double c1_x = 0.0;
    double c1_y = 0.0;

    cairo_set_source_rgb(maincanvas_cs, 146.0/255.0, 185.0/255.0, 189.0/255.0);
    cairo_paint(maincanvas_cs);

    // Set dotted line style
    double dashes[] = {4.0, 4.0}; // 4 pixels on, 4 pixels off
    cairo_set_dash(maincanvas_cs, dashes, 2, 0); // 2 is the number of dashes in the pattern

    cairo_set_source_rgb(maincanvas_cs, 0.0, 0.0, 1.0);
    cairo_rectangle(maincanvas_cs, 0, 0, (maincanvasWidth - 100) * current_scale, (maincanvasHeight - 100) * current_scale );
    // cairo_stroke(maincanvas_cs);
    cairo_stroke(maincanvas_cs);

    // letter C //
    // Draw rectangles to form the word "CAD"
    cairo_set_source_rgb(maincanvas_cs, 168.0/255.0, 212.0/255.0, 173.0/255.0); // light blue
    cairo_rectangle(maincanvas_cs, (50 + maincanvasOx - offset_x) * current_scale + offset_x, (200 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 150 * current_scale);
    cairo_rectangle(maincanvas_cs, (50 + maincanvasOx - offset_x) * current_scale + offset_x, (120 + maincanvasOy - offset_y)  * current_scale + offset_y, 150 * current_scale, 60 * current_scale);
    cairo_rectangle(maincanvas_cs, (50 + maincanvasOx - offset_x) * current_scale + offset_x, (370 + maincanvasOy - offset_y)  * current_scale + offset_y, 150 * current_scale, 60 * current_scale);

    cairo_fill(maincanvas_cs);

    // Set the color for the outline (white)
    cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // White color
    cairo_set_line_width(maincanvas_cs, 2.0); // Adjust the width as needed
    // Set line style to solid
    cairo_set_dash(maincanvas_cs, 0, 0, 0); // Set dash pattern to solid
    // Draw the outline rectangle
    cairo_rectangle(maincanvas_cs, (50 + maincanvasOx - offset_x) * current_scale + offset_x, (200 + maincanvasOy - offset_y) * current_scale + offset_y, 60 * current_scale, 150 * current_scale);
    cairo_rectangle(maincanvas_cs, (50 + maincanvasOx - offset_x) * current_scale + offset_x, (120 + maincanvasOy - offset_y)  * current_scale + offset_y, 150 * current_scale, 60 * current_scale);
    cairo_rectangle(maincanvas_cs, (50 + maincanvasOx - offset_x) * current_scale + offset_x, (370 + maincanvasOy - offset_y)  * current_scale + offset_y, 150 * current_scale, 60 * current_scale);
    cairo_stroke(maincanvas_cs); // Draw the outline

    cairo_select_font_face(maincanvas_cs, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_source_rgb(maincanvas_cs, 1.0, 0.0, 0.0); // Set text color to black
    cairo_set_font_size(maincanvas_cs, 15.0); // Set font size


    // letter A //
    cairo_set_source_rgb(maincanvas_cs, 155.0/255.0, 106.0/255.0, 108.0/255.0); // light blue
    cairo_rectangle(maincanvas_cs, (250 + maincanvasOx - offset_x) * current_scale + offset_x, (140 + maincanvasOy - offset_y) * current_scale + offset_y, 60 * current_scale, 140 * current_scale);
    cairo_rectangle(maincanvas_cs, (250 + maincanvasOx - offset_x) * current_scale + offset_x, (300 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 140 * current_scale);
    cairo_rectangle(maincanvas_cs, (400 + maincanvasOx - offset_x) * current_scale + offset_x, (140 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 140 * current_scale);
    cairo_rectangle(maincanvas_cs, (400 + maincanvasOx - offset_x) * current_scale + offset_x, (300 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 140 * current_scale);
    cairo_rectangle(maincanvas_cs, (280 + maincanvasOx - offset_x) * current_scale + offset_x, (90 + maincanvasOy - offset_y)  * current_scale + offset_y, 150 * current_scale, 40 * current_scale);
    cairo_rectangle(maincanvas_cs, (315 + maincanvasOx - offset_x) * current_scale + offset_x, (270 + maincanvasOy - offset_y)  * current_scale + offset_y, 80 * current_scale, 30 * current_scale);
    cairo_fill(maincanvas_cs);

    // Set the color for the outline (white)
    cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // White color
    cairo_set_line_width(maincanvas_cs, 2.0); // Adjust the width as needed
    // Set line style to solid
    cairo_set_dash(maincanvas_cs, 0, 0, 0); // Set dash pattern to solid
    // Draw the outline rectangle
    cairo_rectangle(maincanvas_cs, (250 + maincanvasOx - offset_x) * current_scale + offset_x, (140 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 140 * current_scale);
    cairo_rectangle(maincanvas_cs, (250 + maincanvasOx - offset_x) * current_scale + offset_x, (300 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 140 * current_scale);
    cairo_rectangle(maincanvas_cs, (400 + maincanvasOx - offset_x) * current_scale + offset_x, (140 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 140 * current_scale);
    cairo_rectangle(maincanvas_cs, (400 + maincanvasOx - offset_x) * current_scale + offset_x, (300 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 140 * current_scale);
    cairo_rectangle(maincanvas_cs, (280 + maincanvasOx - offset_x) * current_scale + offset_x, (90 + maincanvasOy - offset_y) * current_scale + offset_y, 150 * current_scale, 40 * current_scale);
    cairo_rectangle(maincanvas_cs, (315 + maincanvasOx - offset_x) * current_scale + offset_x, (270 + maincanvasOy - offset_y)  * current_scale + offset_y, 80 * current_scale, 30 * current_scale);
    cairo_stroke(maincanvas_cs); // Draw the outline

    // letter D //
    cairo_set_source_rgb(maincanvas_cs, 181.0/255.0, 148.0/255.0, 182.0/255.0); // light blue
    cairo_rectangle(maincanvas_cs, (520 + maincanvasOx - offset_x) * current_scale + offset_x, (150 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 110 * current_scale);
    cairo_rectangle(maincanvas_cs, (520 + maincanvasOx - offset_x) * current_scale + offset_x, (280 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 110 * current_scale);
    cairo_rectangle(maincanvas_cs, (500 + maincanvasOx - offset_x) * current_scale + offset_x, (90 + maincanvasOy - offset_y)  * current_scale + offset_y, 200 * current_scale, 40 * current_scale);
    cairo_rectangle(maincanvas_cs, (500 + maincanvasOx - offset_x) * current_scale + offset_x, (410 + maincanvasOy - offset_y)  * current_scale + offset_y, 200 * current_scale, 40 * current_scale);
    cairo_rectangle(maincanvas_cs, (640 + maincanvasOx - offset_x) * current_scale + offset_x, (150 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 110 * current_scale);
    cairo_rectangle(maincanvas_cs, (640 + maincanvasOx - offset_x) * current_scale + offset_x, (280 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 110 * current_scale);

    cairo_fill(maincanvas_cs);

    // Set the color for the outline (white)
    cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // White color
    cairo_set_line_width(maincanvas_cs, 2.0); // Adjust the width as needed
    // Set line style to solid
    cairo_set_dash(maincanvas_cs, 0, 0, 0); // Set dash pattern to solid
    
    cairo_rectangle(maincanvas_cs, (520 + maincanvasOx - offset_x) * current_scale + offset_x, (150 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 110 * current_scale);
    cairo_rectangle(maincanvas_cs, (520 + maincanvasOx - offset_x) * current_scale + offset_x, (280 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 110 * current_scale);
    cairo_rectangle(maincanvas_cs, (500 + maincanvasOx - offset_x) * current_scale + offset_x, (90 + maincanvasOy - offset_y)  * current_scale + offset_y, 200 * current_scale, 40 * current_scale);
    cairo_rectangle(maincanvas_cs, (500 + maincanvasOx - offset_x) * current_scale + offset_x, (410 + maincanvasOy - offset_y)  * current_scale + offset_y, 200 * current_scale, 40 * current_scale);
    cairo_rectangle(maincanvas_cs, (640 + maincanvasOx - offset_x) * current_scale + offset_x, (150 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 110 * current_scale);
    cairo_rectangle(maincanvas_cs, (640 + maincanvasOx - offset_x) * current_scale + offset_x, (280 + maincanvasOy - offset_y)  * current_scale + offset_y, 60 * current_scale, 110 * current_scale);

    cairo_stroke(maincanvas_cs); // Draw the outline //


    // Wires //
    cairo_set_source_rgb(maincanvas_cs, 227.0/255.0, 151.0/255.0, 116.0/255.0); // Black
    cairo_set_line_width(maincanvas_cs, 7.0);
    cairo_move_to(maincanvas_cs, (80 + maincanvasOx - offset_x) * current_scale + offset_x, (380 + maincanvasOy - offset_y)  * current_scale + offset_y);
    cairo_line_to(maincanvas_cs, (260 + maincanvasOx - offset_x) * current_scale + offset_x, (330 + maincanvasOy - offset_y)  * current_scale + offset_y);
    cairo_stroke(maincanvas_cs);

    cairo_set_line_width(maincanvas_cs, 9.0);
    cairo_move_to(maincanvas_cs, (430 + maincanvasOx - offset_x) * current_scale + offset_x, (200 + maincanvasOy - offset_y)  * current_scale + offset_y);
    cairo_line_to(maincanvas_cs, (540 + maincanvasOx - offset_x) * current_scale + offset_x, (110 + maincanvasOy - offset_y)  * current_scale + offset_y);
    cairo_stroke(maincanvas_cs);

    cairo_set_line_width(maincanvas_cs, 11.0);
    cairo_move_to(maincanvas_cs, (430 + maincanvasOx - offset_x) * current_scale + offset_x, (400 + maincanvasOy - offset_y)  * current_scale + offset_y);
    cairo_line_to(maincanvas_cs, (540 + maincanvasOx - offset_x) * current_scale + offset_x, (350 + maincanvasOy - offset_y)  * current_scale + offset_y);
    cairo_stroke(maincanvas_cs);

    // Label block //
    cairo_set_source_rgb(maincanvas_cs, 0.0, 0.153, 0.0); // light blue
    cairo_rectangle(maincanvas_cs, (100 + maincanvasOx - offset_x) * current_scale + offset_x, (500 + maincanvasOy - offset_y)  * current_scale + offset_y, 500 * current_scale, 40 * current_scale);
    cairo_fill(maincanvas_cs);

    cairo_select_font_face(maincanvas_cs, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // Set text color to black
    cairo_set_font_size(maincanvas_cs, 20.0); // Set font size
    cairo_move_to(maincanvas_cs, (130 + maincanvasOx - offset_x) * current_scale + offset_x, (520 + maincanvasOy - offset_y) * current_scale + offset_y); // Adjust position
    cairo_show_text(maincanvas_cs, "CAD 2: Physical Design Automation Tool");


    // Add text
    // cairo_select_font_face(maincanvas_cs, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    // cairo_set_font_size(maincanvas_cs, 15);
    // cairo_set_source_rgb(maincanvas_cs, 1.0, 1.0, 1.0); // Black
    // cairo_move_to(maincanvas_cs, 0, 645);
    // cairo_show_text(maincanvas_cs, "Designer: Dimitrios Tsalapatas");
    // cairo_fill(maincanvas_cs);

    cairo_destroy(maincanvas_cs);
}

// Size-Allocate Event Function for maincanvas // 
static void resizemaincanvas(GtkWidget *widget, GdkRectangle *gdkrect, gpointer data)
{
    int newmaincanvasWidth = 0;
    int newmaincanvasHeight = 0;
  
    #ifdef DEBUGGUI
    {
      printf("DEBUG: Resize Main Canvas.\n");
      printf("DEBUG: Current Width, Height = (%d, %d), New Width, Height = (%d, %d)\n", maincanvasWidth, maincanvasHeight, gdkrect->width, gdkrect->height);
    }
    #endif

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

    #ifdef DEBUGGUI
    {
      printf("DEBUG: Vertical Scroll\n");
      printf("DEBUG: Scroll value = %.3f\n", value);
    }
    #endif  

  // vertical scrollbar code here //

    adjust_scrollbar = gtk_range_get_adjustment(range);

    // Get the current values of the adjustment
    double low = gtk_adjustment_get_lower(adjust_scrollbar);
    double up = gtk_adjustment_get_upper(adjust_scrollbar);
    double page_size = gtk_adjustment_get_page_size(adjust_scrollbar);

    gtk_adjustment_set_lower(adjust_scrollbar, low = -550.0);
    gtk_adjustment_set_upper(adjust_scrollbar, up = 1100.0);
    gtk_adjustment_set_page_size(adjust_scrollbar, page_size = 550.0);
    
    maxmaincanvasOy = up;
    ivalue = (int) round(value);

    maincanvasOy = ivalue;

    if (maincanvasOy < -550)
    {
        maincanvasOy = low; // Limit minimum value
    }
    else if (maincanvasOy > maxmaincanvasOy)
    {
        maincanvasOy = maxmaincanvasOy; // Limit maximum value
    }

    gtk_adjustment_set_value(adjust_scrollbar, maincanvasOy);

    #ifdef DEBUGGUI
    {
        printf("maincanvasOy = %d\n", maincanvasOy);
    }
    #endif

    gtk_widget_queue_draw(maincanvas);

}

static gboolean maincanvashscroll(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer data)
{
    double low;
    double up;
    double page_size;
    double maxmaincanvasOx;
    int ivalue;

    #ifdef DEBUGGUI
    {
      printf("DEBUG: Horizontal Scroll\n");      
      printf("DEBUG: Scroll value = %.3f\n", value);
    }
    #endif  

  // horizontal scrollbar code here //

    adjust_scrollbar = gtk_range_get_adjustment(range);

    // Get the current values of the adjustment
    low = gtk_adjustment_get_lower(adjust_scrollbar);
    up = gtk_adjustment_get_upper(adjust_scrollbar);
    page_size = gtk_adjustment_get_page_size(adjust_scrollbar);

    gtk_adjustment_set_lower(adjust_scrollbar, low = -550.0);
    gtk_adjustment_set_upper(adjust_scrollbar, up = 1100.0);
    gtk_adjustment_set_page_size(adjust_scrollbar, page_size = 550.0);
    
    maxmaincanvasOx = up;
    ivalue = (int) round(value);

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

    #ifdef DEBUGGUI
    {
        printf("maincanvasOx = %d\n", maincanvasOx);
    }
    #endif

    gtk_widget_queue_draw(maincanvas);

}

static void scroll(GtkWidget *widget, GdkEventScroll *eev, gpointer data)
{
    double zoom_factor = 1.05;
    #ifdef DEBUGGUI
    {
        printf("DEBUG: Canvas Mouse Scroll\n");
    }
    #endif

    // Get the coordinates of the mouse pointer relative to the widget
    double x, y;
    gdk_event_get_coords((GdkEvent*)eev, &x, &y);

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

    // Calculate the offset of the mouse pointer relative to the canvas/widget
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);

    double center_x = allocation.width / 2;
    double center_y = allocation.height / 2;

    offset_x = x;
    offset_y = y;

    gtk_widget_queue_draw(widget);
}

void setupscrolladjustments()
{
    // setup horizontal and vertical scrollbar adjustments //
    scaledhpagesize =  current_scale * ((double) maincanvasWidth);
    scaledvpagesize =  current_scale * ((double) maincanvasHeight);
    hpagesize = ((double) maincanvasWidth);
    vpagesize = ((double) maincanvasHeight);
    hstep = 0.05 * hpagesize;
    vstep = 0.05 * vpagesize;

    gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvasvscrollbaradjustment), maincanvasOy, 0.0, scaledvpagesize, vstep, vstep, vpagesize);
    gtk_adjustment_configure(GTK_ADJUSTMENT(maincanvashscrollbaradjustment), maincanvasOx, 0.0, scaledhpagesize, hstep, hstep, hpagesize);
}

static void mousebutton(GtkWidget *widget, GdkEventButton *eev, gpointer data)
{
    #ifdef DEBUGGUI
    {
      printf("DEBUG: Mouse Button %d Pressed\n", eev->button);
    }
    #endif

    if (eev->button == 1) // Left Mouse Button //
    {
        // code here //

    }

    if(eev->button == 2) // Middle Mouse Button //
    {
        // code here //
        current_scale = 1.0;
        offset_x = 1.0;
        offset_y = 1.0;
        gtk_widget_queue_draw(widget);
    }

    if (eev->button == 3) // Right Mouse Button //
    {
        // code here //
    }
  
}

static void quitaction()
{
    #ifdef DEBUGGUI
    {
      printf("DEBUG: Quit Action\n");
    }
    #endif
  
//   printf("Thank you. Bye now.\n");
    gtk_main_quit();
}

static void button1_clicked(GtkButton *button, gpointer data)
{
    #ifdef DEBUGGUI
    {
      printf("DEBUG: Flow Button Clicked\n");
    }
    #endif

  // code here //

    Tcl_Eval(interp, "get_toposort");

}

static void list_cells_button_clicked(GtkButton *button, gpointer data)
{
    #ifdef DEBUGGUI
    {
      printf("DEBUG: Flow Button Clicked\n");
    }
    #endif

  // code here //

    Tcl_Eval(interp, "list_cells");

}

static void create_buttons_frame()
{
    buttons = gtk_vbox_new(FALSE, 0);

    gtk_container_add(GTK_CONTAINER(hierarchybrowserframe), buttons);

    button1 = gtk_button_new_with_label("get_toposort");
    list_cells_button = gtk_button_new_with_label("list_cells");

    g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(button1_clicked), NULL);
    g_signal_connect(G_OBJECT(list_cells_button), "clicked", G_CALLBACK(list_cells_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(buttons), button1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(buttons), list_cells_button, FALSE, FALSE, 0);
}


void start_gui()
{  
    // *** Local Variables *** //

    GtkWidget *maincanvashbox; // contains maincanvas and vertical scrollbar //
    GtkWidget *maincanvasvbox; // contains maincanvashbox and horizontal scrollbar //

    GtkWidget *hpane; // horizontal pane //
    GtkWidget *hpane2; // horizontal pane //

    GtkWidget *mainframe; // main canvas frame //

    GtkWidget *menubar;
    GtkWidget *filemenu;
    GtkWidget *file;



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

    create_buttons_frame();

    // // Create the menu
    // GtkWidget *menu_bar = create_menu();

    // // Add the menu bar to the main window
    // gtk_container_add(GTK_CONTAINER(mainwindow), menu_bar);

    gtk_widget_show_all(mainwindow);

    gtk_main();

}