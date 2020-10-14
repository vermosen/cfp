#pragma once
#ifndef OTOS_MODEL_RECORDERS_CFP_H
#define OTOS_MODEL_RECORDERS_CFP_H

#include<Eigen/Core>
#include<Eigen/Dense>

#include <cfp/model/recorder.h>

namespace cfp    {

  template <typename Model>
  class recorder<Model, recorders::type::simple> {
  public:
    using data_type = std::vector<std::vector<double>>;

  public:
    recorder(std::size_t sz, std::size_t maxsteps) {

      m_data.reserve(sz);

      for (std::size_t i = 0; i < sz; i++) {
        m_data.push_back(std::vector<double>());
        m_data.back().reserve(maxsteps);
      }
    }

    void record(const Eigen::VectorXd& record) {

      assert(record.size() == m_data.size());

      for (std::size_t i = 0; i < record.size(); i++)
        m_data[i].push_back(record[i]);
    }
    const data_type& data() const { return m_data; }
  private:
    data_type m_data;
  };
}

#endif // OTOS_MODEL_RECORDERS_CFP_H
