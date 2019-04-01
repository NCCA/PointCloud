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
#ifndef DVEC3_H_
#define DVEC3_H_
//----------------------------------------------------------------------------------------------------------------------
/// @file DDVec3.h
/// @brief a simple 3 tuple container for compatibility with glsl
//----------------------------------------------------------------------------------------------------------------------
#include <array>
#include <ostream>
//----------------------------------------------------------------------------------------------------------------------
/// @file DDVec3.h
//----------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------
/// @class DVec3 "include/DVec3.h"
/// @brief simple DVec3 encapsulates a 3 double object like glsl DVec3 but not maths
/// use the DVec3 class for maths and the asDVec3 to return a DVec3
//----------------------------------------------------------------------------------------------------------------------


class  DVec3
{

public:

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief default ctor use default and set to (0.0f,0.0f,0.0f) as attributes are initialised
  //----------------------------------------------------------------------------------------------------------------------
  DVec3() : m_x(0.0),m_y(0.0),m_z(0.0) {}
  DVec3(double _f) : m_x(_f),m_y(_f),m_z(_f) {}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief copy ctor we have POD data so let the compiler do the work!
  //----------------------------------------------------------------------------------------------------------------------
  DVec3(const DVec3& _v)=default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief initialise the constructor from 3 doubles
  /// @param[in]  _x x value
  /// @param[in]  _y y value
  /// @param[in]  _z z value
  //----------------------------------------------------------------------------------------------------------------------
  DVec3(double _x,  double _y, double _z) noexcept:
        m_x(_x),m_y(_y),m_z(_z){}


  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the DVec3 component from 3 values
  /// @param[in]  _x the x component
  /// @param[in]  _y the y component
  /// @param[in]  _z the z component
  //----------------------------------------------------------------------------------------------------------------------
  void set( double _x,  double _y,  double _z) noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set from another  DVec3
  /// @param[in]  _v the DVec3 to set from
  //----------------------------------------------------------------------------------------------------------------------
  void set( const DVec3& _v) noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief return this dotted with _b
  /// @param[in]  _b vector to dot current vector with
  /// @returns  the dot product
  //----------------------------------------------------------------------------------------------------------------------
  double dot(const DVec3 &_b  )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clears the DVec3 to 0,0,0
  //----------------------------------------------------------------------------------------------------------------------
  void null() noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief [] index operator to access the index component of the DVec3
  /// @returns  this[x] as a double
  //----------------------------------------------------------------------------------------------------------------------
  double& operator[](const size_t & _i )  noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief [] index operator to access the index component of the DVec3
  /// @returns  this[x] as a double
  //----------------------------------------------------------------------------------------------------------------------
  const double& operator[](const size_t & _i ) const  noexcept{ return m_openGL[_i]; }

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Normalize the vector using
  /// \n \f$x=x/\sqrt{x^2+y^2+z^2} \f$
  /// \n \f$y=y/\sqrt{x^2+y^2+z^2} \f$
  /// \n \f$z=z/\sqrt{x^2+y^2+z^2} \f$
  //----------------------------------------------------------------------------------------------------------------------
  void normalize() noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the length of the vector
  /// @returns  \f$\sqrt{x^2+y^2+z^2} \f$
  //----------------------------------------------------------------------------------------------------------------------
  double length() const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the length squared of the vector (no sqrt so quicker)
  /// @returns  \f$x^2+y^2+z^2 \f$
  //----------------------------------------------------------------------------------------------------------------------
  double lengthSquared() const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief += operator add DVec3 v to current DVec3
  /// @param[in]  &_v DVec3 to add
  //----------------------------------------------------------------------------------------------------------------------
  void operator+=(const DVec3& _v ) noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief -= operator this-=v
  /// @param[in]  &_v DVec3 to subtract
  //----------------------------------------------------------------------------------------------------------------------
  void operator-=( const DVec3& _v ) noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this * i for each element
  /// @param[in]  _i the scalar to mult by
  /// @returns DVec3
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 operator *( double _i )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief + operator add DVec3+DVec3
  /// @param[in]  &_v the value to add
  /// @returns the DVec3 + v
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 operator +(const DVec3 &_v )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief divide DVec3 components by a scalar
  /// @param[in] _v the scalar to divide by
  /// @returns a DVec3 V(x/v,y/v,z/v,w)
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 operator/(double _v  )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief divide this DVec3 components by a scalar
  /// @param[in] _v the scalar to divide by
  /// sets the DVec3 to DVec3 V(x/v,y/v,z/v,w)
  //----------------------------------------------------------------------------------------------------------------------
  void operator/=( double _v ) noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief multiply this DVec3 components by a scalar
  /// @param[in] _v the scalar to multiply by
  /// sets the DVec3 to DVec3 V(x*v,y*v,z*v,w)
  //----------------------------------------------------------------------------------------------------------------------
  void operator*=( double _v ) noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief subtraction operator subtract vevtor-DVec3
  /// @param[in]  &_v the value to sub
  /// @returns this - v
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 operator-(const DVec3  &_v   )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief * operator mult vevtor*DVec3
  /// @param[in]  _v the value to mult
  /// @returns new DVec3 this*v
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 operator*( const DVec3 &_v  )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief assignment operator set the current DVec3 to rhs
  /// @param[in] _v the DVec3 to set
  /// @returns a new DVec3
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 &operator =( const DVec3 &_v )=default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief assignment operator set the current DVec3 to rhs
  /// @param[in] _v the double to set
  /// @returns a new DVec3
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 &operator =(double _v ) noexcept;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief negate the DVec3 components
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 operator-() const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief check for equality uses FCompare (from Util.h) as double values
  /// @param[in] _v the DVec3 to check against
  /// @returns true or false
  //----------------------------------------------------------------------------------------------------------------------
  bool operator==( const DVec3 &_v )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief not equal check
  /// @param[in] _v the DVec3 to check against
  /// @returns true of false
  //----------------------------------------------------------------------------------------------------------------------
  bool operator!=( const DVec3 &_v )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief \ operator div DVec3/DVec3
  /// @param[in]  _v the value to div by
  /// @returns DVec3 / DVec3
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 operator/( const DVec3& _v )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the DVec3 as the cross product from 2 other DVec3
  /// @param[in]  _v1 the first vector
  /// @param[in]  _v2 the second vector
  //----------------------------------------------------------------------------------------------------------------------
  void cross(const DVec3& _v1, const DVec3& _v2 ) noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief return the cross product of this cross with b
  /// @param[in]  _b the vector cross this with
  /// @returns  the result of this cross b
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 cross(const DVec3& _b )const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clamp the vector values between _min and _max
  /// @param[in]  _min value
  /// @param[in]  _max value
  //----------------------------------------------------------------------------------------------------------------------
  void clamp(double _min, double _max) noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clamp the vector values between +/-_max
  /// @param[in]  _max value
  //----------------------------------------------------------------------------------------------------------------------
  void clamp(double _max) noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief return the vector reflected with this and N
  /// @param[in]  _n the normal vector
  /// @returns  the reflection vector of this with N
  //----------------------------------------------------------------------------------------------------------------------
  DVec3 reflect(const DVec3 & _n) const noexcept;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief accesor to the m_openGL array returns the address of the 0th element
  //----------------------------------------------------------------------------------------------------------------------
  inline double* openGL() noexcept{return &m_openGL[0];}

/// @note I've made this public as some compilers automatically make the
/// anonymous unions public whereas clang++ complains see this post
/// http://jonmacey.blogspot.com/2011/03/anonymous-union-struct-weirdness.html
public :
#pragma pack(push,1)

