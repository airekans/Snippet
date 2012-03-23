#ifndef _ZIP_H_
#define _ZIP_H_

#include "concat.h"

namespace zip_foreach {

namespace __impl {

template<class first_iterator, class second_iterator>
  class __ZipPair {
 public:
  typedef __ZipPair<first_iterator, second_iterator> self;
  typedef typename first_iterator::value_type first_type;
  typedef typename second_iterator::value_type second_type;
  typedef typename first_iterator::reference first_reference;
  typedef typename second_iterator::reference second_reference;

  __ZipPair(first_iterator i1, second_iterator i2)
    : first_iter(i1), second_iter(i2)
  {
    
  }
  
  first_reference first()
  {
    return *first_iter;
  }

  second_reference second()
  {
    return *second_iter;
  }

  bool operator==(const self& i) const
  {
    return first_iter == i.first_iter && second_iter == i.second_iter;
  }

  bool operator!=(const self& i) const
  {
    return first_iter != i.first_iter || second_iter != i.second_iter;
  }
 

  first_iterator first_iter;
  second_iterator second_iter;
};

template<class Container1, class Container2>
  class __ZipContainerIterator {
 public:
  typedef __ZipContainerIterator<Container1, Container2> self;
  typedef typename Container1::iterator first_iterator;
  typedef typename Container2::iterator second_iterator;
  typedef __ZipPair<first_iterator, second_iterator> value_type;
  typedef value_type* pointer;
  typedef value_type& reference;

 __ZipContainerIterator(first_iterator it1, Container1& c1,
			second_iterator it2, Container2& c2)
   : m_iterator_pair(it1, it2), m_c1(&c1), m_c2(&c2)
    {
      
    }

  
  reference operator*()
  {
    return m_iterator_pair;
  }

  pointer operator->()
  {
    return &m_iterator_pair;
  }

  self& operator++()
  {
    ++m_iterator_pair.first_iter;
    ++m_iterator_pair.second_iter;
    
    if (m_iterator_pair.first_iter == m_c1->end() ||
	m_iterator_pair.second_iter == m_c2->end())
      {
	m_iterator_pair.first_iter = m_c1->end();
	m_iterator_pair.second_iter = m_c2->end();
      }

    return *this;
  }

  bool operator==(const self& i) const
  {
    return m_iterator_pair == i.m_iterator_pair;
  }

  bool operator!=(const self& i) const
  {
    return m_iterator_pair != i.m_iterator_pair;
  }

 private:
  value_type m_iterator_pair;
  Container1* m_c1;
  Container2* m_c2;
};

template<class Container1, class Container2>
  class ZipContainer {
 public:
  typedef typename Container1::value_type first_type;
  typedef typename Container2::value_type second_type;
  typedef typename Container1::iterator first_iterator;
  typedef typename Container2::iterator second_iterator;
  typedef __ZipContainerIterator<Container1, Container2> iterator;
  

  ZipContainer(Container1& c1, Container2& c2)
    : m_c1(&c1), m_c2(&c2)
    {
      SetEndIterators();
    }

  ZipContainer(const ZipContainer& zc)
    {
      m_c1 = zc.m_c1;
      m_c2 = zc.m_c2;
      SetEndIterators();
    }

  ZipContainer& operator=(ZipContainer& zc)
    {
      if (&zc != this)
	{
	  m_c1 = zc.m_c1;
	  m_c2 = zc.m_c2;
	  SetEndIterators();
	}

      return *this;
    }

  iterator begin()
  {
    if (m_c1 && m_c2)
      {
	if (m_c1->begin() == first_end || m_c2->begin() == second_end)
	  {
	    return iterator(first_end, *m_c1, second_end, *m_c2);
	  }
	else
	  {
	    return iterator(m_c1->begin(), *m_c1, m_c2->begin(), *m_c2);
	  }
      }
    else
      {
	return iterator(first_end, *m_c1, second_end, *m_c2);
      }
  }

  iterator end()
  {
    return iterator(first_end, *m_c1, second_end, *m_c2);
  }

  
 private:
  void SetEndIterators()
  {
    first_end = m_c1->end();
    second_end = m_c2->end();
  }

 private:
  Container1* m_c1;
  Container2* m_c2;

  first_iterator first_end;
  second_iterator second_end;
};

}

template<class Container1, class Container2>
  __impl::ZipContainer<Container1, Container2> zip(Container1& c1, Container2& c2)
{
  __impl::ZipContainer<Container1, Container2> z(c1, c2);
  return z;
}

}

#define ZIP_2_FOREACH(x, y, zc)						\
  for (int CONCAT_LINE(__f) = 1, CONCAT_LINE(__breakFlag) = 1;		\
       CONCAT_LINE(__f); CONCAT_LINE(__f) = 0)				\
    for (BOOST_AUTO(CONCAT_LINE(__zc), (zc));				\
	 CONCAT_LINE(__f); CONCAT_LINE(__f) = 0)			\
      for (BOOST_AUTO(CONCAT_LINE(__i), CONCAT_LINE(__zc).begin());	\
	   CONCAT_LINE(__breakFlag) &&					\
	     CONCAT_LINE(__i) != CONCAT_LINE(__zc).end();		\
	   ++CONCAT_LINE(__i), CONCAT_LINE(__f) = 1)			\
	for (BOOST_AUTO(& x, CONCAT_LINE(__i)->first());		\
	     CONCAT_LINE(__f); CONCAT_LINE(__f) = 0)			\
	  for (BOOST_AUTO(& y, CONCAT_LINE(__i)->second());		\
	       CONCAT_LINE(__f) && (--CONCAT_LINE(__breakFlag) < 1);	\
	       CONCAT_LINE(__f) = 0, CONCAT_LINE(__breakFlag) = 1)

#define TWO_LIST_FOREACH(x, y, l1, l2)		\
  ZIP_2_FOREACH(x, y, zip_foreach::zip(l1, l2))


#endif
