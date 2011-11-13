/*
 * Scroom - Generic viewer for 2D data
 * Copyright (C) 2009-2011 Kees-Jan Dijkzeul
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License, version 2, as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef BOOKKEEPING_HH
#define BOOKKEEPING_HH

#include <map>
#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <scroom/utilities.hh>
#include <scroom/stuff.hh>

namespace Scroom
{
  namespace Bookkeeping
  {
    using Scroom::Utils::Stuff;
    using Scroom::Utils::StuffList;

    namespace Detail
    {
      class Token;
      class TokenAddition;
    }

    class Token : public boost::shared_ptr<Detail::Token>
    {
    public:
      Token(boost::shared_ptr<Detail::Token> t);
      Token(Detail::Token* t);
      Token(boost::weak_ptr<Detail::Token> t);
      Token();
      Token(Stuff s);
      Token(const StuffList& l);

      void add(Stuff s);
      void add(const StuffList& l);
      void merge(Token& rhs);
      void merge(StuffList& l);

    public:
      Detail::TokenAddition& operator+(const Stuff& rhs);
      Token& operator+=(const Stuff& rhs);
    };
    
    typedef boost::weak_ptr<Detail::Token> WeakToken;

    template<typename K, typename V>
    class MapBase : public virtual Scroom::Utils::Base, public boost::noncopyable
    {
    private:
      std::map<K,V> map;

    public:
      Token add(const K& k, const V& v);
      void remove(const K& k);
      void set(const K& k, const V& v);
      const V& get(const K& k) const;
      V& get(const K& k);
      std::list<K> keys() const;
      std::list<V> values() const;
    };

    template<typename K, typename V>
    class Map : public MapBase<K,V>
    {
    public:
      typedef boost::shared_ptr<Map<K, V> > Ptr;

    public:
      static Ptr create();
    };

    template<typename V>
    class Map<Token, V> : public MapBase<Token,V>
    {
    public:
      typedef boost::shared_ptr<Map<Token, V> > Ptr;

    public:
      static Ptr create();
      
    public:
      void addMe(const Token& k, const V& v);
      Token add(const V& v);
      Token add(const Token& k, const V& v);
    };
    
    template<typename V>
    class Map<WeakToken, V> : public MapBase<WeakToken,V>
    {
    public:
      typedef boost::shared_ptr<Map<WeakToken, V> > Ptr;

    public:
      static Ptr create();

    public:
      void addMe(const WeakToken& k, const V& v);
      Token add(const V& v);
      Token add(const WeakToken& k, const V& v);
    };
    
  }
}


#include <scroom/impl/bookkeepingimpl.hh>


#endif
