#ifndef EXPSUM_QR_COL_PIVOT_HPP
#define EXPSUM_QR_COL_PIVOT_HPP

#include "arma/lapack_extra.hpp"

namespace expsum
{

template <typename T>
struct qr_col_pivot
{
    using value_type = T;
    using real_type  = T;
    using size_type  = arma::uword;

    void run(arma::Mat<value_type>& A)
    {
        resize(A.n_rows, A.n_cols);

        auto m     = static_cast<arma::blas_int>(A.n_rows);
        auto n     = static_cast<arma::blas_int>(A.n_cols);
        auto lwork = static_cast<arma::blas_int>(work.n_elem);
        jpiv.zeros();

        invoke(m, n, A.memptr(), m, jpiv.memptr(), tau.memptr(), work.memptr(),
               lwork);
    }

    void resize(size_type m, size_type n)
    {
        jpiv.set_size(n);
        tau.set_size(std::min(m, n));
        const auto lwork = query(static_cast<arma::blas_int>(m),
                                 static_cast<arma::blas_int>(n));
        if (work.size() < lwork)
        {
            work.set_size(lwork);
        }
    }

    // From output matrix of GEQP3, return matrix R * P.t()
    //
    // @A output matrix of `qr_col_pivot::run`
    arma::Mat<value_type> get_RPT(const arma::Mat<value_type>& A) const
    {
        assert(A.n_cols == jpiv.size());
        arma::Mat<value_type> RPT(std::min(A.n_rows, A.n_cols), A.n_cols,
                                  arma::fill::zeros);
        arma::uvec ipiv(jpiv.size());
        for (size_type i = 0; i < jpiv.size(); ++i)
        {
            ipiv(static_cast<size_type>(jpiv(i) - 1)) = i;
        }

        for (size_type i = 0; i < ipiv.size(); ++i)
        {
            auto n = std::min(i + 1, A.n_cols);
            RPT.col(ipiv(i)).head(n) = A.col(i).head(n);
        }

        return RPT;
    }
private:
    arma::Col<arma::blas_int> jpiv;
    arma::Col<real_type> tau;
    arma::Col<real_type> work;

    // Get optimal workspace size
    static size_type query(arma::blas_int m, arma::blas_int n)
    {
        value_type dummy[2];
        arma::blas_int lwork = -1;
        arma::blas_int jpiv[2];
        arma::blas_int info;
        arma::lapack::geqp3(&m, &n, &dummy[0], &m, &jpiv[0], &dummy[0],
                            &dummy[0], &lwork, &info);
        return static_cast<size_type>(dummy[0]);
    }

    static arma::blas_int invoke(arma::blas_int m, arma::blas_int n,
                                 value_type* A, arma::blas_int lda,
                                 arma::blas_int* jpiv, value_type* tau,
                                 value_type* work, arma::blas_int lwork)
    {
        arma::blas_int info;
        arma::lapack::geqp3(&m, &n, A, &lda, jpiv, tau, work, &lwork, &info);

        return info;
    }
};


template <typename T>
struct qr_col_pivot<std::complex<T> >
{
    using value_type = std::complex<T>;
    using real_type  = T;
    using size_type  = arma::uword;

    // Get optimal workspace size
    static size_type query(arma::blas_int m, arma::blas_int n)
    {
        value_type dummy[1];
        arma::blas_int lwork = -1;
        arma::blas_int jpiv[1];
        real_type rwork[2];
        arma::blas_int info;
        arma::lapack::geqp3(&m, &n, &dummy[0], &jpiv[0], &dummy[0], &dummy[0],
                            &lwork, &rwork[0], &info);
        return static_cast<size_type>(std::real(dummy[0]));
    }

    static arma::blas_int invoke(arma::blas_int m, arma::blas_int n,
                                 value_type** A, arma::blas_int* jpiv,
                                 value_type* tau, value_type* work,
                                 arma::blas_int lwork, real_type* rwork)
    {
        arma::blas_int info;
        arma::lapack::geqp3(&m, &n, A, jpiv, tau, work, lwork, rwork, &info);
        return info;
    }
};


} // namespace: expsum


#endif /* EXPSUM_QR_COL_PIVOT_HPP */
