#include <gtk/gtk.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define RESOURCEPREFIX "/org/gtk/bmicalculator/"
#define MAXBMIBUFFER 7
#define ACTIVATE_SIGNAL_ID 155

#define NORMALMIN 18.5f
#define NORMALMAX 24.9f
#define OVERWEIGHTMIN 25.0f
#define OVERWEIGHTMAX 29.9f
#define OBESEMIN 30.0f
#define OBESEMAX 34.9f

GtkWidget* weight_entry;
GtkWidget* height_entry;
GtkWidget* bmi_entry;
GtkEntryBuffer* weight_entry_buffer;
GtkEntryBuffer* height_entry_buffer;
GtkEntryBuffer* bmi_entry_buffer;

GtkWidget* result_label;
GtkWidget* calculate_button;

char* get_result(float bmi)
{
	if ( bmi < NORMALMIN )
	{
		return "You are underweight\nConsider eating more";
	}
	if ( bmi >= NORMALMIN && bmi <= NORMALMAX )
	{
		return "You are normal weight";
	}
	if ( bmi >= OVERWEIGHTMIN && bmi <= OVERWEIGHTMAX )
	{
		return "You are overweight\nNot very dangerous";
	}
	if ( bmi >= OBESEMIN && bmi <= OBESEMAX )
	{
		return "You are obese\nConsider going on a diet and doing sports";
	}
	if ( bmi > OBESEMAX )
	{
		return "You are extremely obese\nIt's best to go to a nutritionist";
	}
	return NULL;
}

static void calculate_bmi()
{
	// get the text in the entry buffers
	const char* weight_str = gtk_entry_buffer_get_text(weight_entry_buffer);
	const char* height_str = gtk_entry_buffer_get_text(height_entry_buffer);
	// get the lenght of the text in the entry buffers
	uint64_t weight_strlength = gtk_entry_buffer_get_bytes(weight_entry_buffer);
	uint64_t height_strlength = gtk_entry_buffer_get_bytes(height_entry_buffer);

	char* strend;

	// if any of the entries are empty, do nothing
	if ( weight_strlength == 0 || height_strlength == 0 )
	{
		return;
	}

	// convert the text in the weight buffer to a float
	long double weight = strtof(weight_str, &strend);
	// if strtof didn't consume the entire string
	// that means that the weight buffer was invalid, return nothing
	if ( strend - weight_str != weight_strlength )
	{
		return;
	}

	// same thing as above but for the height buffer
	float height = strtof(height_str, &strend);
	if ( strend - height_str != height_strlength )
	{
		return;
	}

	// we turn the height in centimeters to meters
	height /= 100;

	// the formula for BMI is the weight in kilograms divided by the square of the height in meters
	long double bmi = weight / (height * height);

	// round the BMI value to the nearest hundredth
	bmi = floor(bmi * 100 + 0.5) / 100;

	// create a character array that will hold our
	// result in text format
	char bmi_str[MAXBMIBUFFER];
	//
	snprintf(bmi_str, MAXBMIBUFFER, "%.2Lf", bmi);

	gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(bmi_entry)), bmi_str, -1);

	gtk_label_set_text(GTK_LABEL(result_label), get_result(bmi));
	gtk_widget_set_visible(result_label, TRUE);
}

static void click_button()
{
	g_signal_emit(calculate_button, ACTIVATE_SIGNAL_ID, 0, NULL);
}

static void clear()
{

	gtk_entry_buffer_set_text(bmi_entry_buffer, "\0", -1);
	gtk_label_set_text(GTK_LABEL(result_label), "\0");
	gtk_widget_set_visible(result_label, FALSE);
}

static void activate(GtkApplication* app, gpointer user_data)
{
	GtkBuilder* builder = gtk_builder_new_from_resource(RESOURCEPREFIX "bmicalculator.ui");

	GObject* mainwindow = gtk_builder_get_object(builder, "mainwindow");

	weight_entry = GTK_WIDGET(gtk_builder_get_object(builder, "WeightEntry"));
	height_entry = GTK_WIDGET(gtk_builder_get_object(builder, "HeightEntry"));
	bmi_entry = GTK_WIDGET(gtk_builder_get_object(builder, "BmiEntry"));

	weight_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(weight_entry));
	height_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(height_entry));
	bmi_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(bmi_entry));

	calculate_button = GTK_WIDGET(gtk_builder_get_object(builder, "CalcButton"));

	result_label = GTK_WIDGET(gtk_builder_get_object(builder, "ResultLabel"));

	g_signal_connect(calculate_button, "clicked", G_CALLBACK(calculate_bmi), NULL);
	g_signal_connect(weight_entry, "activate", G_CALLBACK(click_button), NULL);
	g_signal_connect(height_entry, "activate", G_CALLBACK(click_button), NULL);

	g_signal_connect(gtk_entry_get_buffer(GTK_ENTRY(weight_entry)), "inserted-text", G_CALLBACK(clear), NULL);
	g_signal_connect(gtk_entry_get_buffer(GTK_ENTRY(height_entry)), "inserted-text", G_CALLBACK(clear), NULL);
	g_signal_connect(gtk_entry_get_buffer(GTK_ENTRY(weight_entry)), "deleted-text", G_CALLBACK(clear), NULL);
	g_signal_connect(gtk_entry_get_buffer(GTK_ENTRY(height_entry)), "deleted-text", G_CALLBACK(clear), NULL);

	GtkCssProvider* cssprovider = gtk_css_provider_new();
	gtk_css_provider_load_from_resource(cssprovider, RESOURCEPREFIX "styles.css");

	gtk_style_context_add_provider_for_display(
	    gdk_display_get_default(), GTK_STYLE_PROVIDER(cssprovider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	g_object_unref(builder);
	gtk_window_set_application(GTK_WINDOW(mainwindow), app);
	gtk_window_present(GTK_WINDOW(mainwindow));
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
