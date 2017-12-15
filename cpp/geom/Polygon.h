#ifndef GEOM_POLYGON_H_
#define GEOM_POLYGON_H_

#include <vector>

#include "Point.h"

namespace geom {

template<typename T>
class Polygon
{
public:
     typedef Point2D<T> PointType;

     std::size_t GetPointSize() const { return m_points.size(); }
     const PointType& GetPoint(std::size_t i) const { return m_points[i]; }
     void SetPoint(std::size_t i, const PointType& p) { m_points[i] = p; }

private:
     std::vector<PointType> m_points;
};

}  // namespace geom

#endif  // GEOM_POLYGON_H_
