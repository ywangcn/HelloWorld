
#include "DebugCppAndQml.h"
#include "nlopt.hpp"
//using namespace Core;



namespace testCPPQml
{
	double TestOptimize(const vector<double>& lowerBound, vector<double>& var)
	{

		nlopt::opt opt(nlopt::LD_MMA, 2);

		opt.set_lower_bounds(lowerBound);

		////Set the object function
		opt.set_min_objective(OptimizationCommand::myvfunc, NULL);

		////coefficients for the constraint
		my_constraint_data data[2] = { { 2,0 },{ -1,1 } };

		//Set the constraint function
		opt.add_inequality_constraint(OptimizationCommand::myvconstraint, &data[0], 1e-8);
		opt.add_inequality_constraint(OptimizationCommand::myvconstraint, &data[1], 1e-8);

		opt.set_xtol_rel(1e-4);

		std::vector<double> x(2);
		x[0] = 1.234; x[1] = 5.678;
		double minf;
		nlopt::result result = opt.optimize(x, minf);

		var.clear();
		var.push_back(x[0]);
		var.push_back(x[1]);
		return minf;
	}
	void test()
	{
		//std::cout << "Hello From >lib";
	}
}