using rt_real = float;
using rt_int = signed int;
using rt_uint = unsigned int;

const double rtPI = 3.141592653589793238462643383279;
const double rtDoublePI = rtPI * 2.0;
const double rtHalfPI = rtPI * 0.5;

// Numerical Limit (1e-6 = 0.000001, using between 2 float numbers to do minus operation)
constexpr rt_real rt_epsilon = static_cast<rt_real>(1e-6);

const float rt_identity_matrix_epsilon_default = 10e-3f;