#ifndef ATTRIBUTEFO_H
#define ATTRIBUTEFO_H

#include <QObject>
#include <QString>

#include "attributefodata.h"

/**
 * represent fo-atribute of foobject, contain enumerator of attribute types and appropriate convert funkcions (e.g. from enum to string)
 * also contain group of attributes, group specify value type of attribute
 */
class AttributeFo
{
public:
	/**
	 * enumerator for type of allignment
	 */
	enum EAlignValues
	{
		left_align,right_align,center_align,unknown_align
	};
	
	/**
	 * group for each attribute, only one group could by assigned to attribute
	 */
	enum EAttributeGroup
	{
		noneGroup,sizeGroup,colorGroup,fontFamilyGroup,alignGroup,stringGroup,numberGroup,boolGroup,
	};
	
	/**
	 * here all all possible attributes which are supported (hm .. not all are supported lol)
	 * each type is represented by "string name" which is present in fo document
	 */	
	enum EAttributeType
	{
		unknown=0,
		margin_top=1,margin_bottom=2,margin_left=3,margin_right=4,margin=5,
		page_height=6,page_width=7,
		extent=8,
		font_size=9,font_family=10,
		line_height=11,
		background_color=12,color=13,
		text_align=14,
		padding_top=15,
		left=16,top=17,width=18,height=19,
		src=20,content_width=21,
		precedence=22,
	};
	/**
	 * 
	 * @param id 
	 * @param defaultValue 
	 */
	AttributeFo(EAttributeType id,QString defaultValue);
	/**
	 * destructor
	 */
	~AttributeFo();
	/**
	 * @return group of attributefo object
	 */
	AttributeFo::EAttributeGroup group() const;
	/**
	 * return type name of attribute in enumerator
	 * @return 
	 */
	EAttributeType type() const {	return m_type;}	
	/**
	 * return type name of attribute in qstring
	 * @return 
	 */
	QString name() const;
private:
	/**
	 * hold type of curent attribute
	 */
	EAttributeType m_type;
	/**
	 * currently not used
	 */
	QString m_defaultValue;	
public:
	////////////////////
	// static methods //
	////////////////////
	/**
	 * convert string representation (in fodocument) of attribute to enum EAttributeType
	 * @param name 
	 * @return 
	 */
	static AttributeFo::EAttributeType stringToType(QString name);
	/**
	 * convert enum EAttributeType to string representation (in fodocument) of attribute
	 * @param enumName 
	 * @return 
	 */
	static QString typeToString(EAttributeType enumName);
	/**
	 * convert string representation of align type (in fodocument) to enum EAlignValues
	 * @param name 
	 * @return 
	 */
	static AttributeFo::EAlignValues stringToAlignType(QString name);
};

#endif

///todo:should handle default values
