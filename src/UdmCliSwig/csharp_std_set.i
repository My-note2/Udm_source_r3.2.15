/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * std_set.i
 *
 * SWIG typemaps for std::set<T>
 * C# implementation
 * The C# wrapper is made to look and feel like a C# System.Collections.Generic.List<> collection.
 * For .NET 1 compatibility, define SWIG_DOTNET_1 when compiling the C# code; then the C# wrapper is 
 * made to look and feel like a typesafe C# System.Collections.ArrayList.
 *
 * Note that IEnumerable<> is implemented in the proxy class which is useful for using LINQ with 
 * C++ std::set wrappers. The IList<> interface is also implemented to provide enhanced functionality
 * whenever we are confident that the required C++ operator== is available. This is the case for when 
 * T is a primitive type or a pointer. If T does define an operator==, then use the SWIG_STD_SET_ENHANCED
 * macro to obtain this enhanced functionality, for example:
 *
 *   SWIG_STD_SET_ENHANCED(SomeNamespace::Klass)
 *   %template(VectKlass) std::set<SomeNamespace::Klass>;
 *
 * Warning: heavy macro usage in this file. Use swig -E to get a sane view on the real file contents!
 * ----------------------------------------------------------------------------- */

// Warning: Use the typemaps here in the expectation that the macros they are in will change name.


%include <std_common.i>

// MACRO for use within the std::set class body
%define SWIG_STD_SET_MINIMUM_INTERNAL(CSINTERFACE, CONST_REFERENCE_TYPE, CTYPE...)
%typemap(csinterfaces) std::set<CTYPE > "IDisposable, System.Collections.IEnumerable\n#if !SWIG_DOTNET_1\n    , System.Collections.Generic.CSINTERFACE<$typemap(cstype, CTYPE)>\n#endif\n";
%typemap(cscode) std::set<CTYPE > %{
  public $csclassname(System.Collections.ICollection c) : this() {
    if (c == null)
      throw new ArgumentNullException("c");
    foreach ($typemap(cstype, CTYPE) element in c) {
      this.Add(element);
    }
  }

  public int Count {
    get {
      return (int)size();
    }
  }

#if !SWIG_DOTNET_1
  System.Collections.Generic.IEnumerator<$typemap(cstype, CTYPE)> System.Collections.Generic.IEnumerable<$typemap(cstype, CTYPE)>.GetEnumerator() {
    return new $csclassnameEnumerator(this);
  }
#endif

  System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
    return new $csclassnameEnumerator(this);
  }

  public $csclassnameEnumerator GetEnumerator() {
    return new $csclassnameEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class $csclassnameEnumerator : System.Collections.IEnumerator
#if !SWIG_DOTNET_1
    , System.Collections.Generic.IEnumerator<$typemap(cstype, CTYPE)>
#endif
  {
    private $csclassname collectionRef;
    private SWIGTYPE_p_void iterator;
    private object currentObject;
    private int currentSize;

    public $csclassnameEnumerator($csclassname collection) {
      collectionRef = collection;
      iterator = null;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public $typemap(cstype, CTYPE) Current {
      get {
        if (iterator == null)
          throw new InvalidOperationException("Enumeration not started.");
        //if (collectionRef.IteratorAtEnd(iterator))
        //  throw new InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new InvalidOperationException("Collection modified.");
        return ($typemap(cstype, CTYPE))currentObject;
      }
    }

    // Type-unsafe IEnumerator.Current
    object System.Collections.IEnumerator.Current {
      get {
        return Current;
      }
    }

    public bool MoveNext() {
      if (iterator == null)
      {
        iterator = collectionRef.IteratorNew();
      }
      if (collectionRef.IteratorAtEnd(iterator))
      {
        return false;
      }
      currentObject = collectionRef.IteratorDeref(iterator);
      collectionRef.IteratorNext(iterator);
      return true;
    }

    public void Reset() {
      // TODO: delete iterator
      iterator = null;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new InvalidOperationException("Collection modified.");
      }
    }

#if !SWIG_DOTNET_1
    public void Dispose() {
        iterator = null;
        // TODO: delete iterator
        currentObject = null;
    }
#endif
  }
%}

  public:
    typedef size_t size_type;
    typedef CTYPE value_type;
    typedef CONST_REFERENCE_TYPE const_reference;
    %rename(Clear) clear;
    void clear();
    %rename(Add) insert;
    void insert(const value_type& x);
    size_type size() const;
    size_type max_size() const;
//    void reserve(size_type n);
    %newobject GetRange(int index, int count);
    %newobject Repeat(const value_type& value, int count);
    set();
    set(const set &other);
    
    class iterator;
    %extend {
      void* IteratorNew() {
        std::set<CTYPE >::iterator* ret = new std::set<CTYPE >::iterator();
        (*ret) = $self->begin();
        return ret;
      }
      bool IteratorAtEnd(void *it) {
        return *(std::set<CTYPE >::iterator*)it == $self->end();
      }
      void IteratorNext(void *it) {
        (*(std::set<CTYPE >::iterator*)it)++;
      }
      CTYPE IteratorDeref(void *it) {
        return **(std::set<CTYPE >::iterator*)it;
      }
      // Takes a deep copy of the elements unlike ArrayList.AddRange
      // FIXME: the argument isn't mapped to the same type as this
      //void AddRange(const std::set<CTYPE >& values) {
      //  $self->insert(values.begin(), values.end());
      //}
    }
