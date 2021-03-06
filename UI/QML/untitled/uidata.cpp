#include "uidata.h"

QVector<QString> UIData::getVecInequalFunc() const
{
    return _vecInequalFunc;
}

void UIData::pushInequalFunc(const QString value)
{
    _vecInequalFunc.push_back(value);
}

void UIData::pushGradofInequalFunc(const QString value)
{
    _vecGradient_InequalFunc.push_back(value);
}

QVector<QString> UIData::getGradofInequalFunc() const
{
    return _vecGradient_InequalFunc;
}





UIData::UIData()
{
    
}

UIData::~UIData()
{
    
}

QVector<QString> UIData::vecLowerBnd() const
{
    return _vecLowerBnd;
}



void UIData::pushLowerBnd(const QString value)
{
    _vecLowerBnd.push_back(value);
}

void UIData::pushvariableName(const QString value)
{
    _vecVariableName.push_back(value);
}

QVector<QString> UIData::vecUpperBnd() const
{
    return _vecUpperBnd;
}

void UIData::pushUpperBnd(const QString value)
{
    _vecUpperBnd.push_back(value);
}

QVector<QString> UIData::vecGradient() const
{
    return _vecGradient;
}

void UIData::pushGradient(const QString value)
{
    _vecGradient.push_back(value);
}

QVector<QString> UIData::vecInitialValue() const
{
    return _vecinitialValue;
}

void UIData::pushInitialValue(const QString value)
{
    _vecinitialValue.push_back(value);
}

QString UIData::objectFunction() const
{
    return _objectFunction;
}

void UIData::setObjectFunction(const QString &objectFunction)
{
    _objectFunction = objectFunction;
}

QVector<std::string> UIData::vecNameKeys() const
{
    return _vecNameKeys;
}

void UIData::pushNameKey(const std::string aKey)
{

    _vecNameKeys.push_back(aKey);
}


QVector<QString> UIData::vecVariableName() const
{
    return _vecVariableName;
}

QString UIData::getoptMethod() const
{
    return _optMethod;
}

void UIData::setOptMethod(const QString &optMethod)
{
    _optMethod = optMethod;
}
