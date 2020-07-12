

namespace cfp {

    template <typename Precision>
    class model {
    public:
        using data_type = Eigen::Matrix<Precision, Eigen::Dynamic, 1>;

    public:
        static constexpr std::size_t dimension = 2;

    public:
        model clone();
        data_type predict();

    private:
        using prow_type   = Eigen::Matrix<Precision, 1, dimension>; 
        using pcol_type   = Eigen::Matrix<Precision, dimension, 1>;
    }
}