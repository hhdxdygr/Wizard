/* This file implements all the necessary functions for handling geometric proof problems in Wizard. */

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

/*https://z-library.se/book/11810820/2f0956/%E5%87%A0%E4%BD%95%E5%AE%9A%E7%90%86%E6%9C%BA%E5%99%A8%E8%AF%81%E6%98%8E%E7%9A%84%E5%9F%BA%E6%9C%AC%E5%8E%9F%E7%90%86.html*/

#ifndef __WIZARD_HPP__
#define __WIZARD_HPP__

//ANSI color codes
#define RESET	"\033[0m"
#define RED	"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define MAGENTA	"\033[35m"
#define CYAN	"\033[36m"
#define WHITE	"\033[37m"

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <string>

#include <ginac/ginac.h>

using namespace std;
using namespace GiNaC;

namespace Wizard
{
	/* -d or --debug */
	bool debug=false;
	/*Definition of Point*/
	class Point
	{
	public:
		Point(){}
		~Point(){}
		Point(const ex& _x, const ex& _y)
		{
			x=_x, y=_y;
		}
		Point(const Point& _p)
		{
			x=_p.x, y=_p.y;
		}
		Point& operator=(const Point& _p)
		{
			if(this!=&_p)
				x=_p.x, y=_p.y;
			return *this;
		}
		void print()
		{
			cout<<"("<<x<<","<<y<<")"<<endl;
		}
		friend ostream& operator<<(ostream& os, const Point& p)
		{
			os<<"("<<p.x<<","<<p.y<<")";
			return os;
		}
		void modify(const ex& _x, const ex& _y)
		{
			x=_x, y=_y;
		}
		void modify(const Point& _p)
		{
			x=_p.x, y=_p.x;
		}
		ex equal(const Point& _p)
		{
			return lst{x==_p.x, y==_p.y};
		}
		ex x, y;
	};
	/*Definition of Line*/
	class Line
	{
	public:
		Line(){}
		~Line(){}
		Line(const Point& _p1, const Point& _p2)
		{
			x1=_p1.x, x2=_p2.x, y1=_p1.y, y2=_p2.y;
		}
		Line(const Line& _l)
		{
			x1=_l.x1, x2=_l.x2, y1=_l.y1, y2=_l.y2;
		}
		Line& operator=(const Line& _l)
		{
			if(this!=&_l)
				x1=_l.x1, x2=_l.x2, y1=_l.y1, y2=_l.y2;
			return *this;
		}
		ex x1, x2, y1, y2;
	};
	/*Definition of Segment*/
	//Seems this is not very useful.
	class Segment
	{
	public:
		Segment(){}
		~Segment(){}
		Segment(const ex& _x1, const ex& _y1, const ex& _x2, const ex& _y2)
		{
			x1=_x1, y1=_y1, x2=_x2, y2=_y2;
		}
		Segment(const Point& _p1, const Point& _p2)
		{
			x1=_p1.x, x2=_p2.x, y1=_p1.y, y2=_p2.y;
		}
		Segment& operator=(const Segment& _s)
		{
			if(this!=&_s)
				x1=_s.x1, x2=_s.x2, y1=_s.y1, y2=_s.y2;
			return *this;
		}
		ex x1, y1, x2, y2;
	};
	
	/*append ex to lst without '{' or '}'*/
	void append(lst& l1, const ex& l2)
	{
		if(is_a<lst>(l2))
			for(size_t i=0; i<l2.nops(); i++)
				l1.append(l2[i]);
		else
			l1.append(l2);
	}
	/*substitute lst with lst*/
	void subs(lst& l1, const lst& l2)
	{
		for(size_t i=0; i<l1.nops(); i++)
			l1.op(i)=l1.op(i).subs(l2);
	}
	
/*Now we define relations of points and line segments.*/
	
