#include <iostream>
#include "Wizard.h"
using namespace std;
using namespace GiNaC;
Geo::Variable Geo::var;
void append(lst& l1, const ex& l2)
{
	for(int i=0;i<l2.nops();i++)l1.append(l2[i]);
}
int main()
{
	Geo::Point A(Geo::HilbertSt::st1());
	Geo::Point B(Geo::HilbertSt::st1());
	Geo::Point D(Geo::HilbertSt::st1());
	Geo::HilbertSt::GeoEx ge_C=Geo::HilbertSt::st10(B, D, Geo::Line(A, D), Geo::Line(A, B));
	Geo::Point C(ge_C.p);
	ex slvC=lsolve(ge_C.e, ge_C.v);
	//cout<<"slvC:\t"<<slvC<<endl;
	lst chng;	append(chng, slvC);
	
	Geo::HilbertSt::GeoEx ge_E=Geo::HilbertSt::st8(Geo::Line(A, C), Geo::Line(B, D));
	Geo::Point E(ge_E.p);
	ge_E.e=ge_E.e.subs(chng);
	ex slvE=lsolve(ge_E.e, ge_E.v);
	cout<<"slvE:\t"<<slvE<<endl<<endl;

	lst conc={};
	lst conc1=Geo::Midpoint(E, A, C);
	lst conc2=Geo::Midpoint(E, B, D);
	append(conc, conc1);
	append(conc, conc2);
	cout<<"conc:"<<conc<<endl;
	return 0;
}
