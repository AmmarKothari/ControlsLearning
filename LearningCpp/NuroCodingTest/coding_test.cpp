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

Point computeIntersection(Segment poly_edge, Segment edge) {
  double X, Y;
  double dx = (poly_edge.end.x - poly_edge.start.x);
  double dy = (poly_edge.end.y - poly_edge.start.y);
  double m = dy / dx;
  double b = poly_edge.start.y - m*poly_edge.start.x;
  // printf("dx: %f, dy: %f\n", dx, dy);
  if (dy == 0) { // horizontal polygon line
    // prevous check so know cutting line must be vertical
    Y = poly_edge.end.y;
    X = edge.end.x;

  }
  else if (dx == 0) { // vertical polygon line
    // prevous check so know cutting line must be horizontal
    Y = edge.end.y;
    X = poly_edge.end.x;
  }
  else {
    if ( (edge.start.y - edge.end.y) == 0) {// horizontal cutting line
      Y = edge.start.y;
    }
    else { // vertical cutting line
      Y = m*edge.start.x + b;  
    }
    X = (Y - b) / m;
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
      std::cout << "Poly Seg: " << SegmentStr(poly_edge) << "-->";


      if (inside(S, edge)) {
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
      S = E;
    }

    std::cout << "Current Polygon: " << PolygonStr(out) << std::endl;
  }
  return out;
}

class Test {
  public:
    void static printTestName(std::string str) {
      std::cout << str << " Test" << std::endl;
    }

    virtual void printTestName() = 0;

    virtual Polygon get_polygon() = 0;

    virtual Box get_box() = 0;

    virtual Polygon get_clipped_polygon() = 0;
};

class NoClippingTest : public Test {
  public:
    void printTestName() {
      Test::printTestName("No Clipping");
    }

    Polygon get_polygon() {
      Point P1 = {-0.5, -0.5};
      Point P2 = { 0.5, -0.5};
      Point P3 = { 0.5,  0.5};
      Point P4 = {-0.5,  0.5};
      Polygon P = {P1, P2, P3, P4};
      return P;
    }

    Box get_box() {
      Point BL = {-1, -1};
      Point TR = {1, 1};
      Box B = {BL, TR};
      return B;
    }

    Polygon get_clipped_polygon() {
      Point P1 = {-0.5, -0.5};
      Point P2 = { 0.5, -0.5};
      Point P3 = { 0.5,  0.5};
      Point P4 = {-0.5,  0.5};
      Polygon P = {P1, P2, P3, P4};
      return P;
    }
};

class AllClippingTestVert : public Test {
  public:
    void printTestName() {
      Test::printTestName("All Clipping -- Vertical");
    }

    Polygon get_polygon() {
      Point P1 = {-1.5, -1.5};
      Point P2 = { 1.5, -1.5};
      Point P3 = { 1.5,  1.5};
      Point P4 = {-1.5,  1.5};
      Polygon P = {P1, P2, P3, P4};
      return P;
    }

    Box get_box() {
      Point BL = {-1, -1};
      Point TR = {1, 1};
      Box B = {BL, TR};
      return B;
    }

    Polygon get_clipped_polygon() {
      Point P1 = {-1.0, -1.0};
      Point P2 = { 1.0, -1.0};
      Point P3 = { 1.0,  1.0};
      Point P4 = {-1.0,  1.0};
      Polygon P = {P1, P2, P3, P4};
      return P;
    }
};

class AllClippingTestNoVert : public Test {
  public:
    void printTestName() {
      Test::printTestName("All Clipping -- No Vertical");
    }

    Polygon get_polygon() {
      Point P1 = {-1.6, -1.6};
      Point P2 = { 1.7, -1.7};
      Point P3 = { 1.8,  1.8};
      Point P4 = {-1.9,  1.9};
      Polygon P = {P1, P2, P3, P4};
      return P;
    }

    Box get_box() {
      Point BL = {-1, -1};
      Point TR = {1, 1};
      Box B = {BL, TR};
      return B;
    }

    Polygon get_clipped_polygon() {
      Point P1 = {-1.0, -1.0};
      Point P2 = { 1.0, -1.0};
      Point P3 = { 1.0,  1.0};
      Point P4 = {-1.0,  1.0};
      Polygon P = {P1, P2, P3, P4};
      return P;
    }
};

class DiamondClippingTest : public Test {
  public:
    void printTestName() {
      Test::printTestName("Diamond");
    }

