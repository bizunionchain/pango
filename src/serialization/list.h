// Copyright (c) 2014-2015, The BUC Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include "serialization.h"

namespace serialization
{
  namespace detail
  {
    template <typename Archive, class T>
    bool serialize_list_element(Archive& ar, T& e)
    {
      return ::do_serialize(ar, e);
    }

    template <typename Archive>
    bool serialize_list_element(Archive& ar, uint64_t& e)
    {
      ar.serialize_varint(e);
      return true;
    }
  }
}

template <template <bool> class Archive, class T>
bool do_serialize(Archive<false> &ar, std::list<T> &l)
{
  size_t cnt;
  ar.begin_array(cnt);
  if (!ar.stream().good())
    return false;
  l.clear();

  // very basic sanity check
  if (ar.remaining_bytes() < cnt) {
    ar.stream().setstate(std::ios::failbit);
    return false;
  }

  for (size_t i = 0; i < cnt; i++) {
    if (i > 0)
      ar.delimit_array();
    l.push_back(T());
    T &t = l.back();
    if (!::serialization::detail::serialize_list_element(ar, t))
      return false;
    if (!ar.stream().good())
      return false;
  }
  ar.end_array();
  return true;
}

template <template <bool> class Archive, class T>
bool do_serialize(Archive<true> &ar, std::list<T> &l)
{
  size_t cnt = l.size();
  ar.begin_array(cnt);
  for (typename std::list<T>::iterator i = l.begin(); i != l.end(); ++i) {
    if (!ar.stream().good())
      return false;
    if (i != l.begin())
      ar.delimit_array();
    if(!::serialization::detail::serialize_list_element(ar, *i))
      return false;
    if (!ar.stream().good())
      return false;
  }
  ar.end_array();
  return true;
}
