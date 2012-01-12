#pragma once

#include <limits.h>

#include <set>
#include <vector>
#include <deque>
#include <list>

namespace BWAPI
{
  template<typename _T>
  class iterator;

  template<typename _T>
  class Vectorset;

  template<typename _T>
  class iterator
  {
  public:
    iterator() : __val(0) {};
    iterator(const iterator &i) : __val(*i) {};
    iterator(_T *ptr) : __val(ptr) {};
    bool operator ==(const iterator<_T> &other) const
    {
      return *this->__val == *other;
    };
    bool operator !=(const iterator<_T> &other) const
    {
      return *this->__val != *other;
    };
    bool operator ==(const _T &element) const
    {
      return *this->__val == element;
    };
    bool operator !=(const _T &element) const
    {
      return *this->__val != element;
    };
    iterator &operator ++()
    {
      ++__val;
      return *this;
    };
    iterator operator ++(int)
    {
      iterator copy = *this;
      ++__val;
      return copy;
    };
    iterator &operator --()
    {
      --__val;
      return *this;
    };
    iterator operator --(int)
    {
      iterator copy = *this;
      --__val;
      return copy;
    };
    _T operator *() const
    {
      return *__val;
    };
    _T operator ->() const
    {
      return *__val;
    };
  private:
    _T *__val;
  };

  template<typename _T>
  class Vectorset
  {
  public:
    typedef iterator<_T> iterator;
    ///////////////////////////////////////////////////////////// Constructors
    //Vectorset() : Vectorset(,) { };
    Vectorset(size_t initialSize = 16, size_t maxSizeHint = UINT_MAX)
      : __totSize( initialSize )
      , __maxSizeHint( maxSizeHint )
      , __valArray( (_T*)malloc(initialSize*sizeof(_T)) )
      , __end(__valArray + initialSize)
      , __last(__valArray)
    { };
    Vectorset(const Vectorset &other)
      : __totSize( other.size() )
      , __maxSizeHint( other.max_size_hint() )
      , __valArray( (_T*)malloc(__totSize*sizeof(_T)) )
      , __end(__valArray + __totSize)
      , __last(__valArray + other.size())
    { };
    ///////////////////////////////////////////////////////////// Operators
    Vectorset &operator =(const Vectorset &set)
    {
      // manage existing set
      this->clear();
      this->expand(set.size());

      // copy the data to this set
      size_t copysize = set.size()*sizeof(_T);
      memcpy(this->__valArray, set, copysize);

      // update variables in this set
      this->__last = this->__valArray + set.size();
      return *this;
    };
    Vectorset &operator +=(const Vectorset &set)
    {
      // manage existing set
      this->expand(this->size() + set.size());
      
      // copy the data to this set
      size_t copysize = set.size()*sizeof(_T);
      memcpy(this->__last, set, copysize);

      // update variables in this set
      this->__last += set.size();
      return *this;
    };

    // Misc usage
    operator void*() const
    {
      return this->__valArray;
    };
    operator bool() const
    {
      return !this->empty();
    };

    ///////////////////////////////////////////////////////////// Destructors
    ~Vectorset()
    {
      free(this->__valArray);
    };

    ///////////////////////////////////////////////////////////// some functions
    bool exists(const _T &val) const
    {
      _T *i = this->__valArray, *iend = this->__last;
      do
      {
        if ( val == *i )
          return true;
      } while ( ++i != iend );
      return false;
    };
    void erase(const _T &val) const
    {
      // declare iterators
      _T *i = this->__valArray, *iend = this->__last;

      // iterate everything and store the size diff
      size_t sizediff = 0;
      do
      {
        while ( val == *(i + sizediff) && i + sizediff != iend ) ++sizediff;

        if ( i + sizediff != iend )
          *i = *(i + sizediff);
      } while ( ++i != iend );
      
      // decrease by sizediff
      this->__last -= sizediff;
      return false;
    };
    ///////////////////////////////////////////////////////////// stl spinoffs
    void clear()
    {
      this->__last = this->__valArray;
    };
    size_t size() const
    {
      return ((size_t)this->__last - (size_t)this->__valArray)/sizeof(_T);
    };
    bool empty() const
    {
      return this->__last == this->__valArray;
    };
    size_t max_size_hint() const
    {
      return this->__maxSizeHint;
    };
    size_t max_size() const
    {
      return this->__totSize;
    };

    // iterators
    iterator begin() const
    {
      return iterator(this->__valArray);
    };
    iterator rbegin() const
    {
      return this->__last - 1;
    };
    iterator end() const
    {
      return this->__last;
    };
    iterator rend() const
    {
      return this->__valArray - 1;
    };

    // element insertion
    void insert(const _T val)
    {
      if ( !this->exists(val) )
        this->push_back(val);
    };
    void insert(const iterator &val)
    {
      this->insert(*val);
    };
    void push_back(const _T val)
    {
      if ( this->__last == this->__end )
        this->expand();
      *__last++ = val;
    };
    void push_back(const iterator &val)
    {
      this->push_back(*val);
    };
  private: /////////////////////////////////////////////// private
    // expand container when full
    void expand()
    {
      // localize the variables
      size_t size = this->__totSize;
      size_t oldsize = size;

      // double the size, but don't exceed the maxSizeHint unless it is equal or already over
      if ( size == this->__maxSizeHint || size * 2 < this->__maxSizeHint )
        size *= 2;
      else
        size = this->__maxSizeHint;

      // Reallocate and store the new values
      this->__valArray  = (_T*)realloc(__valArray, size*sizeof(_T));
      this->__end       = __valArray + size;
      this->__last      = __valArray + oldsize;
      this->__totSize = size;
    };
    void expand(size_t expectedSize)
    {
      while ( expectedSize > this->__totSize )
        this->expand();
    };

    // Variables
    _T *__valArray;
    _T *__last;
    _T *__end;
    size_t __totSize;
    size_t __maxSizeHint;
  };

}
