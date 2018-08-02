// https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Point { double x; double y; };
struct Segment { Point start; Point end; };
// Points in CCW order. Implicit edge between first and last point.
using Polygon = std::vector<Point>;
struct Box { Point bl; Point tr; };

std::string PointStr(const Point& p) {
  std::stringstream ss;
  ss.precision(1);
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

bool inside(Segment poly_edge, Segment edge) {


}

Point clipX(Point p1, Point p2, double X) {
  double m = (p1.y - p2.y)/ ((p1.x - p2.x));
  double Y = m * (X - p1.x) + p1.y;
  Point p = {X, Y};
  return p;
}

Polygon ClipPolygon(Polygon subj, const Box& clip) {

  Polygon out = subj;
  Point BL = {clip.bl.x, clip.bl.y};
  Point TL = {clip.bl.x, clip.tr.y};
  Point TR = {clip.tr.x, clip.tr.y};
  Point BR = {clip.tr.x, clip.bl.y};
  Polygon BoxPoints = {BL, TL, TR, BR};

  for (int c_v=0; c_v<4; c_v++) {
    Segment edge = {BoxPoints[c_v], BoxPoints[(c_v+1)%4]};
    
    Polygon in = out;
    out.clear();
    Point S = in.back();

    for (auto E: in) {
      Segment poly_edge = {S, E};
      if (inside(poly_edge, edge)) {
    //     if (!inside(S, edge)) {
    //       P_x = computeIntersection(S, E, edge);
    //       out.push_back(P_x);
    //     }
    //     out.push_back(E);
      }
    //   else if (inside(S, edge)) {
    //     P_x = computeIntersection(S, E, edge);
    //     out.push_back(P_x);
    //   }
    //   S = E;
    }
  }
  return out;
}

class NoClippingTest {
public:
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
};

void NoClipping() {
  Polygon P = NoClippingTest::get_polygon();
  std::cout << PolygonStr(P) << std::endl;
  Box B = NoClippingTest::get_box();
  std::cout << BoxStr(B) << std::endl;
  Polygon P_clipped = ClipPolygon(P, B);
}

int main() {
  NoClipping();
  return 0;
}