%enddef

%define SWIG_STD_SET_MINIMUM(CTYPE...)
SWIG_STD_SET_MINIMUM_INTERNAL(IEnumerable, const value_type&, CTYPE)
%enddef

// Extra methods added to the collection class if operator== is defined for the class being wrapped
// The class will then implement IList<>, which adds extra functionality
%define SWIG_STD_SET_EXTRA_OP_EQUALS_EQUALS(CTYPE...)
    %extend {
      bool Contains(const value_type& value) {
        return std::find($self->begin(), $self->end(), value) != $self->end();
      }
      int IndexOf(const value_type& value) {
        int index = -1;
        std::set<CTYPE >::iterator it = std::find($self->begin(), $self->end(), value);
        if (it != $self->end())
          index = (int)(it - $self->begin());
        return index;
      }
      int LastIndexOf(const value_type& value) {
        int index = -1;
        std::set<CTYPE >::reverse_iterator rit = std::find($self->rbegin(), $self->rend(), value);
        if (rit != $self->rend())
          index = (int)($self->rend() - 1 - rit);
        return index;
      }
      bool Remove(const value_type& value) {
        std::set<CTYPE >::iterator it = std::find($self->begin(), $self->end(), value);
        if (it != $self->end()) {
          $self->erase(it);
          return true;
        }
        return false;
      }
    }
%enddef

// Macros for std::set class specializations/enhancements
%define SWIG_STD_SET_ENHANCED(CTYPE...)
namespace std {
  template<> class set<CTYPE > {
    SWIG_STD_SET_MINIMUM_INTERNAL(IList, const value_type&, CTYPE)
    SWIG_STD_SET_EXTRA_OP_EQUALS_EQUALS(CTYPE)
  };
}
%enddef

// Legacy macros
%define SWIG_STD_SET_SPECIALIZE(CSTYPE, CTYPE...)
#warning SWIG_STD_SET_SPECIALIZE macro deprecated, please see csharp/std_set.i and switch to SWIG_STD_SET_ENHANCED
SWIG_STD_SET_ENHANCED(CTYPE)
%enddef

%define SWIG_STD_SET_SPECIALIZE_MINIMUM(CSTYPE, CTYPE...)
#warning SWIG_STD_SET_SPECIALIZE_MINIMUM macro deprecated, it is no longer required
%enddef

%{
#include <set>
#include <algorithm>
#include <stdexcept>
%}

%csmethodmodifiers std::set::getitemcopy "private"
%csmethodmodifiers std::set::getitem "private"
%csmethodmodifiers std::set::setitem "private"
%csmethodmodifiers std::set::size "private"
%csmethodmodifiers std::set::capacity "private"
%csmethodmodifiers std::set::reserve "private"

namespace std {
  // primary (unspecialized) class template for std::set
  // does not require operator== to be defined
  template<class T> class less { };
  template<class T, Comp=less<T> > class set {
    SWIG_STD_SET_MINIMUM(T)
  };
  // specializations for pointers
//  template<class T> class set<T*> {
//    SWIG_STD_SET_MINIMUM_INTERNAL(IList, const value_type&, T*)
//    SWIG_STD_SET_EXTRA_OP_EQUALS_EQUALS(T*)
//  };
//  template<class T> class set<const T*> {
//    SWIG_STD_SET_MINIMUM_INTERNAL(IList, const value_type&, const T*)
//    SWIG_STD_SET_EXTRA_OP_EQUALS_EQUALS(const T*)
//  };
  // bool is a bit different in the C++ standard
//  template<> class set<bool> {
//    SWIG_STD_SET_MINIMUM_INTERNAL(IList, bool, bool)
//    SWIG_STD_SET_EXTRA_OP_EQUALS_EQUALS(bool)
//  };
}

// template specializations for std::set
// these provide extra collections methods as operator== is defined
SWIG_STD_SET_ENHANCED(char)
SWIG_STD_SET_ENHANCED(signed char)
SWIG_STD_SET_ENHANCED(unsigned char)
SWIG_STD_SET_ENHANCED(short)
SWIG_STD_SET_ENHANCED(unsigned short)
SWIG_STD_SET_ENHANCED(int)
SWIG_STD_SET_ENHANCED(unsigned int)
SWIG_STD_SET_ENHANCED(long)
SWIG_STD_SET_ENHANCED(unsigned long)
SWIG_STD_SET_ENHANCED(long long)
SWIG_STD_SET_ENHANCED(unsigned long long)
SWIG_STD_SET_ENHANCED(float)
SWIG_STD_SET_ENHANCED(double)
SWIG_STD_SET_ENHANCED(std::string) // also requires a %include <std_string.i>

