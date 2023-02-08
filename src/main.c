#include "bmi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	float weight = strtof(argv[1], NULL);
	float height = strtof(argv[2], NULL);
	float bmi = calculate_bmi_metric(weight, height);
	if ( bmi == NAN )
	{
	}
	return 0;
}
