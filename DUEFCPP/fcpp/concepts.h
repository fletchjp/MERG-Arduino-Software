// concepts.h

// New file to provide a model for concepts

// Copy Constructible removed from arguments, 04 June 2007

// Added ConstCallable6 and ConstCallable7
// also added default parameters to these and ConstCallable5   02 Oct 2007

// Added ConstCallable8   26 January 2008

// Added up to ConstCallable12  February 2008

// Added up to ConstCallable14  March 2008

/*
These concept maps are the equivalent of the different Sig
definitions in full.h. These deal with the cases
with the full number of template parameters.
I have now sorted out how to deal with the case with only one parameter.
This involves a default parameter in the concepts.h file.
These specialisations are in full.h
*/
// Note that for later versions of the concepts compiler DEVEL should be defined.


#ifndef FCPP_CONCEPTS_DOT_H
#define FCPP_CONCEPTS_DOT_H

#ifdef __GXX_CONCEPTS__
#include <concepts>
#define FCPP_CONCEPTS
#endif

std::string check_for_concepts()
{
  std::string concepts;
#ifdef __GXX_CONCEPTS__
  concepts = std::string("concepts are defined");
#else
  concepts = std::string("concepts are not defined");
#endif
  //std::cout << concepts << std::endl;
  return concepts;
}

// Done like this so that the first checks can change.
#ifdef FCPP_CONCEPTS

using namespace std;

namespace fcpp {

struct Nothing {};

// Took out unnecessary dummy arguments from some places in the calls.
auto concept ConstCallable0<typename F> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&);
};

auto concept ConstCallable1<typename F, typename X> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&, const X&);
};

#ifdef DEVEL
auto concept ConstCallable1ConstructibleResult<typename F, typename X> {
  typename result_type;
  requires CopyConstructible<result_type>;
  result_type operator()(const F&, const X&);
};
#endif

auto concept ConstCallable2<typename F, typename X, typename Y = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&, const X&, const Y&);
};

auto concept ConstCallable3<typename F, typename X, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&, const X&, const Y&, const Z&);
};

auto concept ConstCallable4<typename F, typename W, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable5<typename F, typename V,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&, const V&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable6<typename F, typename U,typename V = Nothing,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&, const U&,const V&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable7<typename F, typename T,typename U = Nothing,typename V = Nothing,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&, const T&,const U&,const V&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable8<typename F, typename S, typename T = Nothing,typename U = Nothing,typename V = Nothing,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&,const S&,const T&,const U&,const V&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable9<typename F, typename R, typename S = Nothing, typename T = Nothing,typename U = Nothing,typename V = Nothing,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&,const R&,const S&,const T&,const U&,const V&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable10<typename F, typename Q, typename R = Nothing, typename S = Nothing, typename T = Nothing,typename U = Nothing,typename V = Nothing,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&,const Q&,const R&,const S&,const T&,const U&,const V&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable11<typename F, typename P,typename Q = Nothing, typename R = Nothing, typename S = Nothing, typename T = Nothing,typename U = Nothing,typename V = Nothing,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&,const P&,const Q&,const R&,const S&,const T&,const U&,const V&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable12<typename F, typename O,typename P = Nothing,typename Q = Nothing, typename R = Nothing, typename S = Nothing, typename T = Nothing,typename U = Nothing,typename V = Nothing,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&,const O&,const P&,const Q&,const R&,const S&,const T&,const U&,const V&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable13<typename F, typename N,typename O = Nothing,typename P = Nothing,typename Q = Nothing, typename R = Nothing, typename S = Nothing, typename T = Nothing,typename U = Nothing,typename V = Nothing,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&,const N&,const O&,const P&,const Q&,const R&,const S&,const T&,const U&,const V&, const W&, const X&, const Y&, const Z&);
};

 auto concept ConstCallable14<typename F, typename M,typename N = Nothing,typename O = Nothing,typename P = Nothing,typename Q = Nothing, typename R = Nothing, typename S = Nothing, typename T = Nothing,typename U = Nothing,typename V = Nothing,typename W = Nothing, typename X = Nothing, typename Y = Nothing, typename Z = Nothing> {
  typename result_type;
#ifndef DEVEL
  requires CopyConstructible<result_type>;
#endif
  result_type operator()(const F&,const M&,const N&,const O&,const P&,const Q&,const R&,const S&,const T&,const U&,const V&, const W&, const X&, const Y&, const Z&);
};

 // These are not currently in use
 /*
auto concept ConstCalled1<typename X> {
  typename result_type;
  requires CopyConstructible<result_type>;
  requires CopyConstructible<X>;
  result_type operator()(const X&);
};

auto concept Called1<typename X> {
  typename result_type;
  requires CopyConstructible<result_type>;
  requires CopyConstructible<X>;
  result_type operator()(X);
};

auto concept ConstPredicate<typename X> {
  typename result_type;
  requires CopyConstructible<result_type>;
  requires CopyConstructible<X>;
  requires Convertible<result_type, bool>;
  result_type operator()(const X&);
};
 */
}

#endif // FCPP_CONCEPTS

#endif // FCPP_CONCEPTS_DOT_H
