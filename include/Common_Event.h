// rethink with mordern c++

#pragma once

//#include <cstdint>
#include <memory>
//#include <string>
//#include <vector>
//#include "Data_Dictionary_Types.h"
//#include "Metadata_Dictionary.h"
#include "Timestamp.h"
//#include "TextFileLine.h"
//#include "ReadOnlyValueVector.h"
//#include "WritableValueVector.h"

// class Event_Metadata;
class Common_Event;

class Common_Event
{
    // friend class CLogfileReader;

public:
    enum TY_Element_Type
    {
        TY_REAL_TYPE = 1, /**< = 1. */
        TY_INTEGER_TYPE,  /**< = 2. */
        TY_BOOLEAN_TYPE,  /**< = 3. */
        TY_STRING_TYPE,   /**< = 4. */
        TY_ENUM_TYPE,     /**< = 5. */
        TY_HEX_TYPE,      /**< = 6. */
        TY_EVENT_TYPE     /**< = 7. */
    };
    enum TY_Tolerance_Source
    {
        TY_METADATA = 1,
        TY_TOLERANCELIST
    };
    enum TY_IgnoreFieldName_Source
    {
        TY_METADATA_DONOTCOMPARELIST = 1,
        TY_IGNOREFIELDNAMELIST
    };

    using TY_Real = double;
    using TY_Integer = int32_t;
    using TY_Boolean = bool;
    using TY_String = std::string;
    using TY_Enum = std::string;
    using TY_Hex = uint32_t;
    using TY_Event = std::string;

    using Ptr = std::unique_ptr<Common_Event>; /*typedef counted_ptr<Common_Event> Ptr;*/

    static Ptr Create()
    {
        return std::make_unique<Common_Event>(); // std::make_unique
                                                 // May throw std::bad_alloc or any exception thrown by the
                                                 // constructor of T. If an exception is thrown, this function has no
                                                 // effect.
    }

    Ptr Clone()
    {
		return std::make_unique<Common_Event>(*this);
    }

    Common_Event() = default;
    virtual ~Common_Event();

    unsigned int get_data_elements_count() const;
    std::string get_event_name() const;
    std::string get_event_source() const;
    CTimestamp get_event_timestamp() const;
};
