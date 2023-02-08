#include <math.h>

float calculate_bmi_metric(float weight, float height)
{
	if ( height == 0 )
		return NAN;
	return (weight / (height * height));
}
float calculate_bmi_imperial(float, float);
