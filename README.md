# Wizard
A geometry prover using Hilbert's axioms , Wu's method and GiNaC library.

## Tutorial
### Types
Point  
Line  
Segment  
GeoEx  
### Functions
ex parallel(const Segment&, const Segment&)  
ex parallel(const Point&, const Point&, const Point&, const Point&)  
ex perpendicular(const Segment&, const Segment&)  
ex perpendicular(const Point&, const Point&, const Point&, const Point&)  
ex collinear(const Point&, const Segment&)  
ex collinear(const Point&, const Point&, const Point&)  
bool order(const Point&, const Point&, const Point&)  
lst SectionPoint(const Point&, const Segment&, const ex&, bool type)  
lst Midpoint(const Point&, const Segment&)  
lst Midpoint(const Point&, const Point&, const Point&)  
bool EquivalentProp(const lst&, const lst&)  
bool SubsProp(const lst&, const ex&)  
void append(lst& l1, const ex& l2)  
### HilbertSteps
Point st1(): select an arbitrary point  
Line st2(): select an arbitrary line  
Line st3(const Point&): draw a line passing through a point  
Line st4(const Point&, const Point&): draw a line passing through two points  
GeoEx st5(const Line&): select an arbitrary point on the given line  
GeoEx st6(const Line&): draw a line parallel to the given line  
GeoEx st7(const Line&, const Point&): draw a line parallel to the given line passing through a point  
GeoEx st8(const Line&, const Line&): draw the intersection point of two intersecting lines  
GeoEx st9(const Point&, const Line&, const Line&): draw a line parallel to a given line passing through a point, and intersect it with another line to find the intersection point  
GeoEx st10(const Point&, const Point&, const Line&, const Line&): draw two intersecting parallel lines passing through each of the two given points to find the intersection point  
