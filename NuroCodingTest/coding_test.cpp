// https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Point { double x; double y; };
struct Segment { Point start; Point end; };
// Poinst in CCW order. Implicit edge between first and last point.
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
  for (int i = 0; i < polygon.size(); ++i) {
    s += PointStr(polygon[i]);
    if (i < polygon.size() - 1) {
      s += " ";
    }
  }
  return s + ")";
}

Point clipX(Point p1, Point p2, double X) {
  double m = (p1.y - p2.y)/ ((p1.x - p2.x))
  double Y = m * (X - p1.x) + p1.y;
  Point p = {X, Y};
  return p;
}

Polygon ClipPolygon(Polygon subj, const Box& clip) {

  Polygon newsubj = subj;
  newsubj.append(subj[0]);
  for (int c_v=0; c_v<4; c_v++) {
    
    for (size_t p_v=0; p_v<newsubj.size()-1; p_v++) {
      // deal with last point
      // check inside vs outside
      if (c_v == 0) { // left
        if (newsubj[p_v].x < clip.bl.x) {
          Point p;
          p = clipX(newsubj[p_v], newsubj[p_v+1], clip.bl.x);
        }
      }
      else if (c_v == 1) { // top
        if (newsubj[p_v].y > clip.tr.y) {
          newsubj[p_v].y = clip.tr.y;
        }
      }
      else if (c_v == 2) { // right
        if (newsubj[p_v].x > clip.tr.x) {
          subjnewsubjp_v].x = clip.tr.x;
        }
      }
      else if (c_v == 3) { // bottom
        if (newsubj[p_v].y < clip.bl.y) {
          newsubj[p_v].y = clip.bl.y;
        }
      }
    }
    newsubj
    return subj;
  }
}

int main() {
  return 0;
}
