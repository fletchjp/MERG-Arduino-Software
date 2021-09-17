// add_transitions

// Transitions to be added in setup()

//ADD_TRANSITION(S0,transitionS0S1,S1)
//ADD_TRANSITION(S1,transitionS1S2,S2)
//ADD_TRANSITION(S2,transitionS2S1,S1)

// Add transition from S0 to S1
  addT(0,1)
  // Add transitions from S1 to S2 and back again.
  addT(1,2)
  addT(2,1)