	/*parallel line segments:	A-------B	*
	 *				C-------D	*/
	ex parallel(const Segment& _s1, const Segment& _s2)
	{
		return ((_s1.y1-_s1.y2)*(_s2.x1-_s2.x2)-(_s2.y1-_s2.y2)*(_s1.x1-_s1.x2)==0);
	}
	/*the segment p1p2 is parallel to the segment p3p4*/
	ex parallel(const Point& _p1, const Point& _p2, const Point& _p3, const Point& _p4)
	{
		return parallel(Segment(_p1, _p2), Segment(_p3, _p4));
	}
	/*perpendicular line segments:	A----+--B	*
	 *				    C|		*
	 *			 	     |		*
	 *				    D|		*/
	ex perpendicular(const Segment& _s1, const Segment& _s2/*, const int& _k=1*/)
	{
		return ((_s1.y1-_s1.y2)*(_s2.y1-_s2.y2)+(_s1.x1-_s1.x2)*(_s2.x1-_s2.x2)==0);
	}
	/*the segment p1p2 is perpendicular to the segment p3p4*/
	ex perpendicular(const Point& _p1, const Point& _p2, const Point& _p3, const Point& _p4)
	{
		return perpendicular(Segment(_p1, _p2), Segment(_p3, _p4));
	}
	/*a point is on a line segment(three points are collinear):	A----C-------B	*/
	ex collinear(const Point& _p, const Segment& _s)
	{
		return (determinant(matrix{{_p.x, _p.y,	1},
					   {_s.x1,_s.y1,1},
				 	   {_s.x2,_s.y2,1}})==0);
	}
	ex collinear(const Point& _p1, const Point& _p2, const Point& _p3)
	{
		return collinear(_p1, Segment(_p2, _p3));
	}
	/*the relation of the order of points*/
	bool order(const Point& _p, const Point& _p1, const Point& _p2)
	{
		return 	(_p1.x<_p.x&&_p.x<_p2.x)||
			(_p2.x<_p.x&&_p.x<_p1.x)||
			(_p1.y<_p.y&&_p.y<_p2.y)||
			(_p2.y<_p.y&&_p.y<_p1.y);
	}
	
	/*a section point:	(internal): 	A-----C----------B		*
	 *		  	(external):	A---------B------C		*
	 * We define section=AC/BC.(A(_s.x1,_s.y1), B(_s.x2,_s.y2))	*/
	lst SectionPoint(	const Point& _p, const Segment& _s,	//type:	false->internal 
		       			const ex& _section, bool type=false)  	// 	true ->external
	{
		if(type==false)		//internal
			return 	lst{_p.x==(_section*_s.x2+_s.x1)/(_section+1),
				    _p.y==(_section*_s.y2+_s.y1)/(_section+1)};
		else				//external
			return 	lst{_p.x==(_section*_s.x2-_s.x1)/(_section-1),
				    _p.y==(_section*_s.y2-_s.y1)/(_section-1)};
	}
	/*for special cases: midpoints*/
	lst Midpoint(const Point& _p, const Segment& _s)
	{
		return 	 lst{_p.x==(_s.x1+_s.x2)/2,
			     _p.y==(_s.y1+_s.y2)/2};
	}
	/*p1 is the midpoint of the segment p2p3*/
	lst Midpoint(const Point& _p1, const Point& _p2, const Point& _p3)
	{
		return Midpoint(_p1, Segment(_p2, _p3));
	}

