#include "attributefodata.h"

//qt include
#include <QString>

//project includes


AttributeFoData::AttributeFoData(QString data)
{
	m_data=data;
}


AttributeFoData::~AttributeFoData()
{
	
}

SizeValue::EUnitOfSize AttributeFoData::unitType()
{
	if (m_data.contains("mm"))
		return SizeValue::mmUnit;
	else if (m_data.contains("cm"))
		return SizeValue::cmUnit;
	else if (m_data.contains("in"))
		return SizeValue::inUnit;
	else if (m_data.contains("pt"))
		return SizeValue::ptUnit;
	//if we cant assign unit something is wrong4
	qFatal("Can't resolv unit type");
	return SizeValue::unresolvedUnit;
}

bool AttributeFoData::boolValue()
{
	if (m_data=="true")
		return true;
	else if(m_data=="false")
		return false;
	else
	{
		qFatal("Can't resolv type for bool value");
		return false;
	}
}

int AttributeFoData::intValue()
{
	QString strValue;
	
/*	if (unitType()==SizeValue::mmUnit)
		strValue=m_data.remove("mm");
	else if (unitType()==SizeValue::cmUnit)
		strValue=m_data.remove("cm");
	else if (unitType()==SizeValue::inUnit)
		strValue=m_data.remove("in");
	else if (unitType()==SizeValue::ptUnit)
		strValue=m_data.remove("pt");
*/
	//all units have 2 characters, simple solution:
	strValue=m_data.left(m_data.length()-2);
		
	bool ok;
	int intValue = strValue.toInt(&ok);
	
	if (ok==false)
		qFatal("can't convert value in foattribute to int");

	return intValue;
}

SizeValue AttributeFoData::sizeValue()
{
	int value=intValue();
	SizeValue::EUnitOfSize unit=unitType();
	return SizeValue(value,unit);
}

QString AttributeFoData::stringValue()
{
	return m_data;
}
