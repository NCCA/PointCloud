/*
  Copyright (C) 2011 Jon Macey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cmath>
#include "DVec3.h"
#include <ngl/Types.h>

constexpr double  EPSILON = 0.0001;

//----------------------------------------------------------------------------------------------------------------------
/// @brief DCompare macro used for floating point comparision functions
//----------------------------------------------------------------------------------------------------------------------
  #define DCompare(a,b) \
      ( ((a)-EPSILON)<(b) && ((a)+EPSILON)>(b) )

//----------------------------------------------------------------------------------------------------------------------
/// @file DVec3.cpp
/// @brief implementation files for DVec3 class
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void DVec3::set(double _x,   double _y,  double _z ) noexcept
{
	m_x=_x;
	m_y=_y;
	m_z=_z;
}


//----------------------------------------------------------------------------------------------------------------------
void DVec3::null() noexcept
{
  m_x=0.0;
  m_y=0.0;
  m_z=0.0;
}

//----------------------------------------------------------------------------------------------------------------------
double& DVec3::operator[](const size_t & _i ) noexcept
{
	return (&m_x)[_i];
}



//----------------------------------------------------------------------------------------------------------------------
DVec3 DVec3::operator-() const noexcept
{
  return DVec3(-m_x,-m_y,-m_z);
}


//----------------------------------------------------------------------------------------------------------------------
void DVec3::operator+=(const DVec3& _v) noexcept
{
	m_x+=_v.m_x;
	m_y+=_v.m_y;
	m_z+=_v.m_z;
}

//----------------------------------------------------------------------------------------------------------------------
void DVec3::operator/=(double _v) noexcept
{
  m_x/=_v;
	m_y/=_v;
	m_z/=_v;
}
//----------------------------------------------------------------------------------------------------------------------
void DVec3::operator*=(double _v) noexcept
{
	m_x*=_v;
	m_y*=_v;
	m_z*=_v;
}
//----------------------------------------------------------------------------------------------------------------------
void DVec3::operator-=(const DVec3& _v) noexcept
{
	m_x-=_v.m_x;
	m_y-=_v.m_y;
	m_z-=_v.m_z;
}

//----------------------------------------------------------------------------------------------------------------------
DVec3 DVec3::operator/(double _v)const noexcept
{
  return DVec3(m_x/_v,m_y/_v,m_z/_v);
}

//----------------------------------------------------------------------------------------------------------------------
DVec3 DVec3::operator+( const DVec3& _v)const noexcept
{
  return DVec3(m_x+_v.m_x,m_y+_v.m_y,m_z+_v.m_z);
}

//----------------------------------------------------------------------------------------------------------------------
DVec3 DVec3::operator-(const DVec3& _v)const noexcept
{
  return DVec3(m_x-_v.m_x,m_y-_v.m_y,m_z-_v.m_z);
}

//----------------------------------------------------------------------------------------------------------------------
bool DVec3::operator==(const DVec3& _v)const noexcept
{
	return (
          DCompare(_v.m_x,m_x)  &&
          DCompare(_v.m_y,m_y)  &&
          DCompare(_v.m_z,m_z)
				 );
}
//----------------------------------------------------------------------------------------------------------------------
bool DVec3::operator!=(const DVec3& _v  )const noexcept
{
	return (
          !DCompare(_v.m_x,m_x) ||
          !DCompare(_v.m_y,m_y) ||
          !DCompare(_v.m_z,m_z)
				 );
}
//----------------------------------------------------------------------------------------------------------------------
DVec3 DVec3::operator*( const DVec3& _v  )const noexcept
{
  return DVec3(m_x*_v.m_x,m_y*_v.m_y,m_z*_v.m_z );
}

//----------------------------------------------------------------------------------------------------------------------
DVec3 DVec3::operator/( const DVec3& _v )const noexcept
{
  return DVec3(m_x/_v.m_x,m_y/_v.m_y,m_z/_v.m_z);
}

//----------------------------------------------------------------------------------------------------------------------
DVec3 DVec3::operator *(double _i)const noexcept
{
  return DVec3(m_x*_i,m_y*_i,m_z*_i);
}

//----------------------------------------------------------------------------------------------------------------------
DVec3 & DVec3::operator=(double _v) noexcept
{
  m_x = _v;
  m_y = _v;
  m_z = _v;
  return *this;
}


//----------------------------------------------------------------------------------------------------------------------
void DVec3::cross(const DVec3& _v1, const DVec3& _v2) noexcept
{
  m_x=_v1.m_y*_v2.m_z-_v1.m_z*_v2.m_y;
  m_y=_v1.m_z*_v2.m_x-_v1.m_x*_v2.m_z;
  m_z=_v1.m_x*_v2.m_y-_v1.m_y*_v2.m_x;
}

//----------------------------------------------------------------------------------------------------------------------
DVec3 DVec3::cross( const DVec3& _v )const noexcept
{
  return DVec3(m_y*_v.m_z - m_z*_v.m_y,
              m_z*_v.m_x - m_x*_v.m_z,
              m_x*_v.m_y - m_y*_v.m_x
             );

}


//----------------------------------------------------------------------------------------------------------------------
void DVec3::normalize() noexcept
{
  double len=sqrt(m_x*m_x+m_y*m_y+m_z*m_z);
  m_x/=len;
  m_y/=len;
  m_z/=len;
}





//----------------------------------------------------------------------------------------------------------------------
double DVec3::length() const noexcept
{
  return sqrt((m_x*m_x)+(m_y*m_y)+(m_z*m_z));
}


//----------------------------------------------------------------------------------------------------------------------
double DVec3::lengthSquared() const noexcept
{
  return m_x * m_x+m_y * m_y+ m_z*m_z;
}



void DVec3::clamp(double _min, double _max ) noexcept
{
  m_x<_min ? m_x = _min : m_x;
  m_x>_max ? m_x = _max : m_x;

  m_y<_min ? m_y = _min : m_y;
  m_y>_max ? m_y = _max : m_y;

  m_z<_min ? m_z = _min : m_z;
  m_z>_max ? m_z = _max : m_z;
}

void DVec3::clamp(double _max ) noexcept
{
  m_x<-_max ? m_x = -_max : m_x;
  m_x>_max ? m_x = _max : m_x;

  m_y<-_max ? m_y = -_max : m_y;
  m_y>_max ? m_y = _max : m_y;

  m_z<-_max ? m_z = -_max : m_z;
  m_z>_max ? m_z = _max : m_z;


}



