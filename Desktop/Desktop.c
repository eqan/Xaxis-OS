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

GtkWidget* GTK_Window;
GtkWidget* Main_Window;
GtkWidget* Back_Ground_Wall_Paper;
GtkWidget* Top_Bar;
GtkWidget* Linux_Button;
GtkWidget* Clock_Widget;
GtkWidget* Wifi_Widget;
GtkWidget* Dock;
GtkWidget* Hangman_Button;
GtkWidget* Graph_Button;
GtkWidget* NotePad_Button;
GtkBuilder* builder;
char *current_time;

char* exec(const char* cmd)
{
    FILE *fp;
    char result[1000];
    fp=popen(cmd, "r");
    fgets(result,sizeof(result),fp);
    char* result2 = result;
    return strdup(result2);
}

void update_time()
{
  current_time = malloc(1000);
  current_time = exec("date +%r");
  printf("%s",current_time);
  gtk_label_set_text(GTK_LABEL(Clock_Widget), current_time);
}

void cancel_clicked (GtkButton *button, gpointer user_data){
	gtk_main_quit();
	exit(0);
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);
	builder = gtk_builder_new_from_file ("Desktop.glade");
	GTK_Window = GTK_WIDGET(gtk_builder_get_object(builder, "GTK_Window"));
	g_signal_connect(GTK_Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder, NULL);

	Main_Window = GTK_WIDGET(gtk_builder_get_object(builder, "Main_Window"));
	Back_Ground_Wall_Paper = GTK_WIDGET(gtk_builder_get_object(builder, "Back_Ground_Wall_Paper"));
	Top_Bar = GTK_WIDGET(gtk_builder_get_object(builder, "Top_Bar"));
	Linux_Button = GTK_WIDGET(gtk_builder_get_object(builder, "Linux_Button"));
	Clock_Widget = GTK_WIDGET(gtk_builder_get_object(builder, "Clock_Widget"));
	Wifi_Widget = GTK_WIDGET(gtk_builder_get_object(builder, "Wifi_Widget"));
	Dock = GTK_WIDGET(gtk_builder_get_object(builder, "Dock"));
	Hangman_Button = GTK_WIDGET(gtk_builder_get_object(builder, "Hangman_Button"));
	Graph_Button = GTK_WIDGET(gtk_builder_get_object(builder, "Graph_Button"));
	NotePad_Button = GTK_WIDGET(gtk_builder_get_object(builder, "NotePad_Button"));
  update_time();
    //g_signal_connect(G_OBJECT(cancel), "clicked",G_CALLBACK(cancel_clicked),NULL);
	g_object_unref(builder);
	gtk_widget_show(GTK_Window);

	gtk_main();

	return EXIT_SUCCESS;
}