	class HilbertSt
	{
	public:
		class GeoEx
		{
		public:
			~GeoEx(){}
			GeoEx(const Point& _p, const ex& _e, const lst& _v)
			{
				p.x=_p.x, p.y=_p.y, e=_e, v=_v;
				type=10;
			}
			GeoEx(const Point& _p, const ex& _e, const ex& _D, const lst& _v)
			{
				p.x=_p.x, p.y=_p.y, e=_e, D=_D, v=_v;
				type=11;
			}
			GeoEx(const Line& _l, const ex& _e, const lst& _v)
			{
				l.x1=_l.x1, l.x2=_l.x2, l.y1=_l.y1, l.y2=_l.y2, e=_e, v=_v;
				type=20;
			}
			GeoEx(const Line& _l, const ex& _e, const ex& _D, const lst& _v)
			{
				l.x1=_l.x1, l.x2=_l.x2, l.y1=_l.y1, l.y2=_l.y2, e=_e, D=_D, v=_v;
				type=21;
			}
			GeoEx(const Point& _p, const Line& _l, const ex& _e, const lst& _v)
			{
				p.x=_p.x, p.y=_p.y, l.x1=_l.x1, l.x2=_l.x2, l.y1=_l.y1, l.y2=_l.y2, e=_e, v=_v;
				type=30;
			}
			GeoEx(const Point& _p, const Line& _l, const ex& _e, const ex& _D, const lst& _v)
			{
				p.x=_p.x, p.y=_p.y, l.x1=_l.x1, l.x2=_l.x2, l.y1=_l.y1, l.y2=_l.y2, e=_e, D=_D, v=_v;
				type=31;
			}
			Point p;	//store a point
			Line l;		//store a line
			ex e, D;	//e is the expression
					//D is the generic condition
			lst v;		//the variable that should be solved
			/* type: 10->Point;		20->Line;	30->Point&Line	 *
			 *	 11->Point&D;		21->Line&D;	31->Point&Line&D */
			int type;
		};
		HilbertSt(){}
		~HilbertSt(){}
		/*steps which create new u_i; we don't care about step4.*/
		static Point st1() {symbol u1, u2; return Point(u1, u2);}
		static Line st2() {symbol u1, u2, u3, u4; return Line(Point(u1, u2), Point(u3, u4));}
		static Line st3(const Point& p) {symbol u1, u2; return Line(p, Point(u1, u2));}
		static Line st4(const Point& p1, const Point& p2) {return Line(p1, p2);}
		/*steps which return a equation*/
		static GeoEx st5(const Line& l)
		{
			symbol u1, x1; 
			return GeoEx(Point(u1, x1), 
				     determinant(matrix{{u1,   x1,   1}, 
							{l.x1, l.y1, 1},
							{l.x2, l.y2, 1}}), 
				     lst{x1});
		}
		static GeoEx st5(const Line& l, const Point& obj)
		{
			return GeoEx(obj, 
				     determinant(matrix{{obj.x,obj.y,1}, 
							{l.x1, l.y1, 1},
							{l.x2, l.y2, 1}}), 
				     lst{obj.y});
		}
		static GeoEx st6(const Line& l)
		{
			symbol u1, u2, u3, x1;
			return GeoEx(Line(Point(u1, u2),
				     Point(u3, x1)),
				     ex((l.x2-l.x1)*(x1-u2)-(l.y2-l.y1)*(u3-u1)==0),
				     lst{x1});
		}
		static GeoEx st6(const Line& l, const Point& obj1, const Point& obj2)
		{
			return GeoEx(Line(obj1, obj2),
				     ex((l.x2-l.x1)*(obj2.y-obj1.y)-(l.y2-l.y1)*(obj2.x-obj1.x)==0),
				     lst{obj2.y});
		}
		static GeoEx st7(const Line& l, const Point& p)
		{
			symbol u1, x1;
			return GeoEx(Point(u1, x1),
				     Line(Point(u1, x1), p),
				     ex((l.x2-l.x1)*(x1-p.y)-(l.y2-l.y1)*(u1-p.x)==0),
				     lst{x1});
		}
		static GeoEx st7(const Line& l, const Point& p, const Point& obj)
		{
			return GeoEx(obj, Line(obj, p),
				     ex((l.x2-l.x1)*(obj.y-p.y)-(l.y2-l.y1)*(obj.x-p.x)==0),
				     lst{obj.y});
		}
		/*steps which return two equations*/
		static GeoEx st8(const Line& l1, const Line& l2) 	
		{
			symbol x1, x2;
			return GeoEx(Point(x1, x2), lst{((l1.y1-l1.y2)*x1-(l1.x1-l1.x2)*x2+l1.x1*l1.y2-l1.x2*l1.y1==0), 
							((l2.y1-l2.y2)*x1-(l2.x1-l2.x2)*x2+l2.x1*l2.y2-l2.x2*l2.y1==0)},
				     ex(determinant(matrix{{l1.y1-l2.y2, l1.x2-l1.x1}, {l2.y1-l2.y2, l2.x2-l2.x1}})!=0),
				     lst{x1, x2});
		}
		static GeoEx st8(const Line& l1, const Line& l2, const Point& obj) 	
		{
			return GeoEx(obj, lst{((l1.y1-l1.y2)*obj.x-(l1.x1-l1.x2)*obj.y+l1.x1*l1.y2-l1.x2*l1.y1==0), 
					      ((l2.y1-l2.y2)*obj.x-(l2.x1-l2.x2)*obj.y+l2.x1*l2.y2-l2.x2*l2.y1==0)},
				     ex(determinant(matrix{{l1.y1-l2.y2, l1.x2-l1.x1}, {l2.y1-l2.y2, l2.x2-l2.x1}})!=0),
				     lst{obj.x, obj.y});
		}
		static GeoEx st9(const Point& p, const Line& l1, const Line& l2)
		{
			symbol x1, x2;
			return GeoEx(Point(x1, x2), lst{((l1.x2-l1.x1)*(x2-p.y)-(l1.y2-l1.y1)*(x1-p.x)==0),
							((l2.y1-l2.y2)*x1-(l2.x1-l2.x2)*x2+l2.x1*l2.y2-l2.x2*l2.y1==0)},
				     ex(determinant(matrix{{l1.y2-l1.y1, l1.x1-l1.x2}, {l2.y1-l2.y2, l2.x2-l2.x1}})!=0),
				     lst{x1, x2});
		}
		static GeoEx st9(const Point& p, const Line& l1, const Line& l2, const Point& obj)
		{
			return GeoEx(obj, lst{((l1.x2-l1.x1)*(obj.y-p.y)-(l1.y2-l1.y1)*(obj.x-p.x)==0),
					      ((l2.y1-l2.y2)*obj.x-(l2.x1-l2.x2)*obj.y+l2.x1*l2.y2-l2.x2*l2.y1==0)},
				     ex(determinant(matrix{{l1.y2-l1.y1, l1.x1-l1.x2}, {l2.y1-l2.y2, l2.x2-l2.x1}})!=0),
				     lst{obj.x, obj.y});
		}
		static GeoEx st10(const Point& p1, const Point& p2, const Line& l1, const Line& l2) 
		{
			symbol x1, x2;
			return GeoEx(Point(x1, x2), lst{((p1.y-x2)*(l1.x2-l1.x1)-(l1.y2-l1.y1)*(p1.x-x1)==0),
							((p2.y-x2)*(l2.x2-l2.x1)-(l2.y2-l2.y1)*(p2.x-x1)==0)},
				     ex(determinant(matrix{{l1.y2-l1.y1, l1.x1-l1.x2}, {l2.y2-l2.y1, l2.x1-l2.x2}})!=0),
				     lst{x1, x2});
		}
		static GeoEx st10(const Point& p1, const Point& p2, const Line& l1, const Line& l2, const Point& obj) 
		{
			return GeoEx(obj, lst{((p1.y-obj.y)*(l1.x2-l1.x1)-(l1.y2-l1.y1)*(p1.x-obj.x)==0),
					      ((p2.y-obj.y)*(l2.x2-l2.x1)-(l2.y2-l2.y1)*(p2.x-obj.x)==0)},
				     ex(determinant(matrix{{l1.y2-l1.y1, l1.x1-l1.x2}, {l2.y2-l2.y1, l2.x1-l2.x2}})!=0),
				     lst{obj.x, obj.y});
		}
	};

