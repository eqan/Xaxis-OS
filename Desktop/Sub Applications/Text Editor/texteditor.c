#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>

GtkWidget* window;
GtkWidget* title;
GtkWidget* fixed1;
GtkWidget* scrollwindow;
GtkWidget* viewport;
GtkWidget* textview;
GtkWidget* save;
GtkWidget* cancel;
GtkBuilder* builder;

G_MODULE_EXPORT void cancel_clicked (GtkButton *button, gpointer user_data){
	gtk_main_quit();
	exit(0);
}
int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);
	builder = gtk_builder_new_from_file ("texteditor.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
	scrollwindow = GTK_WIDGET(gtk_builder_get_object(builder, "scrollw"));
	viewport = GTK_WIDGET(gtk_builder_get_object(builder, "vport"));
	textview = GTK_WIDGET(gtk_builder_get_object(builder, "text"));
	save = GTK_WIDGET(gtk_builder_get_object(builder, "save"));
	//cancel = GTK_WIDGET(gtk_builder_get_object(builder, "cancel"));
	title = GTK_WIDGET(gtk_builder_get_object(builder, "title"));
    //g_signal_connect(G_OBJECT(cancel), "clicked",G_CALLBACK(cancel_clicked),NULL);
	g_object_unref(builder);
	gtk_widget_show(window);

	gtk_main();

	return EXIT_SUCCESS;
}