  union
  {
    struct
    {
      double m_x; //!< x component
      double m_y; //!< y component
      double m_z; //!< z component

    };
    struct
    {
      double m_r; //!< r component
      double m_g; //!< g component
      double m_b; //!< b component

    };
#pragma pack(pop)
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief array of four doubles mapped to the x,y,z,w components of the DVec3 useful for openGL fv data types
  /// this is mapped as a union to the following \n
  /// m_x == m_openGL[0] \n
  /// m_y == m_openGL[1] \n
  /// m_z == m_openGL[2] \n
  //----------------------------------------------------------------------------------------------------------------------
  // we have to set the values here rather than above due to C++ spec
  // see section implicitly declared as defaulted (8.4).
  std::array<double,3> m_openGL;
  };
};
//----------------------------------------------------------------------------------------------------------------------
/// @brief scalar * vector operator
/// @param _k the double value
/// @param _v the vector value
/// @returns a vector _k*v
//----------------------------------------------------------------------------------------------------------------------
inline DVec3 operator *(double _k, const DVec3 &_v) noexcept
{
  return DVec3(_k*_v.m_x, _k*_v.m_y, _k*_v.m_z);
}

std::ostream& operator<<( std::ostream& _output, const DVec3& _v	)
{
  return _output<<"["<<_v.m_x<<","<<_v.m_y<<","<<_v.m_z<<"]";
}

#endif

//----------------------------------------------------------------------------------------------------------------------
