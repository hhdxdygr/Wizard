/* This file gives an example by proving Desargues theorem. */

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

#include <iostream>
#include "Wizard.hpp"
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
	cout<<eqnlst<<endl;
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
	if(Analysis::SubsProp(slvlst, prop))
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
