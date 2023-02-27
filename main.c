#include <gtk/gtk.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define RESOURCEPREFIX "/org/gtk/bmicalculator/"
#define MAXBMIBUFFER 7

GtkWidget* weight_entry;
GtkWidget* height_entry;
GtkWidget* result_label;
GtkWidget* calculate_button;
GtkWidget* bmi_entry;

void calculate_bmi()
{
	const char* weight_str = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(weight_entry)));
	const char* height_str = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(height_entry)));
	char* strend;
	uint64_t weight_strlength = gtk_entry_buffer_get_bytes(gtk_entry_get_buffer(GTK_ENTRY(weight_entry)));
	uint64_t height_strlength = gtk_entry_buffer_get_bytes(gtk_entry_get_buffer(GTK_ENTRY(height_entry)));
	if ( weight_strlength == 0 || height_strlength == 0 )
	{
		return;
	}

	float weight = strtof(weight_str, &strend);
	// if ( strend - weight_str != weight_strlength )
	// {
	// 	return;
	// }
	float height = strtof(height_str, &strend);
	// if ( strend - height_str != height_strlength )
	// {
	// 	return;
	// }
	// we turn the height in centimeters to meters
	height /= 100;

	// the formula for BMI is the weight in kilograms divided by the square of the height in meters
	float bmi = weight / (height * height);

	char bmi_str[MAXBMIBUFFER];
	snprintf(bmi_str, 7, "%.2f", bmi);

	gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(bmi_entry)), bmi_str, -1);
}

void activate(GtkApplication* app, gpointer user_data)
{
	GtkBuilder* builder = gtk_builder_new_from_resource(RESOURCEPREFIX "bmicalculator.ui");

	GObject* window = gtk_builder_get_object(builder, "mainwindow");
	weight_entry = GTK_WIDGET(gtk_builder_get_object(builder, "WeightEntry"));
	height_entry = GTK_WIDGET(gtk_builder_get_object(builder, "HeightEntry"));
	result_label = GTK_WIDGET(gtk_builder_get_object(builder, "BmiLabel"));
	calculate_button = GTK_WIDGET(gtk_builder_get_object(builder, "CalcButton"));
	bmi_entry = GTK_WIDGET(gtk_builder_get_object(builder, "BmiEntry"));

	g_signal_connect(calculate_button, "clicked", G_CALLBACK(calculate_bmi), NULL);

	GtkCssProvider* cssprovider = gtk_css_provider_new();
	gtk_css_provider_load_from_resource(cssprovider, RESOURCEPREFIX "styles.css");

	gtk_style_context_add_provider_for_display(
	    gdk_display_get_default(), GTK_STYLE_PROVIDER(cssprovider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	gtk_window_set_application(GTK_WINDOW(window), app);
	gtk_window_present(GTK_WINDOW(window));

	g_object_unref(builder);
}

int main(int argc, char** argv)
{
	GtkApplication* app;
	int status;

	app = gtk_application_new("org.gtk.bmicalculator", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
