// https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Point { double x; double y; };
struct Segment { Point start; Point end; };
// Points in CCW order. Implicit edge between first and last point.
using Polygon = std::vector<Point>;
using Vec = Point;
struct Box { Point bl; Point tr; };

std::string PointStr(const Point& p) {
  std::stringstream ss;
  ss.precision(2);
  ss << "(" << p.x << " " << p.y << ")";
  return ss.str();
}

std::string SegmentStr(const Segment& seg) {
  return "Seg(" + PointStr(seg.start) + " " + PointStr(seg.end) + ")";
}

std::string BoxStr(const Box& box) {
  return "Box(" + PointStr(box.bl) + " " + PointStr(box.tr) + ")";
}

std::string PolygonStr(const Polygon& polygon) {
  std::string s = "Polygon(";
  for (size_t i = 0; i < polygon.size(); ++i) {
    s += PointStr(polygon[i]);
    if (i < polygon.size() - 1) {
      s += " ";
    }
  }
  return s + ")";
}

Point Seg2Vec(Segment seg) {
  Point vec = {seg.end.x - seg.start.x, seg.end.y - seg.start.y};
  return vec;
}

double crossProduct(Point v1, Point v2) {
  return (v1.x*v2.y - v1.y*v2.x);
}

bool inside(Point poly, Segment edge) {
  Point vec_edge = Seg2Vec(edge);
  Segment edge2poly = {edge.start, poly}; // ?? which points?
  Point vec_poly_edge = Seg2Vec(edge2poly);
  double cp = crossProduct(vec_edge, vec_poly_edge);
  if (cp < 0) {
    return false; //outside
  }
  else if (cp >= 0) {
    return true; // inside;
  }
}

// Point computeIntersection(Segment poly_edge, Segment edge) {
//   double dx = (poly_edge.start.x - poly_edge.end.x);
//   double dy = (poly_edge.start.y - poly_edge.end.y);
//   double Y = 0;
//   double X = 0;
//   if (dx == 0) { // vertical line
//     X = edge.start.x;
//     Y = poly_edge.start.y;
//   }
//   else if (dy == 0) { // horizontal line
//     X = poly_edge.start.x;
//     Y = edge.start.y;
//   }
//   else {
//     double m = dy / dx;
//     Y = m * (edge.start.x - poly_edge.start.x) + poly_edge.start.y;
//     X = 1.0/m * (edge.start.y - poly_edge.start.y) + poly_edge.start.x;
//   }
  
//   Point P = {X, Y};
//   return P;
// }

Point computeIntersection(Segment poly_edge, Segment edge) {
  double dx = (poly_edge.start.x - poly_edge.end.x);
  double dy = (poly_edge.start.y - poly_edge.end.y);
  double m = dy / dx;
  double Y = m * (edge.start.x - poly_edge.start.x) + poly_edge.start.y;
  double X = 1.0/m * (edge.start.y - poly_edge.start.y) + poly_edge.start.x;
  if (dx == 0) {
    Y = edge.start.y;
  }
  if (dy == 0) {
    X = edge.start.x;
  }
  Point P = {X, Y};
  return P;
}

Polygon ClipPolygon(Polygon subj, const Box& clip) {

  Polygon out = subj;
  Point BL = {clip.bl.x, clip.bl.y};
  Point TL = {clip.bl.x, clip.tr.y};
  Point TR = {clip.tr.x, clip.tr.y};
  Point BR = {clip.tr.x, clip.bl.y};
  Polygon BoxPoints = {BL, BR, TR, TL};
  // Polygon BoxPoints = {BL, TL, TR, BR};

  for (int c_v=0; c_v<4; c_v++) {
    Segment edge = {BoxPoints[c_v], BoxPoints[(c_v+1)%4]};
    std::cout << "Box Edge: " << SegmentStr(edge) << std::endl;
    
    Polygon in = out;
    out.clear();
    Point S = in.back();

    for (auto E: in) {
      Segment poly_edge = {S, E};
      std::cout << "Poly Point: " << PointStr(E) << "-->";


      if (inside(S, edge)) {
        // std::cout << "Starting Point Inside!" << std::endl;
        if (inside(E, edge)) { // start and end inside -- keep point
          out.push_back(E);
          std::cout << PointStr(E) << std::endl;
        }
        else { // start inside and end outside -- keep point along line
          Point P_ix = computeIntersection(poly_edge, edge);
          out.push_back(P_ix);
          std::cout << PointStr(P_ix) << std::endl;
        }
      }
      else {
        // std::cout << "Starting Point Outisde!" << std::endl;
        if (inside(E,edge)) { // start outside and end inside -- keep intersection and end point
          Point P_ix = computeIntersection(poly_edge, edge);
          out.push_back(P_ix);
          out.push_back(E);
          std::cout << PointStr(P_ix) << ", " << PointStr(E) << std::endl;
        }
        else { // start outside and end outside -- Do nothing
          std::cout << std::endl;
        }
      }
    }
    std::cout << "Current Polygon: " << PolygonStr(out) << std::endl;
  }
  return out;
}

