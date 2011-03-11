/*
 *  CalcPlan.h
 *  Distribution
 *
 *  Created by Andrey on 10.02.11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CALC_PLAN_H_
#define _CALC_PLAN_H_
#include <stdio.h>
#include <stdlib.h>

struct TCalcPlan
{
	TCalcPlan(int argc, char * const argv[])
	{
		FILE * ifile;
		char usage_string[] = "Usage: %s points_file division_number\n";
		
		// 1. Must be 3 parameters
		if (argc != 3) {
			fprintf(stderr, usage_string, argv[0]);
			exit(20);
		}
		
		// 2. File must exist
		filename = argv[1];
		ifile = fopen(filename, "rb");
		if (!ifile) {
			fprintf(stderr, "No such file: %s\n", filename);
			fprintf(stderr, usage_string, argv[0]);
			exit(21);
		}
		fclose(ifile);
		
		// 3. Divisions number must be valid
		divisions = atoi(argv[2]);
		if (divisions <= 0) {
			fprintf(stderr, usage_string, argv[0]);
			exit(22);
		}
	}
	char * filename;
	int divisions;
};

#endif 