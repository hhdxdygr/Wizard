/*Cf. 《几何定理机器证明的基本原理》，吴文俊，科学出版社，1984年*/
/*https://zh.z-library.se/book/11810820/2f0956/%E5%87%A0%E4%BD%95%E5%AE%9A%E7%90%86%E6%9C%BA%E5%99%A8%E8%AF%81%E6%98%8E%E7%9A%84%E5%9F%BA%E6%9C%AC%E5%8E%9F%E7%90%86.html*/
/*HilbertSt类内函数定义见3.5节*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>

#include <ginac/ginac.h>

using namespace std;
using namespace GiNaC;

namespace Wizard
{
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
		void print()
		{
			cout<<"("<<x<<","<<y<<")"<<endl;
			return;
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
		ex x1, x2, y1, y2;
	};
	/*Definition of Segment*/
	//Seems this is not very useful.
	class Segment
	{
	public:
		Segment(){}
		~Segment(){}
		Segment(const Point& _p1, const Point& _p2)
		{
			x1=_p1.x, x2=_p2.x, y1=_p1.y, y2=_p2.y;
		}
		ex x1, y1, x2, y2;
	};
/*Now we define the four relations of points and line segments.*/
	
	/*parallel line segments:	A-------B	*
	 *				C-------D	*/
	GiNaC::ex parallel(const Segment& _s1, const Segment& _s2)
	{
		return ((_s1.y1-_s1.y2)*(_s2.x1-_s2.x2)-(_s2.y1-_s2.y2)*(_s1.x1-_s1.x2)==0);
	}
	/*the segment p1p2 is parallel to the segment p3p4*/
	GiNaC::ex parallel(const Point& _p1, const Point& _p2, const Point& _p3, const Point& _p4)
	{
		return parallel(Segment(_p1, _p2), Segment(_p3, _p4));
	}
	/*perpendicular line segments:	A----+--B	*
	 *				    C|		*
	 *			 	     |		*
	 *				    D|		*/
	GiNaC::ex perpendicular(const Segment& _s1, const Segment& _s2/*, const int& _k=1*/)
	{
		return ((_s1.y1-_s1.y2)*(_s2.y1-_s2.y2)+(_s1.x1-_s1.x2)*(_s2.x1-_s2.x2)==0);
	}
	/*the segment p1p2 is perpendicular to the segment p3p4*/
	GiNaC::ex perpendicular(const Point& _p1, const Point& _p2, const Point& _p3, const Point& _p4)
	{
		return perpendicular(Segment(_p1, _p2), Segment(_p3, _p4));
	}
	/*a point is on a line segment(three points are collinear):	A----C-------B	*/
	GiNaC::ex collinear(const Point& _p, const Segment& _s)
	{
		return (determinant(matrix{{_p.x,	_p.y,	1},
				 				   {_s.x1, _s.y1,	1},
				 				   {_s.x2, _s.y2,	1}})==0);
	}
	GiNaC::ex collinear(const Point& _p1, const Point& _p2, const Point& _p3)
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
	GiNaC::lst SectionPoint(	const Point& _p, const Segment& _s,	//type:	false->internal 
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
	GiNaC::lst Midpoint(const Point& _p, const Segment& _s)
	{
		return 	 lst{_p.x==(_s.x1+_s.x2)/2,
					 _p.y==(_s.y1+_s.y2)/2};
	}
	/*p1 is the midpoint of the segment p2p3*/
	GiNaC::lst Midpoint(const Point& _p1, const Point& _p2, const Point& _p3)
	{
		return Midpoint(_p1, Segment(_p2, _p3));
	}
	/*judge if the two propositions are equivalent*/
	bool EquivalentProp(const lst& prop1, const lst& prop2)
	{
		for(lst::const_iterator i=prop1.begin(); i!=prop1.end(); ++i)
        	if(!prop2.has(*i))return false;
		for(lst::const_iterator i=prop2.begin(); i!=prop2.end(); ++i)
        	if(!prop1.has(*i))return false;
        	return true;
	}
	/*substitute slv into prop_eqn and judge*/
	bool SubsProp(const lst& slv, const ex& prop_eqn)
	{
		ex prv=prop_eqn.subs(slv);
		relational rel=ex_to<relational>(prv);
		if(ex_to<relational>(rel.canonical().normal()))
			return true;
		return false;
	}
	/*append ex to lst without '{' or '}'*/
	void append(lst& l1, const ex& l2)
	{
		for(int i=0;i<l2.nops();i++)
			l1.append(l2[i]);
	}
/************************************************************************************/
/********************************HILBERT STEPS***************************************/
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
			/*type: 10->Point;		20->Line;	30->Point&Line	*
			 *	11->Point&D;		21->Line&D;	31->Point&Line&D*/
			int type;
		};
		HilbertSt(){}
		~HilbertSt(){}
		/*steps which create new u_i; we don't care about step4.*/
		static Point st1() {symbol u1, u2; return Point(u1, u2);}
		static Line st2() {symbol u1, u2, u3, u4; return Line(Point(u1, u2), Point(u3, u4));}
		static Line st3(const Point& p) {symbol u1, u2; return Line(p, Point(u1, u2));}
		static Line st4(const Point p1, const Point p2) {return Line(p1, p2);}
		/*steps which return a equation*/
		static GeoEx st5(const Line& l) {symbol u1, x1; return GeoEx(Point(u1, x1), determinant(matrix{{u1, x1, 1}, {l.x1, l.y1, 1}, {l.x2, l.y2, 1}}), lst{x1});}
		static GeoEx st6(const Line& l) {symbol u1, u2, u3, x1; return GeoEx(Line(Point(u1, u2), Point(u3, x1)), ex((l.x2-l.x1)*(x1-u2)-(l.y2-l.y1)*(u3-u1)==0), lst{x1});}
		static GeoEx st7(const Line& l, const Point& p) {symbol u1, x1; return GeoEx(Point(u1, x1), Line(Point(u1, x1), p), ex((l.x2-l.x1)*(x1-p.y)-(l.y2-l.y1)*(u1-p.x)==0), lst{x1});}
		/*steps which return two equations*/
		static GeoEx st8(const Line& l1, const Line& l2) 	
		{
			symbol x1, x2;
			return GeoEx(Point(x1, x2), lst{((l1.y1-l1.y2)*x1-(l1.x1-l1.x2)*x2+l1.x1*l1.y2-l1.x2*l1.y1==0), 
							((l2.y1-l2.y2)*x1-(l2.x1-l2.x2)*x2+l2.x1*l2.y2-l2.x2*l2.y1==0)},
				     ex(determinant(matrix{{l1.y1-l2.y2, l1.x2-l1.x1}, {l2.y1-l2.y2, l2.x2-l2.x1}})!=0), lst{x1, x2});
		}
		static GeoEx st9(const Point& p, const Line& l1, const Line& l2)
		{
			symbol x1, x2;
			return GeoEx(Point(x1, x2), lst{((l1.x2-l1.x1)*(x2-p.y)-(l1.y2-l1.y1)*(x1-p.x)==0),
							((l2.y1-l2.y2)*x1-(l2.x1-l2.x2)*x2+l2.x1*l2.y2-l2.x2*l2.y1==0)},
				     ex(determinant(matrix{{l1.y2-l1.y1, l1.x1-l1.x2}, {l2.y1-l2.y2, l2.x2-l2.x1}})!=0), lst{x1, x2});
		}
		static GeoEx st10(const Point& p1, const Point& p2, const Line& l1, const Line& l2) 
		{
			symbol x1, x2;
			return GeoEx(Point(x1, x2), lst{((p1.y-x2)*(l1.x2-l1.x1)-(l1.y2-l1.y1)*(p1.x-x1)==0),
							((p2.y-x2)*(l2.x2-l2.x1)-(l2.y2-l2.y1)*(p2.x-x1)==0)},
				     ex(determinant(matrix{{l1.y2-l1.y1, l1.x1-l1.x2}, {l2.y2-l2.y1, l2.x1-l2.x2}})!=0), lst{x1, x2});
		}
	};
/*****************************HILBERT STEPS END**************************************/
/************************************************************************************/
};
