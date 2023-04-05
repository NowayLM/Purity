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
//#include "../algo/dijkstra.c"
#include "../structs/graph.h"
#include "../structs/queue.h"
//#include "Interface.glade" //test
//
char *x=NULL;
char *y=NULL;
char txt_buffer;
void on_start_button_clicked(GtkWidget *button,gpointer user_data)
{
	(void)button;
	//TEST ENTRY
	// test Text buffer
	//GtkTextView *text_view  = GTK_TEXT_VIEW(user_data);// Textview
	//GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);

	// ---------------------------
	GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(user_data,"gtk_entry1")); //test entry
	GtkEntry *entry2 = GTK_ENTRY(gtk_builder_get_object(user_data,"gtk_entry2"));
	const char *entry_text = gtk_entry_get_text(entry); //test entry 
	const char *entry_text2 = gtk_entry_get_text(entry2);
	x = g_strdup(entry_text);
	y = g_strdup(entry_text2);
	g_print("\nLa valeur de depart est : %s\n",x);
	g_print("\nLa valeur d'arrivee est : %s\n",y);
	//txt_buffer+=("\nLa valeur de depart est : %s\n",x);
	//txt_buffer+=("\nLa valeur de d'arrivee  est : %s\n",y);
	// -----------------------
	//gtk_text_buffer_insert_at_cursor(buffer,"hello",-1); //TextView

	int  x_value;
	int  y_value;
	size_t size_x;
	size_t size_y;
	x_value = atoi(x);
	y_value = atoi(y);

	size_x = (size_t) x_value;
	size_y = (size_t) y_value;

	size_t final_length;
	size_t total_length =0;
	struct graph *G = buildGraph("../maps/1.txt");
	//size_t *path = groupeFunction("../maps/1.txt", size_x, size_y, &total_length);
	size_t *path = dijkstra(G, size_x, size_y, &total_length, 1); 
	final_length = compute_path_length(total_length, path, G);
	g_print("\nOn calcule le chemin entre %zu et %zu \n", size_x, size_y);
	g_print("\nLa distance  est : %zu\n",final_length);
	g_print("\nLe chemin est le suivant : ");
	for(size_t i =0; i<total_length-1;i++)
	{
		g_print("%zu -> ",path[i] );
	}
	g_print("%zu\n", path[total_length -1]);
	free(path);
	freeGraph(G);



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
	//GtkWidget *text_view;
	//GtkTextBuffer *buffer;

	gtk_init(&argc,&argv);

	builder = gtk_builder_new_from_file("Interface.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder,"main_window"));
	start_button = GTK_WIDGET(gtk_builder_get_object(builder,"start_button")); //test

	//gtk_builder_connect_signals(builder,NULL); //pas test
	


	g_signal_connect(start_button,"clicked",G_CALLBACK(on_start_button_clicked),builder);//test

	//text_view = GTK_WIDGET(gtk_builder_get_object(builder,"terminal_window"));
	//buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	//gtk_widget_show_all(window);


	gtk_main();

	return 0;
}
