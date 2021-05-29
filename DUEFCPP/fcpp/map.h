 //
 // Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
 //
 // Permission to use, copy, modify, distribute and sell this software
 // and its documentation for any purpose is granted without fee,
 // provided that the above copyright notice and this permission notice
 // appear in all source code copies and supporting documentation. The
 // software is provided "as is" without any express or implied
 // warranty.

 #ifndef FCPP_MAP_DOT_H
 #define FCPP_MAP_DOT_H

// 15th July 2011 New file map.h for the definition
// of new Map and MultiMap FC++ data structures.
// Both are now defined using MapKernel.
 
// These are located in fcpp::data to avoid name clashes.

// John Fletcher (J.P.Fletcher@aston.ac.uk) July 2011.

// Revisited September 2013.
// I have changed many routines over to use List<T>::iterator.

namespace fcpp {

  namespace data {
     
    namespace impl {
      using std::pair;
      using std::make_pair;

      template <class K, class T>
	class MapKernel;
      template <class K, class T>
	class MultiMap; // Needed for copying from one to the other.

      // Map made from MultiMap with unique keys.
      // or else both inherit a common kernel.
      // Previous MultiMap now renamed MapKernel.
      //
      template <class K, class T>
      class Map : public MapKernel<K,T> {
      private:
        static bool unique_keys;
      public:
        typedef pair<K,T> ListType;
        Map() { hasuniquekeys = unique_keys; }

	// See C++ Templates The Complete Guide p.139
	using MapKernel<K,T>::l;
	using MapKernel<K,T>::sort;
	using MapKernel<K,T>::hasuniquekeys;

	Map(const List<ListType> ll) {
          l = ll; sort(unique_keys); hasuniquekeys = unique_keys;
        }
        Map(const Map<K,T>& map) { l = map.l; hasuniquekeys = unique_keys; }
        Map(const MultiMap<K,T>& map) {
          l = map.l; sort(unique_keys); hasuniquekeys = unique_keys;
        }
        Map<K,T>& operator=( const Map<K,T>& map ) {
            if( this == &map ) return *this;
            l = map.l;
            return *this;
	}
        Map<K,T>& operator=( const MultiMap<K,T>& map ) {
	    l = map.l; sort(unique_keys); // Need to sort.
            return *this;
	}

	~Map() { }

        friend class MapKernel<K,T>;

      };

      template <class K, class T>
      class MultiMap : public MapKernel<K,T> {
      private:
        static bool unique_keys;
      public:
        typedef pair<K,T> ListType;
        MultiMap() { hasuniquekeys = unique_keys; }

	// See C++ Templates The Complete Guide p.139
	using MapKernel<K,T>::l;
	using MapKernel<K,T>::sort;
	using MapKernel<K,T>::hasuniquekeys;

	MultiMap(const List<ListType> ll) {
          l = ll; sort(unique_keys); hasuniquekeys = unique_keys;
        }
        MultiMap(const MultiMap<K,T>& map) {
          l = map.l; hasuniquekeys = unique_keys;
        }
        MultiMap(const Map<K,T>& map) {
          l = map.l; hasuniquekeys = unique_keys;
        }
        MultiMap<K,T>& operator=( const MultiMap<K,T>& map ) {
            if( this == &map ) return *this;
            l = map.l;
            return *this;
	}
        MultiMap<K,T>& operator=( const Map<K,T>& map ) {
            l = map.l;
            return *this;
	}
	~MultiMap() { }

        friend class MapKernel<K,T>;
	
      };

      // This distinguishes a Map from a MultiMap.
      // Still to be implemented.
      // See C++ Templates The Complete Guide p.98
      template <class K, class T>
      bool Map<K,T>::unique_keys = true;
      template <class K, class T>
      bool MultiMap<K,T>::unique_keys = false;

