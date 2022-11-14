// fcppcout.h

// Output operators for List Maybe and ByNeed
// These are needed at different points.

#ifndef FCPPCOUT_H
#define FCPPCOUT_H

namespace fcpp {

#ifndef FCPP_ARDUINO
template <class A, class B>
std::ostream& operator<<( std::ostream& o, const std::pair<A,B>& p ) {
   return o << "(" << p.first << "," << p.second << ")";
}
#endif

#ifndef FCPP_ARDUINO
#ifndef FCPPHASLISTCOUT
// Nondestructive output operator for a List<T>
template <class T>
std::ostream & operator << (std::ostream &o, const List<T> &l)
{
  typename List<T>::iterator i;
  if ( !null(l) ) // No output if no list.
    for (i = l.begin(); i != l.end(); i++)  {
	o << *i << std::endl;
  }
  return o;
}
#endif
#endif

//Maybe<int> what;
#ifndef FCPP_ARDUINO

// This compiles after Maybe has been used but not before.
template <class T>
std::ostream& operator<<( std::ostream& o, Maybe<T> const &mx ) {
   if( mx.is_nothing() )
      o << "Nothing";
   else
      o << "Just " << mx.value();
   return o;
}

// Output operator for Either adapted from the one for Maybe
template <class T>
std::ostream& operator<<( std::ostream& o, Either<T> const &ex ) {
   if( ex.is_error() )
     o << "Error : " << ex.left();
   else
     o << "Either : " << ex.right();
   return o;
}

// Output operator for Identity adapted from the one for Maybe
// The decision I have made is to return only the value.
template <class T>
std::ostream& operator<<( std::ostream& o, Identity<T> const &ix ) {
   if( ix.is_error() )
     o << "Empty Identity ";
   else
     o << ix.value();
   return o;
}


#ifdef FCPP_ENABLE_LAMBDA
// Only if monads are defined.
template <class T>
std::ostream& operator<<( std::ostream& o, ByNeed<T> const &mx ) {
  o << "force: " << mx.force();
  return o;
}
#endif
#endif

} // end namespace fcpp

#endif


