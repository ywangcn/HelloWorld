#pragma once
#include "Command.h"
//#include "NloptPara.h"

class NloptPara;
class Grad;
namespace Core
{
	class OptimizationData;
	class OptimizationResult;
	//class CompileGradExcut;
	

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

		unordered_map<string, string> GetOutData() const;

	private:


		_declspec(dllexport)  double TestOptimize(const std::vector<std::string>& lowerBound, std::vector<double>& var);

		//Excute Nlopt optimization
		_declspec(dllexport) void CallNloptOptimize(const OptimizationData* optData, NloptPara* nloptPara, OptimizationResult* optResult);

		////Modify the object function input from user
		string modifyObjectFunc(const OptimizationData* optData);

		////Modify the inequality constraints input from user
		vector<string> ModifyInequalConstraints(const OptimizationData* optData);

		////Modify the Gradient function input from user
		std::vector<string> ModifyGrad(const OptimizationData* optData);

		////Modify the Gradient function of the inequality constraint input from user
		std::vector<vector<string>> ModifyGrad_Inequal(const OptimizationData* optData);

		////Change the variable from unordered_map to a vector<double> that contains all the variable.
		vector<double> changeVariableToVector(const unordered_map<string, string> &variableKeyValue);

		////common tool. Change the variable name to index. For example x1 to x[0]
		string ChangeNameToIndex(string originString, const OptimizationData* optData);


		//Push Grad Pointer to the static vector in the class ObjFuncExcut
		//void PushGradPointer(const int totalVariable);

		////Set the output value.
		void SetOptResult(OptimizationResult * optResult, const vector<double> x, const double minf);

		void CompileGrad(const string pathGrad, const string pathExcu, const OptimizationData* optData, NloptPara* nloptPara);

		vector<vector<Grad*>> OneDToTwoD(int varNum, const vector<Grad*> oneDGrad);

		void CompileInequalCnstFunc(const string pathIneqCnstfunc, const string pathExcu, const OptimizationData* optData, NloptPara* nloptPara);

	public:
		static double myvfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data);

		static double myvconstraint(const std::vector<double> &x, std::vector<double> &grad, void *data);
	};
}