	class Analysis
	{
	public:
		/*Definition of VarPoint*/
		class VarPoint
		{
		public:
			static const int max_v=100;
			Point s[max_v]={};
			bool free[max_v][2]={};
			string name="";
			int vi=0;
			void append(const Point& p, const bool& typex, const bool& typey, const char& n)
			{
				s[vi]=p;
				free[vi][0]=typex;
				free[vi][1]=typey;
				name[vi]=n;
				vi++;
				return;
			}
			void print()
			{
				cout<<"Point\t\t\tis_free_x\tis_free_y"<<endl;
				for(int i=0; i<vi; i++)
				{
					//process the table by using a simple method
					ostringstream oss;
					oss<<"("<<s[i].x<<","<<s[i].y<<")";
					if(oss.str().size()<9)
						cout<<name[i]<<s[i]<<"\t\t\t"<<free[i][0]<<"\t\t"<<free[i][1]<<endl;
					else if(oss.str().size()<13)
						cout<<name[i]<<s[i]<<"\t\t"<<free[i][0]<<"\t\t"<<free[i][1]<<endl;
					else 
						cout<<name[i]<<s[i]<<"\t"<<free[i][0]<<"\t\t"<<free[i][1]<<endl;
				}
				return;
			}
			int findp(const char& p)
			{
				for(int i=0; i<vi; i++)
					if(name[i]==p)
						return i;
				string err=string("findp: cannot find point ")+string(&p, 1)+string("!\n");
				throw runtime_error(err.c_str());
				return -1;
			}
			Point getp(const char& p)
			{
				return s[findp(p)];
			}
			Point* getaddp(const char& p)
			{
				return &s[findp(p)];
			}
			void modifyp(const Point& p, const char& n)
			{
				s[findp(n)]=p;
			}
			lst boundvars()
			{
				lst bv={};
				for(int i=0; i<vi; i++)
				{
					if(!free[i][0])bv.append(s[i].x);
					if(!free[i][1])bv.append(s[i].y);
				}
				return bv;
			}
		};
		static VarPoint varp; 
		static lst eqncond, varcond, Ds, eqnprop, varprop, cov;
		static ex condslv, propslv;
		static bool is_cov, is_es;
		