      template <class K, class T>
      class MapKernel {
      public:
        typedef pair<K,T> ListType;
        typedef typename List<ListType>::iterator IteratorType;
      protected:
        List<ListType> l;
        // I need to sort the list.
        void sort(bool uniquekeys);
        MapKernel() {}
	~MapKernel() { }
        // MapKernels are not copyable or assignable
	MapKernel( const MapKernel<K,T>& );
        void operator=( MapKernel<K,T> );
	bool hasuniquekeys;
      public:
        typedef K KeyType;
        typedef T ElementType;


        // These are not needed - done at the upper level
        //MapKernel(const List<ListType> ll)  { }
        //MapKernel(const MapKernel<K,T>& map) { l = map.l; }


        // Modelled on STD map in a loose way.
        int size() const { return length(l); }
        bool empty() const { return length(l) == 0; }
        int count(const KeyType &key) const;
        ListType find(const KeyType &key) const;
        int list(bool rev=false) const; // List in order
	//#ifdef FCPP_FIX_FOR_CLANG
        //bool insert(const ListType &entry, bool atback = true);
	//#else
        pair<int,bool> insert(const ListType &entry, bool atback = true);
        /* default is atback = true moved here to satisfy Clang. */
	//#endif
        bool del(const KeyType &key);
        int  pos(const KeyType &key) const;
        ListType find_from_pos(int post) const;
        //bool isMap() const;
        KeyType max(T start=T(0)) const;
        List<ListType> get_list(bool rev=false) const;
      };

      template <class K, class T>
      int MapKernel<K,T>::count(const K &key) const
      {
	int i = 0;
        IteratorType it;
        if (length(l) > 0) {
	  for (it = l.begin(); it != l.end(); ++it) {
            if (it->first == key) i++;
	  }
	}
	return i;
      }

      template <class K, class T>
	List<pair<K,T> > MapKernel<K,T>::get_list(bool rev) const
      {
	if (rev) return reverse(l);
	else return l;
      }


      template <class K, class T>
      int MapKernel<K,T>::list(bool rev) const
      {
	int i = 0;
        IteratorType it;
        if (length(l) > 0) {
          if(rev) {
            List<ListType> ll = reverse(l);
#ifndef FCPP_ARDUINO
  	    for (it = ll.begin(); it != ll.end(); ++it) {
  	       std::cout << it->first << " " << it->second << std::endl;
	    }
#endif
	  } else {
#ifndef FCPP_ARDUINO
 	    for (it = l.begin(); it != l.end(); ++it) {
  	       std::cout << it->first << " " << it->second << std::endl;
	    }
#endif
	  }
	}
	return i;
      }

      template <class K, class T>
      int MapKernel<K,T>::pos(const K &key) const
      {
        int pos = 0;
        IteratorType it;
        if (length(l) > 0) {
 	  for (it = l.begin(); it != l.end(); ++it) {
             if (it->first == key) {
	       return pos;
	     }
             pos++;
	   }
	}
	return -1;
      }

      template <class K, class T>
      K MapKernel<K,T>::max(T start) const
      {
	T max = start;
        K key;
        IteratorType it;
        if (length(l) > 0) {
           for (it = l.begin(); it != l.end(); ++it)
           {
                if (it->second > max) {
  	           max = it->second;
                   key = it->first;
                }
           }
        }
        return key;
      }

      template <class K, class T>
      pair<K,T> MapKernel<K,T>::find(const K &key) const
      {
        IteratorType it;
        if (length(l) > 0) {
	  for (it = l.begin(); it != l.end(); ++it) {
               if (it->first == key) {
		 return *it;
	       }
	   }
	}
	return make_pair<K,T>(K(),T());
      }

      template <class K, class T>
      pair<K,T> MapKernel<K,T>::find_from_pos(int pos) const
      {
        IteratorType it;
        if (length(l) > 0 && pos >= 0) {
	  int count = 0;
	  for (it = l.begin(); it != l.end(); ++it) {
             if (count == pos) {
	       return *it;
	     }
             count++;
	  }
	}
	return make_pair<K,T>(K(),T());
      }

