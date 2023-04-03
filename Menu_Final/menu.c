#include <stdio.h>
#include <gtk/gtk.h>
//#include "Interface.glade"
void on_start_button_clicked(GtkButton *button,gpointer user_data)
{
	const char *filename = "mapfinal";
	FILE *fp  = fopen(filename,"w");
	fclose;
	//g_print("le bouton a ete cliquer");
}


int main(int argc,char *argv[])
{
	GtkBuilder *builder;
	GtkWidget *window;
	//GtkWidget *start_button;


	gtk_init(&argc,&argv);

	builder = gtk_builder_new_from_file("Interface.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder,"main_window"));
	//start-button = GTK_WIDGET(gtk_builder_get_object(builder,"start_button"));

	gtk_builder_connect_signals(builder,NULL);

	//g_signal_connect(start_button,"clicked",G_CALLBACK(on_start_button_clicked),NULL)

	gtk_widget_show_all(window);


	gtk_main();

	return 0;
}
