// promoteN.hpp

#ifndef PROMOTEN_HPP
#define PROMOTEN_HPP

template<typename... Ts> class PromotionN;

struct CannotPromoteWithNoTypes {};

CannotPromoteWithNoTypes horrible_problem;

template<>
class PromotionN<>
{
  public: 
  typedef CannotPromoteWithNoTypes ResultT;
};

template<typename T1> 
class PromotionN<T1> 
{
  public:
  typedef T1  ResultT;
};


template<typename T1, typename T2> 
class PromotionN<T1,T2> {
  public:
  typedef typename Promotion <T1, T2>::ResultT  ResultT;
};


template<typename T1, typename... T2toN>
class PromotionN<T1,T2toN...> {
  public:
  typedef typename PromotionN <T1,typename PromotionN<T2toN...>::ResultT>::ResultT ResultT;
};


#endif
