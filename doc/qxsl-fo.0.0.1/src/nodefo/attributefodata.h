#ifndef ATTRIBUTEFODATA_H
#define ATTRIBUTEFODATA_H

class AttributeFo;

#include <QString>
/* moc file window os look here */
#include <QCoreApplication>
#include <QVariant>

typedef int FoSize;

/**
 * class which represent size value, contains value and type of unit
 */
class SizeValue
{
public:
	/**
	 * types of all possible units
	 */	
	enum EUnitOfSize {mmUnit,cmUnit,inUnit,ptUnit,unresolvedUnit};
	/**
	 * constructor
	 * @param value 
	 * @param unit 
	 */
	SizeValue(FoSize value,EUnitOfSize unit):m_value(value),m_unit(unit){};
	/**
	 * value of size
	 */		
	FoSize m_value;
	/**
	 * types of concrete unit
	 */	
	EUnitOfSize m_unit;
};

/**
 * represent data part of attribute (e.g. "12mm"), all types are here stored in QString, and we get specific types with convert functions 
 */
class AttributeFoData
{
public:
	/**
	 * constructor
	 * @param data data with units (e.g. "12mm") 
	 */
	AttributeFoData(QString data);
	/**
	 * destructor
	 */
	~AttributeFoData();
	/**
	 * convert data to size value
	 * @return 
	 */
	SizeValue sizeValue();
	/**
	 * return value in QString format
	 * @return 
	 */
	QString stringValue();
	/**
	 * return type of unit 
	 * @return 
	 */
	SizeValue::EUnitOfSize unitType();
	/**
	 * convert data to int
	 * @return 
	 */
	int intValue();
	/**
	 * convert data to boolean
	 * @return 
	 */
	bool boolValue();
private:
	/**
	 * whole attribute value is stored in this member (e.g. 12mm)
	 */
	QString m_data;
};

#endif
