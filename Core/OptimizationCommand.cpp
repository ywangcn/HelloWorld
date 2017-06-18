#include "OptimizationCommand.h"

#include "Log.h"
#include "DataWrapper.h"
#include "DataParser.h"
#include "OptimizationData.h"
#include "FunctionParser.h"
#include "IFunction.h"
#include "CommonFunc.h"
#include "ObjFuncExcut.h"

#include <sstream>
#include <nlopt.hpp>
#include <math.h>
//#include "GradPasser.h"


namespace Core
{
	using namespace CommonTool;

	OptimizationCommand::OptimizationCommand():_outData(new DataWrapper())
	{
	}


	OptimizationCommand::~OptimizationCommand()
	{
		delete _outData;
	}


	void OptimizationCommand::Execute(const DataWrapper* data)
	{

		////objFunc is the string that contains the modified object function.
			DataParser dataParser(data);
			dataParser.Parse();
			OptimizationData optData = dataParser.GetParsedData();
		string objFunc = modifyObjectFunc(&optData);

		//Parse the objective function.
		ObjectFuncParser parser;
		auto func = parser.Parse(objFunc, "..\\Core\\ObjectFunction.cpp");
		parser.DynamicCompile("ObjectFunction");
		vector<string> grad = ModifyGrad(&optData);


		/////YueWangDebug Dynamic Compiling
		ExcuteGradParser excuPasser;
		auto funcExcu = excuPasser.Parse(objFunc, "..\\Core\\CompileGradExcut.cpp");
		excuPasser.DynamicCompile("CompileGradExcut");
		////YueWangDebug Dynamic Compiling
		//pass the gradient
		GradPasser gradPasser;
		for (size_t i = 0; i<grad.size(); i++)
		{
			auto funcGrad = gradPasser.Parse(grad[i], "..\\Core\\Grad",i);				
		}
		gradPasser.DynamicCompile("Grad");



		

		if (func == NULL)
		{
			Log::Info("func is null");
		}
		else
		{
			callNloptOptimize(&optData);

			vector<std::string> varNameKeys = data->getVecVariableNameKeys();
			for (int i = 0; i < varNameKeys.size(); i++)
			{
				Core::CommonTool::Log::Info("variable Name Keys " + varNameKeys[i]);
			}
		}

	}

	string OptimizationCommand::modifyObjectFunc(const OptimizationData* optData)
	{
		string modifiedObjFun = optData->GetObjFunc();////Object function input by the user
		return ChangeNameToIndex(modifiedObjFun, optData);
		
		
		/*
		////will be modified and returned
		string modifiedObjFun= optData->GetObjFunc();////Object function input by the user

		int varNumber = optData->VarCount();
		int indexInx = 0;
		for (size_t i = 1; i<=varNumber; i++) 
		{
			auto varData = optData->GetVarData(i);
			string value = varData.VarName();

				///find and replace the variable
				size_t pos = modifiedObjFun.find(value, 0);
				while (pos != string::npos)////if find the occurance
				{
					string replace = "x[" + std::to_string(indexInx) + "]";
					modifiedObjFun.replace(pos, value.length(), replace);

					pos = modifiedObjFun.find(value, 0);
				}
				indexInx++;

		}


		return modifiedObjFun;*/

	
	}
	std::vector<string> OptimizationCommand::ModifyGrad(const OptimizationData * optData)
	{
		int varNumber = optData->VarCount();

		////will be returned.
		std::vector<string> grad(varNumber);
		
		int indexInx = 0;
		for (size_t i = 1; i <= varNumber; i++)
		{
			auto varData = optData->GetVarData(i);
			string modifiedGrad = varData.Grad();

			//change xi to x[i-0]. Start

			//int varNumber = optData->VarCount();
			//int indexInx = 0;
			//for (size_t i = 1; i <= varNumber; i++)
			//{
			//	auto varData = optData->GetVarData(i);
			//	string value = varData.VarName();

			//	///find and replace the variable
			//	size_t pos = modifiedGrad.find(value, 0);
			//	while (pos != string::npos)////if find the occurance
			//	{
			//		string replace = "x[" + std::to_string(indexInx) + "]";
			//		modifiedGrad.replace(pos, value.length(), replace);

			//		pos = modifiedGrad.find(value, 0);
			//	}
			//	indexInx++;

			//}

			//change xi to x[i-0]. End

			grad[i - 1] = ChangeNameToIndex(modifiedGrad, optData);
			//grad[i - 1] = modifiedGrad;
		}

		return grad;
	}
	vector<double> OptimizationCommand::changeVariableToVector(const unordered_map<string, string> & variableKeyValue)
	{
		//////make a copy of data, and eraze object function, so that only numbers are remained.
		unordered_map<string, string> allKeys= variableKeyValue;

		unordered_map<string, string>::const_iterator it;
		it = allKeys.find("objFunc");
		allKeys.erase(it);

		//vector<double> x(variableKeyValue.size());
		vector<double> x;
		rsize_t i = 0;
		for (unordered_map<string, string>::iterator it = allKeys.begin(); it!= allKeys.end(); it++)
		{
			////the order of the variable is the same as in the key			
			x.push_back(std::stod(it->second));
		}

		return x;
	}
	string OptimizationCommand::ChangeNameToIndex(string  modified, const OptimizationData* optData)
	{

		int varNumber = optData->VarCount();
		int indexInx = 0;
		for (size_t i = 1; i <= varNumber; i++)
		{
			auto varData = optData->GetVarData(i);
			string value = varData.VarName();

			///find and replace the variable
			size_t pos = modified.find(value, 0);
			while (pos != string::npos)////if find the occurance
			{
				string replace = "x[" + std::to_string(indexInx) + "]";
				modified.replace(pos, value.length(), replace);

				pos = modified.find(value, 0);
			}
			indexInx++;

		}



		return modified;

	}
	unordered_map<string, string> OptimizationCommand::GetOutData() const
	{
		return _outData->GetAllData();
	}

