# include <gtk/gtk.h>

int main (int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window;

    // gtk window new needs an arg, it can be toplevel or popup window.

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "My Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // gtk_main_quit is used to terminate the GTK main loop when the window is closed.

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}