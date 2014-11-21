// Copyright (c) 2014, LAAS-CNRS
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of hpp-manipulation-urdf.
// hpp-manipulation-urdf is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// hpp-manipulation-urdf is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// hpp-manipulation-urdf. If not, see <http://www.gnu.org/licenses/>.

#include <hpp/util/debug.hh>

#include <boost/algorithm/string.hpp>

#include "hpp/manipulation/srdf/factories/sequence.hh"

namespace hpp {
  namespace manipulation {
    namespace srdf {
      namespace Predicate {
        struct IsEmpty : public std::unary_function<std::string, bool>{
          bool operator () (std::string s) const {return s.empty ();}
        };
      }

      template <typename ValueType> bool cast (const std::string& str, ValueType* val)
      {
        hppDout (error, "Unkown type.");
        return false;
      }

      template <> bool cast <int> (const std::string& str, int* val)
      {
        return XMLUtil::ToInt (str.c_str (), val);
      }

      template <> bool cast <unsigned int> (const std::string& str, unsigned int* val)
      {
        return XMLUtil::ToUnsigned (str.c_str (), val);
      }

      template <> bool cast <double> (const std::string& str, double* val)
      {
        return XMLUtil::ToDouble (str.c_str (), val);
      }

      template <> bool cast <float> (const std::string& str, float* val)
      {
        return XMLUtil::ToFloat (str.c_str (), val);
      }

      template <> bool cast <bool> (const std::string& str, bool* val)
      {
        return XMLUtil::ToBool (str.c_str (), val);
      }

      template <typename ValueType>
      void SequenceFactory<ValueType>::addTextChild (const XMLText* text)
      {
        values_.clear ();
        std::string t(text->Value ());
        typedef std::list<std::string> StringList;
        StringList values;

        boost::algorithm::split (values, t,
            boost::algorithm::is_any_of (" \n\t\r"),
            boost::algorithm::token_compress_on);
        values.remove_if (Predicate::IsEmpty());
        if (size_ > 0 && values.size () != size_) {
          throw std::invalid_argument ("Wrong sequence size");
        }

        ValueType v;
        for (StringList::const_iterator it = values.begin ();
            it != values.end (); it++) {
          if (!cast <ValueType> (*it, &v)) {
            v = 0;
            hppDout (error, "could not parse value "<< *it);
          }
          values_.push_back (v);
        }
      }

      template class SequenceFactory <bool>;
      template class SequenceFactory <int>;
      template class SequenceFactory <unsigned int>;
      template class SequenceFactory <double>;
      template class SequenceFactory <float>;
    } // namespace srdf
  } // namespace manipulation
} // namespace hpp