	double OptimizationCommand::TestOptimize(const std::vector<std::string>& lowerBound, std::vector<double>& var)
	{
		nlopt::opt opt(nlopt::LD_MMA, 2);

		vector<double> lb;
		for (auto lb_str : lowerBound)
		{
			lb.push_back(StringToNum<double>(lb_str));
		}
		opt.set_lower_bounds(lb);


		////Set the object function
		opt.set_min_objective(myvfunc, NULL);

		////coefficients for the constraint
		my_constraint_data data[2] = { { 2,0 },{ -1,1 } };

		//Set the constraint function
		opt.add_inequality_constraint(myvconstraint, &data[0], 1e-8);
		opt.add_inequality_constraint(myvconstraint, &data[1], 1e-8);

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

	void OptimizationCommand::callNloptOptimize(const OptimizationData* optData)
	{

		////Get the total amount of variable.
		size_t numberVariable = optData->VarCount();
		
		
		//Set lower bound, upper bound, initial value...
		vector<double> lb(numberVariable);
		vector<double> ub(numberVariable);
		vector<double> x(numberVariable);

		for (size_t index = 0; index<numberVariable; index++)
		{
			auto varData = optData->GetVarData(index+1);////the index start from one
			lb[index] = varData.Lb();
			ub[index] = varData.Ub();
			x[index]=varData.InitVal();
		}


		nlopt::opt opt(nlopt::LD_MMA, x.size());//LD_MMA //GN_DIRECT//LD_AUGLAG_EQ
		opt.set_lower_bounds(lb);
		opt.set_upper_bounds(ub);

		////Set static value for ObjFuncExcut::ObjFunction
		//ObjFuncExcut::_numOfVar = numberVariable;

		////Set the object function	
		opt.set_min_objective(ObjFuncExcut::ObjFunction, NULL);


		////Set the constraint function
		//opt.add_inequality_constraint(myvconstraint, &data[0], 1e-8);
		//opt.add_inequality_constraint(myvconstraint, &data[1], 1e-8);

		opt.set_xtol_rel(1e-4);


		double minf;
		nlopt::result result = opt.optimize(x, minf);
		

	}

	void OptimizationCommand::PushGradPointer(const int totalVariable)
	{
		for (size_t i = 0; i<totalVariable; i++)
		{
			string className = "Grad";
			string indexStr = std::to_string(i);
			className += indexStr;
			//ObjFuncExcut::pushGradPnt(className);
		}
	}

	double OptimizationCommand::myvfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data)
	{
		Log::PushNDC("Optimization");

		double obj = sqrt(x[1]);

		Log::Info("Objective value is %lf.", obj);

		if (!grad.empty()) {
			grad[0] = 0.0;
			grad[1] = 0.5 / sqrt(x[1]);
		}

		Log::PopNDC();

		return obj;
	}

	double OptimizationCommand::myvconstraint(const std::vector<double> &x, std::vector<double> &grad, void *data)
	{
		my_constraint_data *d = reinterpret_cast<my_constraint_data*>(data);
		double a = d->a, b = d->b;
		if (!grad.empty()) {
			grad[0] = 3 * a * (a*x[0] + b) * (a*x[0] + b);
			grad[1] = -1.0;
		}
		return ((a*x[0] + b) * (a*x[0] + b) * (a*x[0] + b) - x[1]);
	}
}

