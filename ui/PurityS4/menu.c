#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
//#include <gtk/gtk.h>
#include <math.h>
#include <ctype.h>
#include <cairo.h>

GtkWidget window;
GtkWidgetfixed1;
GtkWidget button1;
GtkWidgetlabel1;
GtkBuilder builder;


// Define node structure
struct node {
    double x;
    double y;
    double radius;
};


// Define link structure
struct link {
    size_t from;
    size_t to;
};

// Function to draw a graph using Cairo
void drawGraph(struct nodenodes, size_t numNodes, struct link links, size_t numLinks) {
    // Get drawing area widget and create Cairo context
    GtkWidgetdrawing_area = gtk_builder_get_object(builder, "drawingarea1");
    cairo_t *cr = gdk_cairo_create(gtk_widget_get_window(drawing_area));
// Draw nodes
    for (size_t i = 0; i < numNodes; i++) {
        cairo_arc(cr, nodes[i].x, nodes[i].y, nodes[i].radius, 0, 2 * G_PI);
        cairo_stroke(cr);
    }


    // Draw links
    for (size_t i = 0; i < numLinks; i++) {
        cairo_move_to(cr, nodes[links[i].from].x, nodes[links[i].from].y);
        cairo_line_to(cr, nodes[links[i].to].x, nodes[links[i].to].y);
        cairo_stroke(cr);
    }


    // Cleanup Cairo context
    cairo_destroy(cr);
}

// Draw links
    for (size_t i = 0; i < numLinks; i++) {
        cairo_move_to(cr, nodes[links[i].from].x, nodes[links[i].from].y);
        cairo_line_to(cr, nodes[links[i].to].x, nodes[links[i].to].y);
        cairo_stroke(cr);
    }


    // Cleanup Cairo context
    cairo_destroy(cr);
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);


    builder = gtk_builder_new_from_file("menu.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    gtk_builder_connect_signals(builder, NULL);


    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
// Define graph
    struct node nodes[] = {{50, 50, 20}, {100, 100, 10}, {200, 200, 15}, {300, 100, 12}};
    struct link links[] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};


    // Connect drawGraph function to button click event
    g_signal_connect(button1, "clicked", G_CALLBACK(drawGraph), nodes);


    gtk_widget_show(window);
    gtk_main();
    return EXIT_SUCCESS;
}
