#include "implementOpt.h"
#include "Log.h"

//double myfunc(unsigned n, const double *x, double *grad, void *my_func_data)
//{
//	if (grad) {
//		grad[0] = 0.0;
//		grad[1] = 0.5 / sqrt(x[1]);
//	}
//	return sqrt(x[1]);
//}

//double myconstraint(unsigned n, const double *x, double *grad, void *data)
//{
//	my_constraint_data *d = (my_constraint_data *)data;
//	double a = d->a, b = d->b;
//	if (grad) {
//		grad[0] = 3 * a * (a*x[0] + b) * (a*x[0] + b);
//		grad[1] = -1.0;
//	}
//	return ((a*x[0] + b) * (a*x[0] + b) * (a*x[0] + b) - x[1]);
//}




void testFun()
{
	cout << "this is test\n";

}