    Polygon get_polygon() {
      Point P1 = {-1.5,  0.0};
      Point P2 = { 0.0, -1.5};
      Point P3 = { 1.5,  0.0};
      Point P4 = { 0.0,  1.5};
      Polygon P = {P1, P2, P3, P4};
      return P;
    }

    Box get_box() {
      Point BL = {-1, -1};
      Point TR = {1, 1};
      Box B = {BL, TR};
      return B;
    }

    Polygon get_clipped_polygon() {
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

void runTest(Test* T) {
  T->printTestName();
  Polygon P = T->get_polygon();
  std::cout << "Original Polygon: " << PolygonStr(P) << std::endl;
  Box B = T->get_box();
  std::cout << "Clipping Box" << BoxStr(B) << std::endl;
  Polygon P_clipped = ClipPolygon(P, B);
  std::cout << "Clipped Polygon: " << PolygonStr(P_clipped) << std::endl;
  std::cout << "Expected Polygon: " << PolygonStr(T->get_clipped_polygon()) << std::endl;
}

void NoClipping() {
  Test *T = new NoClippingTest();
  runTest(T);
}

void AllClippingVert() {
  Test *T = new AllClippingTestVert();
  runTest(T);
}
void AllClippingNoVert() {
  Test *T = new AllClippingTestNoVert();
  runTest(T);
}
void DiamondClipping() {
  Test *T = new DiamondClippingTest();
  runTest(T);
}


void testIntersectionFunc() {
  Point p1, p2, e1, e2, i;
  Segment poly_edge, edge;

  std::cout << "VERTICAL CUT" << std::endl;
  // vertical cut of positive slope line
  p1 = {-1, -1};
  p2 = { 1, 1};
  poly_edge = {p1, p2};
  e1 = {0,  1};
  e2 = {0, -1};
  edge = {e1, e2};
  i = computeIntersection(poly_edge, edge);
  std::cout << "Calculated: " << PointStr(i) << std::endl;
  std::cout << "Expected : (0,0) " << std::endl;

  // vertical cut of negative slope line
  p1 = {-1, 1};
  p2 = { 1, -1};
  poly_edge = {p1, p2};
  e1 = {0,  1};
  e2 = {0, -1};
  edge = {e1, e2};
  i = computeIntersection(poly_edge, edge);
  std::cout << "Calculated: " << PointStr(i) << std::endl;
  std::cout << "Expected : (0,0) " << std::endl;

  // vertical cut of horizontal line
  p2 = { 1, 0};
  p1 = {-1, 0};
  poly_edge = {p2, p1};
  e1 = {0,  1};
  e2 = {0, -1};
  edge = {e1, e2};
  i = computeIntersection(poly_edge, edge);
  std::cout << "Calculated: " << PointStr(i) << std::endl;
  std::cout << "Expected : (0,0) " << std::endl;

  std::cout << std::endl << "HORIZONTAL CUT" << std::endl;
  // horizontal cut of positive slope line
  p1 = {-1, -1};
  p2 = { 1, 1};
  poly_edge = {p1, p2};
  e1 = {-1, 0};
  e2 = { 1, 0};
  edge = {e1, e2};
  i = computeIntersection(poly_edge, edge);
  std::cout << "Calculated: " << PointStr(i) << std::endl;
  std::cout << "Expected : (0,0) " << std::endl;

  // horiztonal cut of negative slope line
  p1 = {-1, 1};
  p2 = { 1, -1};
  poly_edge = {p1, p2};
  e1 = {-1, 0};
  e2 = { 1, 0};
  edge = {e1, e2};
  i = computeIntersection(poly_edge, edge);
  std::cout << "Calculated: " << PointStr(i) << std::endl;
  std::cout << "Expected : (0,0) " << std::endl;

  // horizontal cut of vertical line
  p1 = {0, -1};
  p2 = {0, 1};
  poly_edge = {p1, p2};
  e1 = {-1, 0};
  e2 = { 1, 0};
  edge = {e1, e2};
  i = computeIntersection(poly_edge, edge);
  std::cout << "Calculated: " << PointStr(i) << std::endl;
  std::cout << "Expected : (0,0) " << std::endl;
}
// make a general test class with virtual functions
// in each test function, allow for a single value to be overwritten
int main() {
  // testIntersectionFunc();
  // NoClipping();
  // AllClippingNoVert();
  // AllClippingVert();
  DiamondClipping();
  return 0;
}
