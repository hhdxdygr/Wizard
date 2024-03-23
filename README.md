# Wizard
A geometry prover using Hilbert's axioms , Wu's method and GiNaC library.

## Tutorial
### Compile
Install CLN and GiNaC:  
[CLN](https://www.ginac.de/CLN/) [GiNaC](https://www.ginac.de/Download.html)  
Compile CLN&GiNaC:  
[Installation](https://www.ginac.de/tutorial/#Installation)  
Compile Wizard:  
`g++ main.cpp -o Wizard -lginac -lcln`  

### Usage
./Wizard --condition <condition> --proposition <proposition>  
Point(fAB,baCxDEyFGH) means that you let Wizard create 8 points, A and B are free points, C is bound point(x&y are bound), D and E are x-bound(D.x and E.x are bound while D.y and E.y are free), F, G and H are y-bound(F.x, G.x and H.x are free while F.y, G.y and H.y are bound).  

You can also establish a system by the function EstSys.  
EstSys(oOxOADyOBE) means that you designate point O as the origin, with points O, B, and D located on the x-axis, and points O, C, and E located on the y-axis.  

st_x means Hilbert steps(see [APIs](./#APIs))  
parallel, collinear and midpoint are supported(see [APIs](./#APIs))  

#### Example
test1: ./Wizard --condition "Point(fABD,baCExy) st10(B,D,AD,AB,C) intersect(AC,BD,E)" --proposition "midpoint(E,AC) midpoint(E,BD)"  
test2: ./Wizard --condition "Point(fADBCO,baFxyE) EstSys(oOxOADyOBE) parallel(AB,DE) parallel(AC,DF) parallel(BC,EF)" --proposition "collinear(O,C,F)"  

### APIs
- Types
	- Point  
	- Line  
	- Segment  
	- GeoEx  
- Functions
	- ex parallel(const Segment&, const Segment&)  
	- ex parallel(const Point&, const Point&, const Point&, const Point&)  
	- ex perpendicular(const Segment&, const Segment&)  
	- ex perpendicular(const Point&, const Point&, const Point&, const Point&)  
	- ex collinear(const Point&, const Segment&)  
	- ex collinear(const Point&, const Point&, const Point&)  
	- bool order(const Point&, const Point&, const Point&)  
	- lst SectionPoint(const Point&, const Segment&, const ex&, bool type)  
	- lst Midpoint(const Point&, const Segment&)  
	- lst Midpoint(const Point&, const Point&, const Point&)  
	- bool EquivalentProp(const lst&, const lst&)  
	- bool SubsProp(const lst&, const ex&)  
	- void append(lst& l1, const ex& l2)  
- HilbertSteps
	- Point **st1**(): select an arbitrary point  
	- Line **st2**(): select an arbitrary line  
	- Line **st3**(const Point&): draw a line passing through a point  
	- Line **st4**(const Point&, const Point&): draw a line passing through two points  
	- GeoEx **st5**(const Line&): select an arbitrary point on the given line  
	- GeoEx **st6**(const Line&): draw a line parallel to the given line  
	- GeoEx **st7**(const Line&, const Point&): draw a line parallel to the given line passing through a point  
	- GeoEx **st8**(const Line&, const Line&): draw the intersection point of two intersecting lines  
	- GeoEx **st9**(const Point&, const Line&, const Line&): draw a line parallel to a given line passing through a point, and intersect it with another line to find the intersection point  
	- GeoEx **st10**(const Point&, const Point&, const Line&, const Line&): draw two intersecting parallel lines passing through each of the two given points to find the intersection point  

### Note
There are some TODOs yet. See main.cpp for more details.  
