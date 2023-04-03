#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "../algo/algo.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <gtk/gtk.h>
#include "../algo/dijkstra.h"
#include "../structs/graph.h"
#include "../structs/queue.h"
//#include "Interface.glade" //test
//
char *x=NULL;
char *y=NULL;
void on_start_button_clicked(GtkWidget *button,gpointer user_data)
{
	//TEST ENTRY
	GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(user_data,"gtk_entry1")); //test entry
	GtkEntry *entry2 = GTK_ENTRY(gtk_builder_get_object(user_data,"gtk_entry2"));
	const char *entry_text = gtk_entry_get_text(entry); //test entry 
	const char *entry_text2 = gtk_entry_get_text(entry2);
	x = g_strdup(entry_text);
	y = g_strdup(entry_text2);
	g_print("La valeur x est : %s\n",x);
	g_print("La valeur y  est : %s\n",y);


	int  x_value;
	int  y_value;
	size_t size_x;
	size_t size_y;
	x_value = x-'0';
	y_value = y-'0';

	size_x = (size_t) x_value;
	size_y = (size_t) y_value;

	size_t total_length =0;
	size_t *path = groupeFunction("../maps/1.txt", size_x, size_y, &total_length);

	g_print("La distance  est : %zu\n",path);
	free(path);



	//const char *filename = "mapfinal";
	//FILE *fp  = fopen(filename,"w");
	//fclose;
	//g_print("le bouton a ete cliquer");
}


int main(int argc,char *argv[])
{
	GtkBuilder *builder;
	GtkWidget *window;
	GtkWidget *start_button; //test

	gtk_init(&argc,&argv);

	builder = gtk_builder_new_from_file("Interface.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder,"main_window"));
	start_button = GTK_WIDGET(gtk_builder_get_object(builder,"start_button")); //test

	//gtk_builder_connect_signals(builder,NULL); //pas test
	//

	g_signal_connect(start_button,"clicked",G_CALLBACK(on_start_button_clicked),builder);//test

	gtk_widget_show_all(window);


	gtk_main();

	return 0;
}