      template <class K, class T>
      bool MapKernel<K,T>::del(const K &key)
      { // There is no need to sort on deletion.
        List<ListType> result;
        bool done = false;
        IteratorType it;
        if (length(l) > 0) {
	  for (it = l.begin(); it != l.end(); ++it) {
	    if (!(it->first == key) ) {
	       result = cat(result,list_with(*it));
	     } else done = true;
	   }
           l = result;
	}
	return done;
      }

      // Changed to return bool for success.
      // This is in preparation for Map as well as MultiMap.
      // This line is an error with Clang 3.1 and libc++.
      // It is not clear to me why. This is code I built.
      template <class K, class T>
	//#ifdef FCPP_FIX_FOR_CLANG
	//bool MapKernel<K,T>::insert(const pair<K,T> &entry,
        //                                    bool atback)
      //#else
      pair<int,bool> MapKernel<K,T>::insert(const pair<K,T> &entry,
                                            bool atback)
	//#endif
      {
        bool done = true;
        if ( count(entry.first) > 0 && hasuniquekeys ) {
            // Fail for Map case if key is already present.
            done = false;
	    //#ifdef FCPP_FIX_FOR_CLANG
            //return done;
	    //#else
            return make_pair(pos(entry.first),done);
	    //#endif
	}
        if (atback) {
            // Choice of where to do insert.
	    // The choice is now controlled by bool atback (default true).
            l = cat(l,list_with(entry));
	} else {
   	    l = cons(entry, l);
	}
        sort(hasuniquekeys);
	//#ifdef FCPP_FIX_FOR_CLANG
        //    return done;
	//#else
        return make_pair(pos(entry.first),done);
	//#endif
      }

      template <class K, class T>
      void MapKernel<K,T>::sort(bool uniquekeys)
      {
	// Don't need to sort a list of 1!
        if (length(l) > 1) {
#ifdef FCPP_DEBUG_MAP
	  std::cout << "Starting to sort with length "
                    << length(l) << std::endl;
#endif
	  List<ListType> copy = l;
          bool changed = true;
          bool just_changed = false;
          while (changed) {
#ifdef FCPP_DEBUG_MAP
	    std::cout << "after while(changed)" << std::endl;
#endif
            changed = false;
            List<ListType> temporary = copy;
            List<ListType> new_copy;
            ListType first_pair = head(temporary);
            K first_key = first_pair.first;
#ifdef FCPP_DEBUG_MAP
	    std::cout << "first key = " << first_key << std::endl;
#endif
            while (!null(tail(temporary))) {
	       temporary = tail(temporary);
               ListType next_pair = head(temporary);
               K next_key = next_pair.first;
#ifdef FCPP_DEBUG_MAP
  	       std::cout << "next key = " << next_key << std::endl;
#endif
               if(first_key == next_key && uniquekeys )
                      {
#ifdef FCPP_DEBUG_MAP
			std::cout << "found duplicate key " << first_key
                                  << std::endl;
#endif
			// Skip what was found.
			changed = true;
                        just_changed = true;
           	      }
               else if(first_key > next_key)
                      {
#ifdef FCPP_DEBUG_MAP
			std::cout << "found " << first_key << " > "
                                  << next_key << std::endl;
#endif
			new_copy = cat(new_copy,list_with(next_pair));
			changed = true;
                        just_changed = true;
		      } else {
		 	new_copy = cat(new_copy,list_with(first_pair));
			first_key = next_key;
                        first_pair = next_pair;
                        just_changed = false;
		      }
	    }
            if (just_changed) new_copy = cat(new_copy,list_with(first_pair));
	    else new_copy = cat(new_copy,list_with(head(temporary)));
#ifdef FCPP_DEBUG_MAP
            std::cout << "setting copy = new_copy" << std::endl;
#endif
	    copy = new_copy;
#ifdef FCPP_DEBUG_MAP
	    std::cout << "length of copy = " << length(copy) << std::endl;
            if (changed) std::cout << "changed is true" << std::endl;
#endif
	  }
#ifdef FCPP_DEBUG_MAP
	  std::cout << "setting l = copy" << std::endl;
#endif
          l = copy;
	}
      }

    }

    using impl::Map;
    using impl::MultiMap;

  }

}

#endif
