// seq_string  () function from List<T> class.

// This is the no_debug version which is fairly readable.

   std::string seq_string_no_debug() const {
     std::string temp;
     // Aha! priv_isEmpty actually evaluates if there is a job to be done.
     // That is NOT what is wanted here. Oh dear!
     if( is_empty() ) { // Progress - empty list case.
         temp +=  std::string ("[ ]");
     } else {
       if (has_value()) { // This no longer forces evaluation.
	 temp +=  std::string ("[ ");
	    temp +=  fcpp::traits::argument_traits<T>::value(head());
	  List<T> ll = tail();
          while (ll.has_value()) {
	    temp += std::string(" , ");
	    temp += fcpp::traits::argument_traits<T>::value(ll.head());
            ll = ll.tail();
          } // Print values if there are any
          if (!ll.is_empty() )
	      temp +=  std::string (" , _");
          temp +=  std::string (" ]"); // Finish off here
       }  else {
	 temp += std::string ("[ _ ]"); // Unevaluated function.
       }
     }
     return temp;
     }