class NoClippingTest {
public:
  void static printTestName() {
    std::cout << "No Clipping Test" << std::endl;
  }

  Polygon static get_polygon() {
    Point P1 = {-0.5, -0.5};
    Point P2 = { 0.5, -0.5};
    Point P3 = { 0.5,  0.5};
    Point P4 = {-0.5,  0.5};
    Polygon P = {P1, P2, P3, P4};
    return P;
  }

  Box static get_box() {
    Point BL = {-1, -1};
    Point TR = {1, 1};
    Box B = {BL, TR};
    return B;
  }

  Polygon static get_clipped_polygon() {
    Point P1 = {-0.5, -0.5};
    Point P2 = { 0.5, -0.5};
    Point P3 = { 0.5,  0.5};
    Point P4 = {-0.5,  0.5};
    Polygon P = {P1, P2, P3, P4};
    return P;
  }
};

class AllClippingTest {
public:
  void static printTestName() {
    std::cout << "All Clipping Test" << std::endl;
  }

  Polygon static get_polygon() {
    Point P1 = {-1.5, -1.5};
    Point P2 = { 1.5, -1.5};
    Point P3 = { 1.5,  1.5};
    Point P4 = {-1.5,  1.5};
    Polygon P = {P1, P2, P3, P4};
    return P;
  }

  Box static get_box() {
    Point BL = {-1, -1};
    Point TR = {1, 1};
    Box B = {BL, TR};
    return B;
  }

  Polygon static get_clipped_polygon() {
    Point P1 = {-1.0, -1.0};
    Point P2 = { 1.0, -1.0};
    Point P3 = { 1.0,  1.0};
    Point P4 = {-1.0,  1.0};
    Polygon P = {P1, P2, P3, P4};
    return P;
  }
};

class DiamondClippingTest {
public:
  void static printTestName() {
    std::cout << "Diamond Test" << std::endl;
  }

  Polygon static get_polygon() {
    Point P1 = {-1.5,  0.0};
    Point P2 = { 0.0, -1.5};
    Point P3 = { 1.5,  0.0};
    Point P4 = { 0.0,  1.5};
    Polygon P = {P1, P2, P3, P4};
    return P;
  }

  Box static get_box() {
    Point BL = {-1, -1};
    Point TR = {1, 1};
    Box B = {BL, TR};
    return B;
  }

  Polygon static get_clipped_polygon() {
    Point P1 = {-1.0, -0.5};
    Point P2 = {-0.5, -1.0};
    Point P3 = { 0.5, -1.0};
    Point P4 = { 1.0, -0.5};
    Point P5 = { 1.0,  0.5};
    Point P6 = {-0.5,  1.0};
    Point P7 = { 0.5,  1.0};
    Point P8 = {-1.0,  0.5};
    Polygon P = {P1, P2, P3, P4, P5, P6, P7, P8};
    return P;
  }
};

void NoClipping() {
  NoClippingTest::printTestName();
  Polygon P = NoClippingTest::get_polygon();
  std::cout << "Original Polygon: " << PolygonStr(P) << std::endl;
  Box B = NoClippingTest::get_box();
  std::cout << "Clipping Box" << BoxStr(B) << std::endl;
  Polygon P_clipped = ClipPolygon(P, B);
  std::cout << "Clipped Polygon: " << PolygonStr(P) << std::endl;
  std::cout << "Expected Polygon: " << PolygonStr(NoClippingTest::get_clipped_polygon()) << std::endl;
}

void AllClipping() {
  AllClippingTest::printTestName();
  Polygon P = AllClippingTest::get_polygon();
  std::cout << "Original Polygon: " << PolygonStr(P) << std::endl;
  Box B = AllClippingTest::get_box();
  std::cout << "Clipping Box" << BoxStr(B) << std::endl;
  Polygon P_clipped = ClipPolygon(P, B);
  std::cout << "Clipped Polygon: " << PolygonStr(P_clipped) << std::endl;
  std::cout << "Expected Polygon: " << PolygonStr(AllClippingTest::get_clipped_polygon()) << std::endl;
}

void DiamondClipping() {
  DiamondClippingTest::printTestName();
  Polygon P = DiamondClippingTest::get_polygon();
  std::cout << "Original Polygon: " << PolygonStr(P) << std::endl;
  Box B = DiamondClippingTest::get_box();
  std::cout << "Clipping Box" << BoxStr(B) << std::endl;
  Polygon P_clipped = ClipPolygon(P, B);
  std::cout << "Clipped Polygon: " << PolygonStr(P_clipped) << std::endl;
  std::cout << "Expected Polygon: " << PolygonStr(DiamondClippingTest::get_clipped_polygon()) << std::endl;
}

// make a general test class with virtual functions
// in each test function, allow for a single value to be overwritten
int main() {
  // NoClipping();
  AllClipping();
  // DiamondClipping();
  return 0;
}
