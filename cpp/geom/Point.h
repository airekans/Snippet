#ifndef GEOM_POINT_H_
#define GEOM_POINT_H_


namespace goem {

template<typename T>
class Point2D
{
public:
    Point2D(T x, T y) : m_x(x), m_y(y) {}
    
    T GetX() const { return m_x; }
    void SetX(T x) { m_x = x; }
    
    T GetY() const { return m_y; }
    void SetY(T y) { m_y = y; }

private:
    T m_x;
    T m_y;
};

typedef Point2D<int> Point2DInt32;

}  // namespace goem

#endif // GEOM_POINT_H_

