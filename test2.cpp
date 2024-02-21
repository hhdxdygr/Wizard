#include <iostream>
#include "Wizard.h"
using namespace std;
using namespace GiNaC;
using namespace Wizard;

 /*Known: AB||A'B', AC||A'C', BC||B'C', O∈AA', O∈BB'	*/
 /*Proposition: O∈CC'	*/
int main()
{
	/*For simplicity, we directly initialize the points required for the proof.*/
	/*u represents the parameter, and x represents the variable.*/
	symbol u0("u0"), u1("u1"), u2("u2"), u3("u3"), u4("u4"), u5("u5"), x1("x1"), x2("x2"), x3("x3");
	Point A(u1, u0);
	Point A_(u2, u0);
	Point B(u0, u3);
	Point C(u4, u5);
	Point B_(u0, x1);
	Point C_(x2, x3);
	ex e1=parallel(A, B, A_, B_);
	ex e2=parallel(A, C, A_, C_);
	ex e3=parallel(B, C, B_, C_);
	lst eqnlst={e1, e2, e3};
	lst varlst={x1, x2, x3};
	//cout<<eqnlst<<endl;
	//cout<<varlst<<endl;
	ex slv=lsolve(eqnlst, varlst);
	lst slvlst={};
	append(slvlst, slv);
	cout<<"slv:"<<slv<<endl;
	
	/*make the equation about the proposition*/
	Point O(u0, u0);
	ex prop=collinear(O, C, C_);
	cout<<"prop:"<<prop<<endl;
	
	/*judge if the proposition can be proved*/
	if(SubsProp(slvlst, prop))
	{
		cout<<"true"<<endl;
		cout<<"Note:\n";
		cout<<"A";A.print();
		cout<<"B";B.print();
		cout<<"C";C.print();
		cout<<"A'";A_.print();
		cout<<"B'";B_.print();
		cout<<"C'";C_.print();
		cout<<"O";O.print();
		//cout<<"the generic condition is\n"<<ge_B_.D<<endl<<ge_C_.D<<endl<<ge_O.D<<endl;
	}
	else cout<<"false"<<endl;
	
	return 0;
}
