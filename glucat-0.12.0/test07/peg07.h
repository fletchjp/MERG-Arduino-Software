#ifndef GLUCAT_TEST_PEG07_H
#define GLUCAT_TEST_PEG07_H
/***************************************************************************
    GluCat : Generic library of universal Clifford algebra templates
    peg07.cpp : programming example 7 : Triality
                             -------------------
    begin                : Sun 2001-12-09
    copyright            : (C) 2001-2012 by Paul C. Leopardi
 ***************************************************************************

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library.  If not, see <http://www.gnu.org/licenses/>.

 ***************************************************************************
 This library is based on a prototype written by Arvind Raja and was
 licensed under the LGPL with permission of the author. See Arvind Raja,
 "Object-oriented implementations of Clifford algebras in C++: a prototype",
 in Ablamowicz, Lounesto and Parra (eds.)
 "Clifford algebras with numeric and symbolic computations", Birkhauser, 1996.
 ***************************************************************************
     See also Arvind Raja's original header comments in glucat.h
 ***************************************************************************/

namespace peg07
{
  using namespace glucat;
  using namespace std;

  template< class Multivector_T >
  static
  Multivector_T operator|| (const Multivector_T& lhs, const Multivector_T& rhs)
  {
    typedef Multivector_T number;
    typedef typename number::scalar_t scalar_t;
    const typename number::index_set_t R8("{1,2,3,4,5,6,7,8}");
    const number p1_8("{1,2,3,4,5,6,7,8}", R8);
    const number v("-{1,2,4}-{2,3,5}-{3,4,6}-{4,5,7}-{1,5,6}-{2,6,7}-{1,3,7}", R8);
    const number p8("{8}", R8);
    const number w = v * number("{1,2,3,4,5,6,7}", R8);
    return (lhs*p8*rhs*(scalar_t(1.0)-p1_8)*(scalar_t(1.0)+w))(1);
  }

  template< class Multivector_T >
  static
  Multivector_T expo(const Multivector_T& f)
  {
    typedef Multivector_T number;
    typedef typename number::scalar_t scalar_t;
    const number ff = f^f;
    return scalar_t(1.0) + f + ff/scalar_t(2.0) + (ff^(f/scalar_t(6.0) + ff/scalar_t(24.0)));
  }

  template< class Multivector_T >
  static
  void do_test7()
  {
    typedef Multivector_T number;
    typedef typename number::scalar_t scalar_t;
    const typename number::index_set_t R8("{1,2,3,4,5,6,7,8}");
    const number F =
      number("3{1,2}+4{2,3}+4{2,6}+5{3,7}+{4,5}+2{6,7}", R8) / scalar_t(10.0);
    number u = number(expo(F), R8);
    u /= abs(u);
    const number x = number("3{1}+4{3}+5{5}", R8);
    const number y = number("2{2}+3{4}+7{7}", R8);
    (u*x*reverse(u) || u*y*reverse(u)).write
      ("u*x*reverse(u) || u*y*reverse(u) =");
    cout << endl;
  }
}

int test07();

#endif
