// #include <gtk-2.0/gtk/gtk.h>
// #include <gtk-2.0/gdk/gdkx.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
}

static void
activate (GApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Hello");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  gtk_container_add (GTK_CONTAINER (window), button);

  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  gtk_init(&argc, &argv);  // Initialize GTK+ toolkit

  GApplication *app;
  int status;

  app = g_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

