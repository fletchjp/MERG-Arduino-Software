#ifndef GLUCAT_TEST_PEG12_H
#define GLUCAT_TEST_PEG12_H
/***************************************************************************
    GluCat : Generic library of universal Clifford algebra templates
    peg12.cpp : programming example 12 : Frame group multiplication tables
                             -------------------
    begin                : Sun 2001-12-09
    copyright            : (C) 2001-2007 by Paul C. Leopardi
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

namespace peg12
{
  using namespace glucat;
  using namespace std;

  template< class Multivector_T >
  static
  void 
  mult_table(const Multivector_T& e1, const Multivector_T& e2)
  {
    typedef Multivector_T e;
    const char* spaces("        ");
    e m = 1, a1 = 1, a2 = 1;
    for( int i = 0; i != 2; ++i)
    {
      a2 = 1;
      for( int j = 0; j != 2; ++j)
      {
        a1 = 1;
        for( int k = 0; k != 2; ++k)
        {
          cout <<  m*a1*a2 << spaces <<  m*a1*a2*e1 << spaces
               <<  m*a1*a2*e2 << spaces <<  m*a1*a2*e1*e2 << spaces
               << -m*a1*a2 << spaces << -m*a1*a2*e1 << spaces
               << -m*a1*a2*e2 << spaces << -m*a1*a2*e1*e2 << spaces;
          cout << endl;
          a1 *= e1;
        }
        a2 *= e2;
      }
      m *= -1;
    }
  }

  template< class Multivector_T >
  static
  void 
  do_test12()
  {
    typedef Multivector_T e;
    cout << "{-2}, {-1}" << endl;
    mult_table(e("{-2}"),e("{-1}"));
    cout << endl;
    cout << "{-1}, {1}" << endl;
    mult_table(e("{-1}"),e("{1}"));
    cout << endl;
    cout << "{1}, {2}" << endl;
    mult_table(e("{1}"),e("{2}"));
  }
}

int test12();

#endif
