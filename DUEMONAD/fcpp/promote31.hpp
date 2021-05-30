/* The following code example is taken from the book
 * "C++ Templates - The Complete Guide"
 * by David Vandevoorde and Nicolai M. Josuttis, Addison-Wesley, 2002
 *
 * (C) Copyright David Vandevoorde and Nicolai M. Josuttis 2002.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
// primary template for type promotion with three parameters - extension of the above.
#ifndef PROMOTE31_HPP
#define PROMOTE31_HPP

template<typename T1, typename T2, typename T3>
class Promotion3 {
  public:
  typedef typename Promotion <T1,typename Promotion<T2,T3>::ResultT>::ResultT ResultT;
};

template<typename T1, typename T2, typename T3, typename T4>
class Promotion4 {
  public:
  typedef typename Promotion <T1,typename Promotion3<T2,T3,T4>::ResultT>::ResultT ResultT;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
class Promotion5 {
  public:
  typedef typename Promotion <T1,typename Promotion4<T2,T3,T4,T5>::ResultT>::ResultT ResultT;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Promotion6 {
  public:
  typedef typename Promotion <T1,typename Promotion5<T2,T3,T4,T5,T6>::ResultT>::ResultT ResultT;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Promotion7 {
  public:
  typedef typename Promotion <T1,typename Promotion6<T2,T3,T4,T5,T6,T7>::ResultT>::ResultT ResultT;
};

#endif