		/*determine whether the variable is bound or free*/
		/* bound->false *
		 * free->true   */
		static bool DetermineVar(ex var)
		{
			for(int i=0; i<varp.vi; i++)
				if((varp.s[i].x==var&&varp.free[i][0]==false)||(varp.s[i].y==var&&varp.free[i][1]==false))
					return false;
			return true;
		}
		
		/*determine whether the proposition is bound or free*/
		/* bound->false *
		 * free->true   */
		//**The implementation of this function is problematic.**//
		static bool DetermineExpr(ex expr)
		{
			int lnum=0, rnum=0;
			if(is_a<lst>(expr))
			{
				if(debug)
				{
					for(size_t j=0; j<expr.nops(); j++)
						cout<<"DetermineExpr: "<<expr.op(j)<<endl;
				}
				for(size_t j=0; j<expr.nops(); j++)
				{
					for(int i=0; i<varp.vi; i++)
					{
						if(is_a<numeric>(expr.op(j)))continue;
						if(expr.op(j).op(0).has(varp.s[i].x)&&varp.free[i][0]==false)lnum++;
						if(expr.op(j).op(0).has(varp.s[i].y)&&varp.free[i][1]==false)lnum++;
						if(expr.op(j).op(1).has(varp.s[i].x)&&varp.free[i][0]==false)rnum++;
						if(expr.op(j).op(1).has(varp.s[i].y)&&varp.free[i][1]==false)rnum++;
					}
					if(lnum==1&&rnum>0)return false;
					if(rnum==1&&lnum>0)return false;
					lnum=0, rnum=0;
				}
				return true;
			}
			else
			{
				if(debug) cout<<"DetermineExpr: "<<expr<<endl;
				for(int i=0; i<varp.vi; i++)
				{
					if(is_a<numeric>(expr))continue;
					if(expr.op(0).has(varp.s[i].x)&&varp.free[i][0]==false)lnum++;
					if(expr.op(0).has(varp.s[i].y)&&varp.free[i][1]==false)lnum++;
					if(expr.op(1).has(varp.s[i].x)&&varp.free[i][0]==false)rnum++;
					if(expr.op(1).has(varp.s[i].y)&&varp.free[i][1]==false)rnum++;
				}
				if(lnum==1&&rnum>0)return false;
				if(rnum==1&&lnum>0)return false;
				return true;
			}
			
		}
		
		/*determine the relation between slv and prop*/
		/*judge if the two propositions are equivalent*/
		static bool EquivalentProp(const lst& prop1, const lst& prop2)
		{
			for(lst::const_iterator i=prop1.begin(); i!=prop1.end(); ++i)
				if(!prop2.has(*i))return false;
			for(lst::const_iterator i=prop2.begin(); i!=prop2.end(); ++i)
				if(!prop1.has(*i))return false;
			return true;
		}
		
		/*substitute slv into prop_eqn and judge*/
		static bool SubsProp(const lst& slv, const ex& prop_eqn)
		{
			ex prv=prop_eqn.subs(slv);
			relational rel=ex_to<relational>(prv);
			if(ex_to<relational>(rel.canonical().normal()))
				return true;
			return false;
		}
		static bool SubsProp(const ex& slv, const ex& prop_eqn)
		{
			if(is_a<lst>(slv)&&is_a<lst>(prop_eqn))
			{
				lst slv_lst, prop_eqn_lst;
				append(slv_lst, slv);
				append(prop_eqn_lst, prop_eqn);
				return SubsProp(slv_lst, prop_eqn_lst);
			}
			ex prv=prop_eqn.subs(slv);
			relational rel=ex_to<relational>(prv);
			if(ex_to<relational>(rel.canonical().normal()))
				return true;
			return false;
		}
		static bool SubsProp(const lst& slv, const lst& prop_eqn)
		{
			for(size_t i=0; i<prop_eqn.nops(); i++)
			{
				ex prv=prop_eqn[i].subs(slv);
				relational rel=ex_to<relational>(prv);
				if(!ex_to<relational>(rel.canonical().normal()))
					return false;
			}
			return true;
		}
		
