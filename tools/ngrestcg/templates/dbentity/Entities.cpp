// This file generated by ngrestcg
// For more information, please visit: https://github.com/loentar/ngrest
// DO NOT EDIT. ANY CHANGES WILL BE LOST

##var lastNs
##var lastNsEnd
\
#include <ngrest/db/Field.h>
#include <ngrest/db/Db.h>
#include <ngrest/db/Query.h>
#include <ngrest/db/QueryImpl.h>

#include "$(interface.name)Entities.h"
\
\
##foreach $(.structs)
##ifeq($(.isExtern),false)
##include <common/nsopt.cpp>

// $(.name)

const std::string& $(.name)Entity::getName() const
{
    const static std::string name("$(.name)");
    return name;
}

const std::string& $(.name)Entity::getTableName() const
{
    const static std::string tableName("$(.options.*table)");
    return tableName;
}

const std::list<std::string>& $(.name)Entity::getFieldsNames() const
{
    const static std::list<std::string> fieldsNames = {
##foreach $(.fields)
        "$(.name)",
##endfor
    };
    return fieldsNames;
}

const std::string& $(.name)Entity::getFieldsNamesStr() const
{
    const static std::string fieldsNames = "\
##var first 1
##foreach $(.fields)
##ifeq($($first),1)
##var first 0
##else
,\
##endif
$(.name)\
##endfor
";
    return fieldsNames;
}

const std::string& $(.name)Entity::getFieldsArgs() const
{
    const static std::string fieldsNames = "(\
##var first 1
##foreach $(.fields)
##ifeq($($first),1)
##var first 0
##else
,\
##endif
?\
##endfor
)";
    return fieldsNames;
}

const std::list< ::ngrest::Field>& $(.name)Entity::getFields() const
{
    const static std::list< ::ngrest::Field> fields = {
##foreach $(.fields)
        ::ngrest::Field {
            ::ngrest::Field::DataType::\
##switch $(.dataType.type)
##case string
String\
##case enum
Int\
##case generic
##ifeq($(.dataType.name.!match/bool/),true)
Bool\
##else
##ifeq($(.dataType.name.!match/float/),true)
Float\
##else
##ifeq($(.dataType.name.!match/double/),true)
Float\
##else // defaulting to int
Int\
##endif // double/int
##endif // float
##endif // bool
##default
##error Cannot serialize type: $(.dataType)
##endswitch
, // type
            "$(.name)", // name
##ifeq($(.options.*null),true)
            "NULL", // default value
            false, // not null
##else
            "$(.options.*default)", // default value
            true, // not null
##endif
            $(.options.*pk||"false"), // pk
            $(.options.*unique||"false"), // unique
            $(.options.*autoincrement||"false"), // autoincrement
##ifneq($(.options.*fk),) // fk
##var fkTable $(.options.fk.!token/ /)
##var fkField $(.options.fk.!lasttoken/ /)
##var entity
##foreach $(project.interfaces)
##foreach $(interface.structs)
##ifeq($(struct.options.*table),$($fkTable))
##var entity $(struct.nsName)
##endif
##endfor
##endfor
\
##ifeq($($entity),)
##error Cannot find FK reference to table $($fkTable). Referenced from from $(.name) [$(.options.fk)]
##endif
            new ::ngrest::ForeignKey {
                "$(.options.*key||"$(struct.options.table)_fk_$($fkTable)_$($fkField)")",
                getEntityByDataType< $($entity) >(),
                "$($fkField)",
                "$(.options.*onDelete)",
                "$(.options.*onUpdate)"
            }

##else
            nullptr // foreignKey
##endif // fk
        },
##endfor
    };
    return fields;
}

##endif
##endfor
$($lastNsEnd)

namespace ngrest {
##foreach $(.structs)
##ifeq($(.isExtern),false)

template <>
const Entity& getEntityByDataType< $(.nsName) >()
{
    static const $(.nsName)Entity entity;
    return entity;
}

void bindDataToQuery(Query& query, const $(struct.nsName)& data)
{
##var index 0
##foreach $(.fields)
##switch $(.dataType.type)
##case enum
    query.bind($($index), static_cast<int>(data.$(.name)));
##case generic||string
    query.bind($($index), data.$(.name));
##default
##error Cannot serialize type: $(.dataType)
##endswitch
##ifeq($(.options.*null),true)
##error TODO: nullable
##endif
\
##var index $($index.!inc)
\
##endfor
}

##var fieldsCount 0
##foreach $(struct.fields)
##var fieldsCount $($fieldsCount.!inc)
##endfor

void bindDataToQuery(Query& query, const $(struct.nsName)& data, const std::bitset<$($fieldsCount)>& includedFields)
{
    int index = 0;
##var index 0
##foreach $(.fields)
    if (includedFields[$($index)])
##switch $(.dataType.type)
##case enum
      query.bind(index++, static_cast<int>(data.$(.name)));
##case generic||string
      query.bind(index++, data.$(.name));
##default
##error Cannot serialize type: $(.dataType)
##endswitch
##ifeq($(.options.*null),true)
##error TODO: nullable
##endif
\
##var index $($index.!inc)
\
##endfor
}

void readDataFromQuery(Query& query, $(struct.nsName)& data)
{
##var index 0
##foreach $(.fields)
##switch $(.dataType.type)
##case enum
    data.$(.name) = static_cast< $(.dataType) >(query->resultInt($($index)));
##case generic||string
    query.result($($index), data.$(.name));
##default
##error Cannot serialize type: $(.dataType)
##endswitch
##ifeq($(.options.*null),true)
##error TODO: nullable
##endif
\
##var index $($index.!inc)
\
##endfor
}

void readDataFromQuery(Query& query, $(struct.nsName)& data, const std::bitset<$($fieldsCount)>& includedFields)
{
    int index = 0;
##var index 0
##foreach $(.fields)
    if (includedFields[$($index)])
##switch $(.dataType.type)
##case enum
        data.$(.name) = static_cast< $(.dataType) >(query->resultInt(index++));
##case generic||string
        query.result(index++, data.$(.name));
##default
##error Cannot serialize type: $(.dataType)
##endswitch
##ifeq($(.options.*null),true)
##error TODO: nullable
##endif
\
##var index $($index.!inc)
\
##endfor
}


##endif
##endfor

} // namespace ngrest


// debug operator helpers
##foreach $(.structs)
##ifeq($(struct.isExtern),false)
std::ostream& operator<<(std::ostream& out, const $(struct.nsName)& data)
{
    return out\
##foreach $(struct.fields)
 << "\t" << data.$(field.name)\
##endfor
;
}

##endif
##endfor
