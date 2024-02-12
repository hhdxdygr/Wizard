#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>

#include <ginac/ginac.h>

using namespace std;
using namespace GiNaC;
DECLARE_FUNCTION_2P(root)
#define ERR_01 "INVALID_ARGUMENT"
class Geo
{
public:
	/*Definition of Variable*/
	class Variable
	{
	public:
		static const int max_sym=100;
		symbol s[max_sym]={};
		int i=0;
		Variable(){}
		void printvars()
		{
			for(int vi=0; vi<i; vi++)
				cout<<s[vi]<<endl;
			return;
		}
		template<typename V>
		Variable(const V& var1, const V& ux)
		{
			s[i++]=var1;
		}
		template<typename V>
		void append(const V& var1)
		{
			s[i++]=var1;
		}
		template<typename V, typename... Vars>
		Variable(const V& var1, Vars... vars)
		{
			s[i++]=var1;
			append(vars...);
		}
		template<typename V, typename... Vars>
		void append(const V& var1, Vars... vars)
		{
			s[i++]=var1;
			append(vars...);
		}
	};
	/*variable "var" stores all the variables the program makes.*/
	static Variable var;
	/*Definition of Point*/
	class Point
	{
	public:
		Point(){}
		Point(const ex& _x, const ex& _y)
		{
			x=_x, y=_y;
		}
		Point(const Point& _p)
		{
			x=_p.x, y=_p.y;
		}
		ex x, y;
	};
	/*Definition of Line*/
	class Line
	{
	public:
		Line(){}
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
	class Segment
	{
	public:
		Segment(){}
		Segment(const Point& _p1, const Point& _p2)
		{
			x1=_p1.x, x2=_p2.x, y1=_p1.y, y2=_p2.y;
		}
		ex x1, y1, x2, y2;
	};
/*Now we define the four relations of points and line segments.*/
	
