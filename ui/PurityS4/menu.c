#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *button1;
GtkWidget *label1;
GtkBuilder *builder;

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

// Function to draw a graph using SDL
void drawGraph(SDL_Renderer* renderer, struct node* nodes, size_t numNodes, struct link* links, size_t numLinks) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Draw nodes
    for (size_t i = 0; i < numNodes; i++) {
        SDL_RenderDrawPoint(renderer, nodes[i].x, nodes[i].y);
    }

    // Draw links
    for (size_t i = 0; i < numLinks; i++) {
        SDL_RenderDrawLine(renderer, nodes[links[i].from].x, nodes[links[i].from].y, nodes[links[i].to].x, nodes[links[i].to].y);
    }
}

// Callback function for button click event
void on_button_clicked(GtkButton *button, gpointer data) {
    // Define graph
    struct node nodes[] = {{50, 50, 5}, {100, 100, 5}, {200, 200, 5}, {300, 100, 5}};
    struct link links[] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}};
    size_t numNodes = 4;
    size_t numLinks = 4;

    // Create SDL window and renderer
    SDL_Window *sdlWindow;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(400, 300, SDL_WINDOW_SHOWN, &sdlWindow, &renderer);

    // Draw graph
    drawGraph(renderer, nodes, numNodes, links, numLinks);

    // Present renderer to window
    SDL_RenderPresent(renderer);

    // Wait for 5 seconds
    SDL_Delay(5000);

    // Cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("menu.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL)


    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));

    g_signal_connect(button1, "clicked", G_CALLBACK(on_button_clicked), NULL);

    gtk_widget_show(window);
    gtk_main();
    return EXIT_SUCCESS;
};
