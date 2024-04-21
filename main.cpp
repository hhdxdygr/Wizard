/* This file implements the command-line functionality of the Wizard. */

/*
 *  Wizard Copyright (C) 2024 hhdxdygr
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*Now the problem is:															*
 *1.Wizard.hpp is tooooo long.														*
 *															 --Mar. 23, 2024*/
 
#include "Wizard.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <vector>
using namespace Wizard;

Analysis::VarPoint Analysis::varp; 
lst Analysis::eqncond, Analysis::varcond, Analysis::Ds, Analysis::eqnprop, Analysis::varprop, Analysis::cov;
ex Analysis::condslv, Analysis::propslv;
bool Analysis::is_cov=false, Analysis::is_es=false;

void usage()
{
	cout<<"Usage: "<<argv[0]<<" --condition <condition> --proposition <proposition>\n"<<endl;
	return;
}

void version()
{
	
}

void help()
{
	usage();
	return;
}

void console()
{
	//TODO
}

int main(int argc, char* argv[])
{
	//test1: ./Wizard --condition "Point(fABD,baCExy) st10(B,D,AD,AB,C) st8(AC,BD,E)" --proposition "midpoint(E,AC) midpoint(E,BD)"
	//test2: ./Wizard --condition "Point(fADBCO,baFxyE) EstSys(oOxOADyOBE) parallel(AB,DE) parallel(AC,DF) parallel(BC,EF)" --proposition "collinear(O,C,F)"
	
	int opt;
	const char* condition=NULL;
	const char* proposition=NULL;
	static struct option long_options[]=
	{
		{"condition", required_argument, NULL, 'c'},
		{"proposition", required_argument, NULL, 'p'},
		{"debug", no_argument, NULL, 'd'},
		{"console", no_argument, NULL, 'C'},
		{"version", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
	if(argc<=2)
	{
		usage();
	}
	while((opt=getopt_long(argc, argv, "c:p:d:C:v:h", long_options, NULL))!=-1) 
	{
		switch(opt)
		{
		case 'c':
			condition=optarg;
			break;
		case 'p':
			proposition=optarg;
			break;
		case 'd':
			debug=true;
			break;
		case 'C':
			console();
			break;
		case 'v':
			version();
			break;
		case 'h':
			help();
			break;
		default:
			break;
		}
	}
	string cond=condition;
	string prop=proposition;
	Analysis::AnalysisStr(cond, true);
	Analysis::AnalysisStr(prop, false);
	return 0;
}
