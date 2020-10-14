#pragma once
#ifndef OTOS_RCPP_UTILS_H
#define OTOS_RCPP_UTILS_H

#include <Rcpp.h>

#include "fwd/rcpp-utils.h"

namespace Rcpp      {
namespace RcppEigen {

  // helper trait to identify if T is a plain object type
  // TODO: perhaps move this to its own file
  template <typename T> struct is_plain : Rcpp::traits::same_type<T, typename T::PlainObject> {};

  // helper trait to identify if the object has dense storage
  template <typename T> struct is_dense : Rcpp::traits::same_type<typename T::StorageKind, Eigen::Dense> {};

  // for plain dense objects
  template <typename T>
  SEXP eigen_wrap_plain_dense(const T& obj, Rcpp::traits::true_type) {
    typename Eigen::internal::conditional<T::IsRowMajor,
      Eigen::Matrix<typename T::Scalar,
      T::RowsAtCompileTime,
      T::ColsAtCompileTime>,
      const T&>::type objCopy(obj);
    int m = obj.rows(), n = obj.cols();
    R_xlen_t size = static_cast<R_xlen_t>(m) * n;
    SEXP ans = PROTECT(::Rcpp::wrap(objCopy.data(), objCopy.data() + size));
    if (T::ColsAtCompileTime != 1) {
      SEXP dd = PROTECT(::Rf_allocVector(INTSXP, 2));
      int* d = INTEGER(dd);
      d[0] = m;
      d[1] = n;
      ::Rf_setAttrib(ans, R_DimSymbol, dd);
      UNPROTECT(1);
    }
    UNPROTECT(1);
    return ans;
  }

  // for plain sparse objects
  template <typename T>
  SEXP eigen_wrap_plain_dense(const T& object, Rcpp::traits::false_type) {
    typedef typename T::Scalar     Scalar;
    const int  RTYPE = Rcpp::traits::r_sexptype_traits<Scalar>::rtype;
    std::string klass;
    switch (RTYPE) {
    case REALSXP: klass = T::IsRowMajor ? "dgRMatrix" : "dgCMatrix";
      break;
      //          case INTSXP: klass = T::IsRowMajor ? "igRMatrix" : "igCMatrix";  // classes not exported
      //              break;
    default:
      throw std::invalid_argument("RTYPE not matched in conversion to sparse matrix");
    }
    S4           ans(klass);
    const int    nnz = object.nonZeros();
    ans.slot("Dim") = Dimension(object.rows(), object.cols());
    ans.slot(T::IsRowMajor ? "j" : "i") =
      IntegerVector(object.innerIndexPtr(), object.innerIndexPtr() + nnz);
    ans.slot("p") = IntegerVector(object.outerIndexPtr(),
      object.outerIndexPtr() + object.outerSize() + 1);
    ans.slot("x") = Vector<RTYPE>(object.valuePtr(), object.valuePtr() + nnz);
    return  ans;
  }

  // plain object, so we can assume data() and size()
  template <typename T>
  inline SEXP eigen_wrap_is_plain(const T& obj, ::Rcpp::traits::true_type) {
    return eigen_wrap_plain_dense(obj, typename is_dense<T>::type());
  }

  // when the object is not plain, we need to eval()uate it
  template <typename T>
  inline SEXP eigen_wrap_is_plain(const T& obj, ::Rcpp::traits::false_type) {
    return eigen_wrap_is_plain(obj.eval(), Rcpp::traits::true_type());
  }


  // at that point we know that T derives from EigenBase
  // so it is either a plain object (Matrix, etc ...) or an expression
  // that evaluates into a plain object
  //
  // so the first thing we need to do is to find out so that we don't evaluate if we don't need to
  template <typename T>
  inline SEXP eigen_wrap(const T& obj) {
    return eigen_wrap_is_plain(obj,
      typename is_plain<T>::type()
      );
  }

}}

namespace Rcpp   {
namespace traits {

  template <typename T, typename value_type>
  class matrix_exporter {
  public:
    typedef value_type r_export_type;

    matrix_exporter(SEXP x) : object(x) {}
    ~matrix_exporter() {}

    T get() {
      Shield<SEXP> dims(::Rf_getAttrib(object, R_DimSymbol));
      if (Rf_isNull(dims) || ::Rf_length(dims) != 2) {
        throw ::Rcpp::not_a_matrix();
      }
      int* dims_ = INTEGER(dims);
      T result(dims_[0], dims_[1]);
      value_type* data = result.data();
      ::Rcpp::internal::export_indexing<value_type*, value_type>(object, data);
      return result;
    }

  private:
    SEXP object;
  };

  template <typename T>
  class Exporter<Eigen::Matrix<T, Eigen::Dynamic, 1> >
    : public IndexingExporter<Eigen::Matrix<T, Eigen::Dynamic, 1>, T> {
  public:
    Exporter(SEXP x) : IndexingExporter<Eigen::Matrix<T, Eigen::Dynamic, 1>, T >(x) {}
  };

