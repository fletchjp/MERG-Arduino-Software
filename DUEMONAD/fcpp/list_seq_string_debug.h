// seq_string  () function from List<T> class.

   std::string seq_string_debug() const {
     std::string temp;
     // Aha! priv_isEmpty actually evaluates if there is a job to be done.
     // That is NOT what is wanted here. Oh dear!
     if( is_empty() ) { // Progress - empty list case.
         temp +=  std::string ("[ ]");
     } else {
       if (has_value()) { // This no longer forces evaluation.
	 temp +=  std::string ("[ ");
         if (fcpp::traits::debug_traits<T>::has_name)
	   // This deals with the case where the list is of functoids.
            temp +=  fcpp::traits::debug_traits<T>::name(head());
         else
	    temp +=  fcpp::traits::argument_traits<T>::value(head());
	  List<T> ll = tail();
          while (ll.has_value()) {
	    temp += std::string(" , ");
         if (fcpp::traits::debug_traits<T>::has_name)
	   // This deals with the case where the list is of functoids.
            temp +=  fcpp::traits::debug_traits<T>::name(ll.head());
         else
	    temp += fcpp::traits::argument_traits<T>::value(ll.head());
            ll = ll.tail();
          } // Print values if there are any
	  // Print this if there is still a function.
          // I could change this to output the actual function when
	  // in debug mode, and also for unevaluated function.
	  // ======================================================
	  // This is interesting. The underlying function in List<T>
	  // is stored as Fun0<OddList<T> > which is not polymorphic.
	  // It does not need to be polymorphic.
	  // However, this means that the debug code as implemented
	  // currently cannot dig deeper and get the functoid name.
	  // So far.
	  // Fun0 is defined such that even when constructed from a
	  // Full0 object it does not store it but converts it.
	  // =======================================================
          if (!ll.is_empty() )
	    {
	      temp +=  std::string (" , ");
  	      if (fcpp::traits::debug_traits<Fun0<OddList<T> > >::has_name) {
	         temp += fcpp::traits::debug_traits<Fun0<OddList<T> > >::name
                 (ll.rep->fxn);
	      }
	    }
          temp +=  std::string (" ]"); // Finish off here
       }  else {
         temp += std::string ("[ ");
	 // I need to find the type to put in here...
	 //if (fcpp::traits::debug_traits<Full0<List<T> > >::has_name) {
	 //temp += fcpp::traits::debug_traits<Full0<List<T> > >::name(rep->fxn);
         //   temp += std::string(" List<T> ");
	 //}
	 //else
         if (fcpp::traits::debug_traits<Fun0<OddList<T> > >::has_name) {
	    temp += fcpp::traits::debug_traits<Fun0<OddList<T> > >::name(rep->fxn);
            //temp += std::string("(OddList<T>)"); // temporary addition.
	 }
         temp += std::string (" ]");
       }
     }
     return temp;
     }