		/*string manipulation functions*/
		static vector<string> SplitStr(const string& s, const char& delim)
		{
			vector<string> tokens;
			int start=0, end=0;
			while((size_t(end=s.find(delim, start)))!=string::npos)
			{
				tokens.push_back(s.substr(start, end-start));
				start=end+1;
			}
			tokens.push_back(s.substr(start));
			return tokens;
		}
		static string StrSegment(const string& s, const char& fr, const char& to)
		{
			int f=s.find(fr), t=s.find(to);
			string rt="";
			for(int i=f+1; i<t; i++)
				rt+=s[i];
			return rt;
		}
		static string StrSegment(const string& s, const char& fr, const string& to)
		{
			int f=s.find(fr);
			string rt="";
			for(size_t i=f+1; i<s.size(); i++)
				if(to.find(s[i])==string::npos)
					rt+=s[i];
				else break;
			return rt;
		}
		
		/*
		static vector<string> SplitFunc(const string& s)
		{
			vector<string> strs=SplitStr(s, '(');					//erase '('
			string name=strs[0];
			vector<string> poses=SplitStr(strs[1].erase(strs[1].size()-1), ',');	//erase ')' and ','
			int npos=poses.size();
			if(npos==2||npos==3)			//number of arguments
				return poses;
			else return vector<std::string>();	//invalid cases
		}
		*/

		/*pos manipulation functions*/
		static string GetPos(const string& s)
		{
			string sp=s;
			return sp.erase(0, sp.find("("));
		}
		static vector<string> SplitPos(const string& pos)
		{
			string posp=pos.substr(1, pos.length()-2);	//erase '(' and ')'
			return SplitStr(posp, ',');			//erase ','
		}
		
		/*analysis functions*/
		static void SolveEqns(const bool& type)
		{
			if(type==true)	//condition
			{
				condslv=lsolve(eqncond, varcond);
				if(debug)
				{
					cout<<endl<<YELLOW<<"eqncond to solve: "<<RESET<<eqncond<<endl;
					cout<<YELLOW<<"varcond to solve: "<<RESET<<varcond<<endl;
					cout<<YELLOW<<"condslv: "<<RESET<<condslv<<endl;
				}
			}
			if(type==false)	//propsition
			{
				if(eqnprop.nops()==varprop.nops())
					propslv=lsolve(eqnprop, varprop);
				else
					propslv=eqnprop;
				
				if(debug)
				{
					cout<<endl<<YELLOW<<"eqnprop to solve: "<<RESET<<eqnprop<<endl;
					cout<<YELLOW<<"varprop to solve: "<<RESET<<varprop<<endl;
					cout<<YELLOW<<"propslv: "<<RESET<<propslv<<endl;
				}
				/*QED!*/
				if(SubsProp(condslv, propslv))
				{
					if(debug)
						cout<<endl<<GREEN<<"Qed!"<<RESET<<endl<<endl;
					else
						cout<<GREEN<<"True"<<RESET<<endl;
					cout<<BLUE<<"Note:"<<RESET<<endl;
					varp.print();
					cout<<BLUE<<"Generic condition:"<<RESET<<endl;
					cout<<Ds<<endl;
				}
				else
				{
					cout<<RED<<"False"<<RESET<<endl;
				}
			}
		}
		
		/*Due to the shortcomings of this program, this function is crucial.*/
		/*Establish a system in a plane and set some points artificially.*/
		static void EstablishSystem(string esstr)	/*(oOxOADyOBE)*/
		{
			is_es=true;
			for(size_t i=0; i<esstr.size(); i++)
			{
				if(esstr[i]==' '||esstr[i]=='('||esstr[i]==')')continue;
				if(esstr[i]=='o')
				{
					string ostr=StrSegment(esstr, 'o', "oxy)");
					for(size_t j=0; j<ostr.size(); j++)
						varp.modifyp(Point(0,0), esstr[i+j+1]);
				}
				if(esstr[i]=='x')
				{
					string xstr=StrSegment(esstr, 'x', "oxy)");
					for(size_t j=0; j<xstr.size(); j++)
						varp.modifyp(Point(varp.getp(esstr[i+j+1]).x,0), esstr[i+j+1]);
				}
				if(esstr[i]=='y')
				{
					string ystr=StrSegment(esstr, 'y', "oxy)");
					for(size_t j=0; j<ystr.size(); j++)
						varp.modifyp(Point(0,varp.getp(esstr[i+j+1]).y), esstr[i+j+1]);
				}
			}
			//if(debug)varp.print();
		}
		
