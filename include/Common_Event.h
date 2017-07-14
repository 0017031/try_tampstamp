// rethink with mordern c++

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
//#include "Data_Dictionary_Types.h"
//#include "Metadata_Dictionary.h"
//#include "Timestamp.h"
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

    typedef double TY_Real;
    typedef long TY_Integer;
    typedef bool TY_Boolean;
    typedef std::string TY_String;
    typedef std::string TY_Enum;
    typedef std::uint32_t TY_Hex;
    /*typedef unsigned long TY_Hex;*/
    typedef std::string TY_Event;

    using Ptr = std::unique_ptr<Common_Event>;
    /*typedef counted_ptr<Common_Event> Ptr;*/

    static Ptr Create()
    {
        /*Ptr sp(new Common_Event());
        if (!sp.valid())
          throw std::bad_alloc();*/

        auto sp = std::make_unique<Common_Event>(); // std::make_unique
        return sp;                                  // May throw std::bad_alloc or any exception thrown by the
                                                    // constructor of T. If an exception is thrown, this function has no
                                                    // effect.
    }

    Ptr Clone()
    {
        /*Ptr sp(new Common_Event(*this));
        if (!sp.valid())
          throw std::bad_alloc();*/

        auto sp = std::make_unique<Common_Event>(*this);
        return sp;
    }

	Common_Event() = default;
    virtual ~Common_Event();

    unsigned int get_data_elements_count() const;
	std::string get_event_name() const;
	std::string get_event_source() const;
	CTimestamp get_event_timestamp() const;

};
