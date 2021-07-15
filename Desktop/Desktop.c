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
GtkWidget* Battery_Widget;
GtkWidget* Dock;
GtkWidget* Hangman_Picture;
GtkWidget* Graph_Picture;
GtkWidget* NotePad_Picture;
GtkWidget* Terminal_Picture;
GtkWidget* Hangman_Button;
GtkWidget* Graph_Button;
GtkWidget* NotePad_Button;
GtkWidget* Terminal_Button;
GtkBuilder* builder;
char *current_time;
char *current_battery;

char* exec(const char* cmd)
{
    FILE *fp;
    char result[1000];
    fp=popen(cmd, "r");
    fgets(result,sizeof(result),fp);
    pclose(fp);
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

char* filter_integers_from_string(char *ptr)
{
    char result[3];
    int i=0;
    while (*ptr)
    {
        if (isdigit(*ptr))
        {
            result[i] = *ptr;
            i++;
        }
        else
            ptr++;
    }
    char* result2 = result;
    return strdup(result2);
}

void set_battery_image(char* icon_name)
{
     gtk_image_set_from_file(GTK_IMAGE(Battery_Widget), icon_name);
}

void update_battery()
{
    current_battery = malloc(3);
    current_battery = exec("upower -i /org/freedesktop/UPower/devices/battery_BAT0| grep -E 'percentage'");
    printf("%s",current_battery);
    current_battery = filter_integers_from_string(current_battery);
    printf("%s",current_battery);
}
void hangman_pressed (GtkButton *button, gpointer user_data)
{
	exec("cd ~/Desktop/Xaxis-OS/Desktop/Sub-Applications/Hangman-In-Shell/ && ./game.sh && cd -");
}

void graph_pressed (GtkButton *button, gpointer user_data)
{
	exec("alacritty -e 'cd ~/Desktop/Xaxis-OS/Desktop/Sub-Applications/Graphs/ && ./Graph.o'");
}

void notepad_pressed (GtkButton *button, gpointer user_data)
{
	exec("cd ~/Desktop/Xaxis-OS/Desktop/Sub-Applications/Text-Editor/ && ./texteditor");
}

void terminal_pressed (GtkButton *button, gpointer user_data)
{
	exec("alacritty");
}

int main(int argc, char *argv[])
{
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
	Battery_Widget = GTK_WIDGET(gtk_builder_get_object(builder, "Battery_Widget"));
	Dock = GTK_WIDGET(gtk_builder_get_object(builder, "Dock"));
	Hangman_Picture = GTK_WIDGET(gtk_builder_get_object(builder, "Hangman_Picture"));
	Graph_Picture = GTK_WIDGET(gtk_builder_get_object(builder, "Graph_Picture"));
	NotePad_Picture = GTK_WIDGET(gtk_builder_get_object(builder, "NotePad_Picture"));
	Terminal_Picture = GTK_WIDGET(gtk_builder_get_object(builder, "Terminal_Picture"));
	Hangman_Button = GTK_WIDGET(gtk_builder_get_object(builder, "Hangman_Button"));
	Graph_Button = GTK_WIDGET(gtk_builder_get_object(builder, "Graph_Button"));
	NotePad_Button = GTK_WIDGET(gtk_builder_get_object(builder, "NotePad_Button"));
	Terminal_Button = GTK_WIDGET(gtk_builder_get_object(builder, "Terminal_Button"));
  update_time();
  g_signal_connect(G_OBJECT(Hangman_Button), "clicked",G_CALLBACK(hangman_pressed),NULL);
  g_signal_connect(G_OBJECT(Graph_Button), "clicked",G_CALLBACK(graph_pressed),NULL);
  g_signal_connect(G_OBJECT(NotePad_Button), "clicked",G_CALLBACK(notepad_pressed),NULL);
  g_signal_connect(G_OBJECT(Terminal_Button), "clicked",G_CALLBACK(terminal_pressed),NULL);
	g_object_unref(builder);
	gtk_widget_show(GTK_Window);

	gtk_main();

	return EXIT_SUCCESS;
}