  template <typename T>
  class Exporter<Eigen::Array<T, Eigen::Dynamic, 1> >
    : public IndexingExporter<Eigen::Array<T, Eigen::Dynamic, 1>, T> {
  public:
    Exporter(SEXP x) : IndexingExporter<Eigen::Array<T, Eigen::Dynamic, 1>, T >(x) {}
  };

  template <typename T>
  class Exporter< Eigen::Matrix<T, 1, Eigen::Dynamic> >
    : public IndexingExporter< Eigen::Matrix<T, 1, Eigen::Dynamic>, T > {
  public:
    Exporter(SEXP x) : IndexingExporter< Eigen::Matrix<T, 1, Eigen::Dynamic>, T >(x) {}
  };

  template <typename T>
  class Exporter< Eigen::Array<T, 1, Eigen::Dynamic> >
    : public IndexingExporter< Eigen::Array<T, 1, Eigen::Dynamic>, T > {
  public:
    Exporter(SEXP x) : IndexingExporter< Eigen::Array<T, 1, Eigen::Dynamic>, T >(x) {}
  };

  template <typename T>
  class Exporter< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> >
    : public matrix_exporter< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>, T > {
  public:
    Exporter(SEXP x) :
      matrix_exporter< Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>, T >(x) {}
  };

  template <typename T>
  class Exporter< Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic> >
    : public matrix_exporter< Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic>, T > {
  public:
    Exporter(SEXP x) :
      matrix_exporter< Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic>, T >(x) {}
  };

  template<typename T>
  class Exporter<Eigen::SparseMatrix<T> > {
  public:
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype;
    Exporter(SEXP x) : d_x(x), d_dims(d_x.slot("Dim")), d_i(d_x.slot("i")), d_p(d_x.slot("p")), xx(d_x.slot("x")) {
      if (!d_x.is("dgCMatrix"))
        throw std::invalid_argument("Need S4 class dgCMatrix for a sparse matrix");
    }
    Eigen::SparseMatrix<T> get() {
      Eigen::SparseMatrix<T>  ans(d_dims[0], d_dims[1]);
      ans.reserve(d_p[d_dims[1]]);
      for (int j = 0; j < d_dims[1]; ++j) {
        ans.startVec(j);
        for (int k = d_p[j]; k < d_p[j + 1]; ++k) ans.insertBack(d_i[k], j) = xx[k];
      }
      ans.finalize();
      return ans;
    }
  protected:
    S4            d_x;
    IntegerVector d_dims, d_i, d_p;
    Vector<RTYPE> xx;
  };

  template<typename T>
  class Exporter<Eigen::SparseMatrix<T, Eigen::RowMajor> > {
  public:
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype;

    Exporter(SEXP x) : d_x(x), d_dims(d_x.slot("Dim")), d_j(d_x.slot("j")), d_p(d_x.slot("p")), xx(d_x.slot("x")) {
      if (!d_x.is("dgRMatrix"))
        throw std::invalid_argument("Need S4 class dgRMatrix for a sparse matrix");
    }
    Eigen::SparseMatrix<T, Eigen::RowMajor> get() {
      Eigen::SparseMatrix<T, Eigen::RowMajor>  ans(d_dims[0], d_dims[1]);
      ans.reserve(d_p[d_dims[0]]);
      for (int i = 0; i < d_dims[0]; ++i) {
        ans.startVec(i);
        for (int k = d_p[i]; k < d_p[i + 1]; ++k) ans.insertBack(i, d_j[k]) = xx[k];
      }
      ans.finalize();
      return ans;
    }
  protected:
    S4            d_x;
    IntegerVector d_dims, d_j, d_p;
    Vector<RTYPE> xx;
  };

  template<typename T>
  class Exporter<Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> > > {
    typedef typename Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> > OUT;
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype;
    Rcpp::Vector<RTYPE> vec;

  public:
    Exporter(SEXP x) : vec(x) {
      if (TYPEOF(x) != RTYPE)
        throw std::invalid_argument("Wrong R type for mapped vector");
    }
    OUT get() { return OUT(vec.begin(), vec.size()); }
  };

  template<typename T>
  class Exporter<Eigen::Map<Eigen::Matrix<std::complex<T>, Eigen::Dynamic, 1> > > {
    typedef typename Eigen::Map<Eigen::Matrix<std::complex<T>, Eigen::Dynamic, 1> > OUT;
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<std::complex<T>>::rtype;
    Rcpp::Vector<RTYPE> vec;

  public:
    Exporter(SEXP x) : vec(x) {
      if (TYPEOF(x) != RTYPE)
        throw std::invalid_argument("Wrong R type for mapped vector");
    }
    OUT get() { return OUT(reinterpret_cast<std::complex<T>*>(vec.begin()), vec.size()); }
  };

  template<typename T>
  class Exporter<Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> > > {
    typedef typename Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> > OUT;
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype;
    Rcpp::Vector<RTYPE> vec;
    int d_ncol, d_nrow;

  public:
    Exporter(SEXP x) 
      : vec(x), d_ncol(1)
      , d_nrow(Rf_xlength(x)) {
      if (TYPEOF(x) != RTYPE)
        throw std::invalid_argument("Wrong R type for mapped matrix");
      if (::Rf_isMatrix(x)) {
        int* dims = INTEGER(::Rf_getAttrib(x, R_DimSymbol));
        d_nrow = dims[0];
        d_ncol = dims[1];
      }
    }
    OUT get() { return OUT(vec.begin(), d_nrow, d_ncol); }
  };

