# include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data){
    //gtk code comes here.
}

int main (int argc, char *argv[]) {
    
    GtkApplication *app;
    int ret;

    // Create a new application pointer and store it in a variable here `app` is used. 
    // The application is created with the gtk_application_new function, which takes two arguments: 
    // the application ID and the application flags. The application ID is a unique identifier for 
    // the application, and the application flags specify how the application should behave.
    

    app = gtk_application_new ("in.raeark", G_APPLICATION_DEFAULT_FLAGS);

    // G Signal Connect function is used to connect the "activate" signal of the application 
    // to the activate function.

    // G Callback is a macro that casts the activate function to the appropriate type for the signal handler.

    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    // G Application function is used to run the application. It takes three arguments: the application pointer, 
    // the number of command-line arguments, and the array of command-line arguments. The function
    // returns an integer value that indicates the exit status of the application. 

    // G Application Run function is used to start the main event loop of the application. It blocks until the 
    // application is closed, and returns an integer value that indicates the exit status of the application.

    ret = g_application_run (G_APPLICATION(app), argc, argv);

    // G Object Unref function is used to release the memory allocated for the application pointer.

    g_object_unref(app);

    return ret;
}