		static void AnalPoint(const string& anal, const bool& type)	//(fABD,baCExy) "f,baxy)"
		{
			for(size_t i=0; i<anal.size(); i++)
			{
				if(anal[i]==' '||anal[i]=='('||anal[i]==')'||anal[i]==',')continue;
				if(anal[i]=='f')
				{
					string fp=StrSegment(anal, 'f', ",baxy)");	//fp: ABD
					for(size_t j=0; j<fp.size(); j++)
					{
						if('A'<=fp[j]&&fp[j]<='Z')
						{
							Point fpoint(HilbertSt::st1());
							varp.append(fpoint, true, true, fp[j]);
						}
						else continue;
					}
				}
				if(anal[i]=='b')					//bp: aCExy
				{
					string bp=StrSegment(anal, 'b', "f,)");
					string bpx=StrSegment(bp, 'x', "axy");
					string bpy=StrSegment(bp, 'y', "axy");
					string bpa=StrSegment(bp, 'a', "axy");
					for(size_t j=0; j<bpa.size(); j++)
					{
						if('A'<=bpa[j]&&bpa[j]<='Z')
						{
							Point bpoint(HilbertSt::st1());
							varp.append(bpoint, false, false, bpa[j]);
						}
						else continue;
					}
					for(size_t j=0; j<bpx.size(); j++)
					{
						if('A'<=bpx[j]&&bpx[j]<='Z')
						{
							Point bpoint(HilbertSt::st1());
							varp.append(bpoint, false, true, bpx[j]);
						}
						else continue;
					}
					for(size_t j=0; j<bpy.size(); j++)
					{
						if('A'<=bpy[j]&&bpy[j]<='Z')
						{
							Point bpoint(HilbertSt::st1());
							varp.append(bpoint, true, false, bpy[j]);
						}
						else continue;
					}
				}
			}
			if(type) append(varcond, varp.boundvars());	//condition
			else append(varprop, varp.boundvars());		//proposition
			return;
		}
		