  template<typename T>
  class Exporter<Eigen::Map<Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> > > {
    typedef typename Eigen::Map<Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> > OUT;
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<std::complex<T>>::rtype;
    Rcpp::Vector<RTYPE> vec;
    int d_ncol, d_nrow;

  public:
    Exporter(SEXP x)
      : vec(x), d_ncol(1)
      , d_nrow(Rf_xlength(x)) {
      if (TYPEOF(x) != RTYPE)
        throw std::invalid_argument("Wrong R type for mapped matrix");
      if (::Rf_isMatrix(x)) {
        int* dims = INTEGER(::Rf_getAttrib(x, R_DimSymbol));
        d_nrow = dims[0];
        d_ncol = dims[1];
      }
    }
    OUT get() { return OUT(reinterpret_cast<std::complex<T>*>(vec.begin()), d_nrow, d_ncol); }
  };

  template<typename T>
  class Exporter<Eigen::Map<Eigen::Matrix<T, 1, Eigen::Dynamic> > > {
    typedef typename Eigen::Map<Eigen::Matrix<T, 1, Eigen::Dynamic> > OUT;
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype;
    Rcpp::Vector<RTYPE> vec;

  public:
    Exporter(SEXP x) : vec(x) {
      if (TYPEOF(x) != RTYPE)
        throw std::invalid_argument("Wrong R type for mapped rowvector");
    }
    OUT get() { return OUT(vec.begin(), vec.size()); }
  };

  template<typename T>
  class Exporter< Eigen::Map<Eigen::Array<T, Eigen::Dynamic, 1> > > {
    typedef typename Eigen::Map<Eigen::Array<T, Eigen::Dynamic, 1> > OUT;
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype;
    Rcpp::Vector<RTYPE> vec;

  public:
    Exporter(SEXP x) : vec(x) {
      if (TYPEOF(x) != RTYPE)
        throw std::invalid_argument("Wrong R type for mapped 1D array");
    }
    OUT get() { return OUT(vec.begin(), vec.size()); }
  };

  template<typename T>
  class Exporter<Eigen::Map<Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic> > > {
    typedef typename Eigen::Map<Eigen::Array<T, Eigen::Dynamic, Eigen::Dynamic> > OUT;
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype;
    Rcpp::Vector<RTYPE> vec;
    int d_ncol, d_nrow;

  public:
    Exporter(SEXP x) : vec(x), d_ncol(1), d_nrow(Rf_xlength(x)) {
      if (TYPEOF(x) != RTYPE)
        throw std::invalid_argument("Wrong R type for mapped 2D array");
      if (::Rf_isMatrix(x)) {
        int* dims = INTEGER(::Rf_getAttrib(x, R_DimSymbol));
        d_nrow = dims[0];
        d_ncol = dims[1];
      }
    }
    OUT get() { return OUT(vec.begin(), d_nrow, d_ncol); }
  };

  template<typename T>
  class Exporter<Eigen::Map<Eigen::SparseMatrix<T> > > {
  public:
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype;
    Exporter(SEXP x) : d_x(x), d_dims(d_x.slot("Dim")), d_i(d_x.slot("i")), d_p(d_x.slot("p")), xx(d_x.slot("x")) {
      if (!d_x.is("dgCMatrix"))
        throw std::invalid_argument("Need S4 class dgCMatrix for a mapped sparse matrix");
    }
    Eigen::Map<Eigen::SparseMatrix<T> > get() {
      return Eigen::Map<Eigen::SparseMatrix<T> >(
          d_dims[0], d_dims[1], d_p[d_dims[1]]
        , d_p.begin(), d_i.begin(), xx.begin());
    }
  protected:
    S4            d_x;
    IntegerVector d_dims, d_i, d_p;
    Vector<RTYPE> xx;
  };

  template<typename T>
  class Exporter<Eigen::Map<Eigen::SparseMatrix<T, Eigen::RowMajor> > > {
  public:
    const static int RTYPE = ::Rcpp::traits::r_sexptype_traits<T>::rtype;
    Exporter(SEXP x) : d_x(x), d_dims(d_x.slot("Dim")), d_j(d_x.slot("j")), d_p(d_x.slot("p")), xx(d_x.slot("x")) {
      if (!d_x.is("dgRMatrix"))
        throw std::invalid_argument("Need S4 class dgRMatrix for a mapped sparse matrix");
    }
    Eigen::Map<Eigen::SparseMatrix<T, Eigen::RowMajor> > get() {
      return Eigen::Map<Eigen::SparseMatrix<T, Eigen::RowMajor> >(d_dims[0], d_dims[1], d_p[d_dims[1]],
        d_p.begin(), d_j.begin(), xx.begin());
    }
  protected:
    S4            d_x;
    IntegerVector d_dims, d_j, d_p;
    Vector<RTYPE> xx;
  };
}}

#endif // OTOS_RCPP_UTILS_H