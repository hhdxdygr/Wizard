#include <iostream>
#include "Wizard.hpp"
using namespace std;
using namespace GiNaC;
using namespace Wizard;

int main()
{

/*		   D/------------------/C		*
 *		   /  \		    / /			*
 *		  /	\       /    /			*
 *		 /	  \  /      /			*
 *		/        / E\      /			*
 *	       /     /	      \   /			*
 *	      /	/	       \ /			*
 *	    A/------------------/B			*/
 
 /*Known: AB||CD, AD||BC, E∈AC, E∈BD	*/
 /*Proposition: AE=CE, BE=DE		*/
 
	/*create points*/
	Point A(HilbertSt::st1());
	Point B(HilbertSt::st1());
	Point D(HilbertSt::st1());
	/*C is the intersection point of BC and CD, which is also the parallel lines of AD and AB.*/
	HilbertSt::GeoEx ge_C=HilbertSt::st10(B, D, Line(A, D), Line(A, B));
	Point C(ge_C.p);
	/*solve the equations of C*/
	ex slvC=lsolve(ge_C.e, ge_C.v);
	/*change of variable*/
	lst cov;	append(cov, slvC);
	/*E is the intersection point of AC and BD.*/
	HilbertSt::GeoEx ge_E=HilbertSt::st8(Line(A, C), Line(B, D));
	Point E(ge_E.p);
	/*change the variable*/
	ge_E.e=ge_E.e.subs(cov);
	ex slvE=lsolve(ge_E.e, ge_E.v);
	//cout<<"slvE:"<<slvE<<endl;
	lst slvE_lst;	append(slvE_lst, slvE);
	
	/*solve the equations about the proposition*/
	lst prop={};
	/*In fact, point C is controlled by point A, B and D.*/
	/*So we don't add the proposition.*/
	//lst prop1=Midpoint(E, A, C);
	lst prop2=Midpoint(E, B, D);
	//append(prop, prop1);
	append(prop, prop2);
	//cout<<"prop:"<<prop<<endl;
	
	/*judge if the two lists are equivalent*/
	/*you can use SubsProp(slvE_lst, prop), too*/
	if(Analysis::EquivalentProp(slvE_lst, prop))
	{
		cout<<"true"<<endl;
		cout<<"Note:\n";
		cout<<"A";A.print();
		cout<<"B";B.print();
		cout<<"C";C.print();
		cout<<"D";D.print();
		cout<<"E";E.print();
		cout<<"the generic condition is\n"<<ge_C.D<<endl<<ge_E.D<<endl;
	}
	else cout<<"false"<<endl;
	
	return 0;
}
