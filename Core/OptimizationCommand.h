#pragma once
#include "Command.h"

namespace Core
{
	typedef struct {
		double a, b;
	} my_constraint_data;

	//Command of optimization.
	class OptimizationCommand :
		public Command
	{


		DataWrapper* _outData;

	public:
		OptimizationCommand();
		~OptimizationCommand();

		virtual void Execute(const DataWrapper* data);

		map<string, string> GetOutData() const;

	private:


		_declspec(dllexport)  double TestOptimize(const std::vector<std::string>& lowerBound, std::vector<double>& var);

	public:
		static double myvfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data);

		static double myvconstraint(const std::vector<double> &x, std::vector<double> &grad, void *data);
	};
}


