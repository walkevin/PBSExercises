#include <eigen/core>
#include <vector>

namespace sph
{
  typedef Eigen::Array<double, 3, 1> vel;
  typedef Eigen::Array<double, 3, 1> pos;

  class sphSolver
  {
    public:
      sphSolver();
      ~sphSolver();

    private:
      std::vector<sphCell> cells
  }

  class sphCell
  {
    public:
      sphCell();
      ~sphCell();

    private:
       
