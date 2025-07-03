# include "vector.hpp"

std::ostream &operator<<(std::ostream &os, const Vector3 &v)
{
	os << '[' << v.m_x << ' ' << v.m_y << ' ' << v.m_z << ']';
	return os;
}