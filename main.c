#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

GtkWidget* weight_entry;
GtkWidget* height_entry;
GtkWidget* result_label;

static void activate(GtkApplication* app, gpointer user_data)
{
	GtkWidget* window;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "BMI Calculator");
	gtk_window_set_default_size(GTK_WINDOW(window), 420, 69);
	weight_entry = gtk_entry_new();
	height_entry = gtk_entry_new();

	gtk_entry_set_placeholder_text(GTK_ENTRY(weight_entry), "e.g: 70.25");
	gtk_entry_set_placeholder_text(GTK_ENTRY(height_entry), "e.g: 175.3");

	GtkWidget* weight_label = gtk_label_new("Weight (kg): ");
	GtkWidget* height_label = gtk_label_new("Height (cm): ");
	result_label = gtk_label_new("BMI: ");
	GtkWidget* grid = gtk_grid_new();

	gtk_grid_attach(GTK_GRID(grid), weight_label, 0, 0, 1, 1);

	gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char** argv)
{
	GtkApplication* app;
	int status;

	app = gtk_application_new("org.bmi.calculator", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
