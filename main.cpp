/*This file isn't completed yet.*/
/*Now the problem is:															*
 *1.test1: ./Wizard --condition "Point(fABD,baCExy) st10(B,D,AD,AB,C) intersect(AC,BD,E)" --proposition "midpoint(E,AC) midpoint(E,BD)"	*
 *  is **true**,															*
 *  but ./Wizard --condition "Point(fABD,baCExy) intersect(AC,BD,E) st10(B,D,AD,AB,C)"  --proposition "midpoint(E,AC) midpoint(E,BD)"	*
 *  is **false**.															*
 *2.test2: st8(AD,BE,O) can't bring infomation to the three parallel conditions.							*
 *3.debug infomation is too long.													*
 *4.Wizard.hpp is tooooo long.														*
 *															--Feb. 24, 2024	*/
 
#include "Wizard.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <vector>
using namespace Wizard;


Analysis::VarPoint Analysis::varp; 
lst Analysis::eqncond, Analysis::varcond, Analysis::Ds, Analysis::eqnprop, Analysis::varprop, Analysis::cov;
ex Analysis::condslv, Analysis::propslv;
bool Analysis::is_cov=false;

int main(int argc, char* argv[])
{
	//test1: ./Wizard --condition "Point(fABD,baCExy) st10(B,D,AD,AB,C) intersect(AC,BD,E)" --proposition "midpoint(E,AC) midpoint(E,BD)"
	//test2: ./Wizard --condition "Point(fADBCO,baFxyE) st8(AD,BE,O) parallel(AB,DE) parallel(AC,DF) parallel(BC,EF)" --proposition "collinear(O,C,F)"
	int opt;
	const char *condition=NULL;
	const char *proposition=NULL;
	static struct option long_options[]=
	{
		{"condition", required_argument, NULL, 'c'},
		{"proposition", required_argument, NULL, 'p'},
		{"debug", no_argument, NULL, 'd'},
		{NULL, 0, NULL, 0}
	};
	if(argc<=2)
	{
		cerr<<"Usage: "<<argv[0]<<" --condition <condition> --proposition <proposition>\n"<<endl;
		return 1;
	}
	while((opt=getopt_long(argc, argv, "c:p:d", long_options, NULL))!=-1) 
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