	/*parallel line segments:	A-------B	*
	 *							C-------D	*/
	static GiNaC::ex parallel(const Segment& _s1, const Segment& _s2)
	{
		return ((_s1.y1-_s1.y2)*(_s2.x1-_s2.x2)-(_s2.y1-_s2.y2)*(_s1.x1-_s1.x2)==0);
	}
	/*the segment p1p2 is parallel to the segment p3p4*/
	static GiNaC::ex parallel(const Point& _p1, const Point& _p2, const Point& _p3, const Point& _p4)
	{
		return parallel(Segment(_p1, _p2), Segment(_p3, _p4));
	}
	/*perpendicular line segments:	A----+--B	*
	 *									C|		*
	 *								 	 |		*
	 *									D|		*/
	static GiNaC::ex perpendicular(const Segment& _s1, const Segment& _s2/*, const int& _k=1*/)
	{
		return ((_s1.y1-_s1.y2)*(_s2.y1-_s2.y2)+(_s1.x1-_s1.x2)*(_s2.x1-_s2.x2)==0);
	}
	/*the segment p1p2 is perpendicular to the segment p3p4*/
	static GiNaC::ex perpendicular(const Point& _p1, const Point& _p2, const Point& _p3, const Point& _p4)
	{
		return perpendicular(Segment(_p1, _p2), Segment(_p3, _p4));
	}
	/*a point is on a line segment(three points are collinear):	A----C-------B	*/
	static GiNaC::ex collinear(const Point& _p, const Segment& _s)
	{
		return (determinant(matrix{{_p.x,	_p.y,	1},
				 				   {_s.x1, _s.y1,	1},
				 				   {_s.x2, _s.y2,	1}})==0);
	}
	static GiNaC::ex collinear(const Point& _p1, const Point& _p2, const Point& _p3)
	{
		return collinear(_p1, Segment(_p2, _p3));
	}
	/*the relation of the order of points*/
	static bool order(const Point& _p, const Point& _p1, const Point& _p2)
	{
		return 	(_p1.x<_p.x&&_p.x<_p2.x)||
				(_p2.x<_p.x&&_p.x<_p1.x)||
				(_p1.y<_p.y&&_p.y<_p2.y)||
				(_p2.y<_p.y&&_p.y<_p1.y);
	}
	
	
	/*a section point:	(internal): 	A-----C----------B		*
	 *		  			(external):		A---------B------C		*
	 * We define section=AC/BC.(A(_s.x1,_s.y1), B(_s.x2,_s.y2))	*/
	static GiNaC::lst SectionPoint(	const Point& _p, const Segment& _s,	//type:	false->internal 
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
	static GiNaC::lst Midpoint(const Point& _p, const Segment& _s)
	{
		return 	 lst{_p.x==(_s.x1+_s.x2)/2,
					 _p.y==(_s.y1+_s.y2)/2};
	}
	/*p1 is the midpoint of the segment p2p3*/
	static GiNaC::lst Midpoint(const Point& _p1, const Point& _p2, const Point& _p3)
	{
		return Midpoint(_p1, Segment(_p2, _p3));
	}
/************************************************************************************/
/********************************HILBERT STEPS***************************************/
	class HilbertSt
	{
	public:
		class GeoEx
		{
		public:
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
			Point p;
			Line l;
			ex e, D;
			lst v;
			/*type: 10->Point;	20->Line;	*
			 *		11->Point&D;21->Line&D;	*/
			int type;
		};
		HilbertSt(){}
		/*steps which create new u_i; we don't care about step4.*/
		static Point st1() {symbol u1, u2; Geo::var.append(u1, u2); return Point(u1, u2);}
		static Line st2() {symbol u1, u2, u3, u4; Geo::var.append(u1, u2, u3, u4); return Line(Point(u1, u2), Point(u3, u4));}
		static Line st3(const Point& p) {symbol u1, u2; Geo::var.append(u1, u2); return Line(p, Point(u1, u2));}
		//static Variable st4(Point p1, Point p2) {return Line(p1, p2);}
		/*steps which return a equation*/
		static GeoEx st5(const Line& l) {symbol u1, x1; Geo::var.append(u1, x1); return GeoEx(Point(u1, x1), determinant(matrix{{u1, x1, 1}, {l.x1, l.y1, 1}, {l.x2, l.y2, 1}}), lst{u1, x1});}
		static GeoEx st6(const Line& l) {symbol u1, u2, u3, x1; Geo::var.append(u1, u2, u3, x1); return GeoEx(Line(Point(u1, u2), Point(u3, x1)), ex((l.x2-l.x1)*(x1-u2)-(l.y2-l.y1)*(u3-u1)==0), lst{u1, u2, u3, x1});}
		static GeoEx st7(const Line& l, const Point& p) {symbol u1, x1; Geo::var.append(u1, x1); return GeoEx(Line(Point(u1, x1), p), ex((l.x2-l.x1)*(x1-p.y)-(l.y2-l.y1)*(u1-p.x)==0), lst{u1, x1});}
		/*steps which return two equations*/
		static GeoEx st8(const Line& l1, const Line& l2) 	
		{
			symbol x1, x2; Geo::var.append(x1, x2); 
			return GeoEx(Point(x1, x2), lst{((l1.y1-l1.y2)*x1-(l1.x1-l1.x2)*x2+l1.x1*l1.y2-l1.x2*l1.y1==0), 
											((l2.y1-l2.y2)*x1-(l2.x1-l2.x2)*x2+l2.x1*l2.y2-l2.x2*l2.y1==0)},
						 ex(determinant(matrix{{l1.y1-l2.y2, l1.x2-l1.x1}, {l2.y1-l2.y2, l2.x2-l2.x1}})!=0), lst{x1, x2});
		}
		static GeoEx st9(const Point& p, const Line& l1, const Line& l2)
		{
			symbol x1, x2; Geo::var.append(x1, x2); 
			return GeoEx(Point(x1, x2), lst{((l1.x2-l1.x1)*(x2-p.y)-(l1.y2-l1.y1)*(x1-p.x)==0),
											((l2.y1-l2.y2)*x1-(l2.x1-l2.x2)*x2+l2.x1*l2.y2-l2.x2*l2.y1==0)},
						 ex(determinant(matrix{{l1.y2-l1.y1, l1.x1-l1.x2}, {l2.y1-l2.y2, l2.x2-l2.x1}})!=0), lst{x1, x2});
		}
		static GeoEx st10(const Point& p1, const Point& p2, const Line& l1, const Line& l2) 
		{
			symbol x1, x2; Geo::var.append(x1, x2); 
			return GeoEx(Point(x1, x2), lst{((p1.y-x2)*(l1.x2-l1.x1)-(l1.y2-l1.y1)*(p1.x-x1)==0),
											((p2.y-x2)*(l2.x2-l2.x1)-(l2.y2-l2.y1)*(p2.x-x1)==0)},
						 ex(determinant(matrix{{l1.y2-l1.y1, l1.x1-l1.x2}, {l2.y2-l2.y1, l2.x1-l2.x2}})!=0), lst{x1, x2});
		}
	};
/*****************************HILBERT STEPS END**************************************/
/************************************************************************************/
	Geo()
	{
		
	}
};


//Root function definition
//Note: b&p should be nonnegative integers.
//The output is closed-form.
//To GeoCalc it's enough.

static ex _rt(const ex &b, const ex &p)
{
	//easy situations
	if(b==0)
		return 0;
	if(b<0)
		return root(b, p).hold();
	if(b==1)
		return 1;
	//define some constants and variables
	const int max_pr=std::pow((ex_to<numeric>(b)).to_double(), 0.5);
	const int max_pw=100;
	ex pop[max_pw+1]={0};			//powers of primes 
	ex bt=b, reti=1, retr=b;
	
	for(numeric i=2; i*i<=bt;)
	{
		if(i.is_prime())
			while(true)
			{
				numeric t=ex_to<numeric>(bt)/i;
				if(t.is_integer())
					pop[i.to_int()]++, bt/=i;
				else break;
			}
		if(i==2) i++;
		else i+=2;
	}
	
	for(int i=2; i<=(b>max_pw*max_pw?max_pw:max_pr); i++)
	{
		numeric q=ex_to<numeric>(pop[i]/p);
		if(pop[i]>0)
		{
			int tmp=floor(q.to_double());
			reti*=GiNaC::pow(i, tmp), retr/=GiNaC::pow(i, p*tmp);
		}
	}
	
	if(retr==1) return reti;
	return reti*root(retr, p).hold();
}

REGISTER_FUNCTION(root, eval_func(_rt))
