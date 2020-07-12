

namespace cfp {

    template <
      typename Model
    > class parameters {
    private:
        friend Model;
        using precision_t = typename Model::precision_t;
        using data_t = Eigen::Matrix<precision_t, Dynamic, 1>; // internal representation of the parameter set

    public:
        parameter();
        ~parameter();
        parameters(const parameters&);
        const parameters<Model>& operator=(const parameters<Model>&);
        
        // accessors
    public:
        precision_t psi(std::size_t);

    private:
        data_t m_data;
    }
}