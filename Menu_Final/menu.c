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
#include "../maps/finalMap.h"
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
	GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(user_data,"gtk_entry1"));
	const char *entry_text = gtk_entry_get_text(entry); //test entry 
	x = g_strdup(entry_text);
	int index = atoi(x) ;
	
	char *L[] = {"../maps/1.txt","../maps/2.txt","../maps/3.txt","../maps/4.txt","../maps/5.txt","../maps/6.txt","../maps/7.txt"};
	if (index-1<0 || index-1>7)
	{
		return -1;
		}
	finalFunction(L[index-1]);
	//txt_buffer+=("\nLa valeur de depart est : %s\n",x);
	//txt_buffer+=("\nLa valeur de d'arrivee  est : %s\n",y);
	// -----------------------
	//gtk_text_buffer_insert_at_cursor(buffer,"hello",-1); //TextView




	//const char *filename = "mapfinal";
	//FILE *fp  = fopen(filename,"w");
	//fclose;
	//g_print("le bouton a ete cliquer");
}

void on_header_quit_button_clicked(GtkButton *button,gpointer user_data)
{
	gtk_main_quit();
}


int main(int argc,char *argv[])
{
	GtkBuilder *builder;
	GtkWidget *window;
	GtkWidget *start_button; //test
	GtkWidget *end_button; 
	//GtkWidget *text_view;
	//GtkTextBuffer *buffer;

	gtk_init(&argc,&argv);

	builder = gtk_builder_new_from_file("Interface.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder,"main_window"));
	start_button = GTK_WIDGET(gtk_builder_get_object(builder,"start_button")); //test
	end_button = GTK_WIDGET(gtk_builder_get_object(builder,"header_quit_button")); 

	//gtk_builder_connect_signals(builder,NULL); //pas test
	
	gtk_builder_connect_signals(builder,NULL);

	g_signal_connect(start_button,"clicked",G_CALLBACK(on_start_button_clicked),builder);//test
	g_signal_connect(end_button,"clicked",G_CALLBACK(on_header_quit_button_clicked),NULL);

	//text_view = GTK_WIDGET(gtk_builder_get_object(builder,"terminal_window"));
	//buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	//gtk_widget_show_all(window);


	gtk_main();

	return 0;
}