		static void AnalFunc(const string& anal, const string& func, const bool& type)
		{
			if(func=="intersect"||func=="st8")
			{
				if(!type)
				{
					string err="AnalFunc: the proposition "+func+anal+" is invalid\n";
					throw runtime_error(err.c_str());
				}
				vector<string> poses=SplitPos(anal);						//3 parts(AC BD E)
				Point 	tmp00(varp.getp(poses[0][0])), 
					tmp01(varp.getp(poses[0][1])),
					tmp10(varp.getp(poses[1][0])),
					tmp11(varp.getp(poses[1][1]));
				HilbertSt::GeoEx ge=HilbertSt::st8(Line(tmp00, tmp01), Line(tmp10, tmp11), varp.getp(poses[2][0]));	//with p, e, D, v
				ex isslv;
				if(is_cov)
				{
					ge.e=ge.e.subs(cov);		//change the variable
					isslv=lsolve(ge.e, ge.v);	//solve the equations
					append(eqncond, isslv);
					is_cov=false;
					cov=lst();
				}
				else
				{
					ex slv_pre=lsolve(eqncond, varcond);
					cout<<"slv_pre: "<<slv_pre<<endl;
					isslv=lsolve(ge.e, ge.v);	//solve the equations to change the variables
					cout<<"isslv: "<<isslv<<endl;
					isslv=isslv.subs(slv_pre);
					cout<<"after subs isslv: "<<isslv<<endl;
					is_cov=true;
					append(cov, isslv);
				}
				if(debug)
				{
					cout<<"intersect eqns: "<<ge.e<<endl;
					cout<<"intersect vars: "<<ge.v<<endl;
					cout<<"isslv:"<<isslv<<endl;
				}
				append(Ds, ge.D);
					
			}
			if(func=="st9")	//Point Line Line ->Point
			{
				if(!type)
				{
					string err="AnalFunc: the proposition "+func+anal+" is invalid\n";
					throw runtime_error(err.c_str());
				}
				vector<string> poses=SplitPos(anal);
				Point 	tmp00(varp.getp(poses[0][0])), 
					tmp10(varp.getp(poses[1][0])),
					tmp11(varp.getp(poses[1][1])),
					tmp20(varp.getp(poses[2][0])),
					tmp21(varp.getp(poses[2][1]));
				HilbertSt::GeoEx ge=HilbertSt::st9(tmp00, Line(tmp10, tmp11), Line(tmp20, tmp21), varp.getp(poses[3][0]));
				ex st9slv;
				if(is_cov)
				{
					ge.e=ge.e.subs(cov);
					st9slv=lsolve(ge.e, ge.v);
					append(eqncond, st9slv);
					is_cov=false;
					cov=lst();
				}
				else
				{
					ex slv_pre=lsolve(eqncond, varcond);
					st9slv=lsolve(ge.e, ge.v);
					st9slv=st9slv.subs(slv_pre);
					is_cov=true;
					append(cov, st9slv);
				}
				append(Ds, ge.D);
				
			}
			if(func=="st10")//Point Point Line Line ->Point
			{
				if(!type)
				{
					string err="AnalFunc: the proposition "+func+anal+" is invalid\n";
					throw runtime_error(err.c_str());
				}
				vector<string> poses=SplitPos(anal);
				Point 	tmp00(varp.getp(poses[0][0])), 
					tmp10(varp.getp(poses[1][0])),
					tmp20(varp.getp(poses[2][0])),
					tmp21(varp.getp(poses[2][1])),
					tmp30(varp.getp(poses[3][0])),
					tmp31(varp.getp(poses[3][1]));
				HilbertSt::GeoEx ge=HilbertSt::st10(tmp00, tmp10, Line(tmp20, tmp21), Line(tmp30, tmp31), varp.getp(poses[4][0]));
				ex st10slv;
				if(is_cov)
				{
					ge.e=ge.e.subs(cov);
					st10slv=lsolve(ge.e, ge.v);
					append(eqncond, st10slv);
					is_cov=false;
					cov=lst();
				}
				else
				{
					ex slv_pre=lsolve(eqncond, varcond);
					st10slv=lsolve(ge.e, ge.v);
					st10slv=st10slv.subs(slv_pre);
					is_cov=true;
					append(cov, st10slv);
				}
				append(Ds, ge.D);
			}
			if(func=="parallel")
			{
				vector<string> poses=SplitPos(anal);						//2 parts(AB CD)
				Point 	tmp00(varp.getp(poses[0][0])), 
					tmp01(varp.getp(poses[0][1])),
					tmp10(varp.getp(poses[1][0])),
					tmp11(varp.getp(poses[1][1]));
				ex eqn_prl=parallel(tmp00, tmp01, tmp10, tmp11);
				if(debug) cout<<"parallel eqn: "<<eqn_prl<<endl;
				if(DetermineExpr(eqn_prl)==false&&type==false)
				{
					if(debug)
						cout<<"bound:"<<eqn_prl<<endl;
					return;
				}
				if(type==true)
					append(eqncond, eqn_prl);
				else	
					append(eqnprop, eqn_prl);
			}
			if(func=="midpoint")									//2 parts(E AC)
			{
				vector<string> poses=SplitPos(anal);
				Point 	tmp00(varp.getp(poses[0][0])), 
					tmp10(varp.getp(poses[1][0])),
					tmp11(varp.getp(poses[1][1]));
				ex eqn_mp=Midpoint(tmp00, tmp10, tmp11);
				if(debug) cout<<"midpoint eqns: "<<eqn_mp<<endl;
				if(DetermineExpr(eqn_mp)==false&&type==false)
				{
					if(debug)
						cout<<"bound:"<<eqn_mp<<endl;
					return;
				}
				if(type==true)
					append(eqncond, eqn_mp);
				else
					append(eqnprop, eqn_mp);
			}
			if(func=="collinear")
			{
				vector<string> poses=SplitPos(anal);
				Point 	tmp0(varp.getp(poses[0][0])), 
					tmp1(varp.getp(poses[1][0])),
					tmp2(varp.getp(poses[2][0]));
				ex eqn_cl=collinear(tmp0, tmp1, tmp2);
				if(debug) cout<<"collinear eqn: "<<eqn_cl<<endl;
				if(DetermineExpr(eqn_cl)==false&&type==false)
				{
					if(debug)
						cout<<"bound:"<<eqn_cl<<endl;
					return;
				}
				if(type==true)
					append(eqncond, eqn_cl);
				else
					append(eqnprop, eqn_cl);
			}
		}
		
		/*type: condition  ->true  *
		 *	proposition->false */
		static void AnalysisStr(const string& anal, const bool& type)	//first
		{
			vector<string> strs=SplitStr(anal, ' ');
			for(size_t i=0; i<strs.size(); i++)
			{
				string func=(SplitStr(strs[i], '('))[0];
				if(func=="Point")
					AnalPoint(strs[i].erase(0, strs[i].find("Point")+5), type);		//the argument is like (fABD,bCE)
				else if(func=="EstSys")
					EstablishSystem(strs[i].erase(0, strs[i].find("EstSys")+6));
				else
					AnalFunc(GetPos(strs[i]), func, type);
			}
			SolveEqns(type);
		}
		Analysis(){}
		~Analysis(){}
	};
};

#endif
