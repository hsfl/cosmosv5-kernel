/*!
 * \file jsonvalue.h
 * \brief JSON value variant type.
 * \ingroup cosmos_kernel_json
 * \defgroup cosmos_jsonvalue JSON Value
 *
 * Defines the JsonValue variant that can hold any JSON scalar or container
 * (null, bool, number, string, array, object). Provides type-safe accessors
 * and visitor support. Used by jsonobject.h and jsonclass.h to build
 * structured JSON documents in the kernel layer.
 */

#ifndef JSONVALUE_H
#define JSONVALUE_H
#include "support/configCosmos.h"
#include "math/vector.h"
using namespace Cosmos::Math::Vectors;
using namespace Cosmos::Math::Quaternions;

class JSONObject;
class JSONValue;
typedef vector<JSONValue> JSONArray;


class JSONValue {
public:
    enum class JsonValueType : uint8_t {
            NULL_VALUE = 0,
            STRING = 1,
            NUMBER = 2,
            JSON_OBJECT = 3,
            JSON_ARRAY = 4,
            BOOL_TRUE = 5,
            BOOL_FALSE = 6,
    };
    //! \brief Construct a null JSONValue.
    JSONValue();
    //! \brief Construct a JSONValue from a uint8.
    JSONValue(uint8_t val);
    //! \brief Construct a JSONValue from an int8.
    JSONValue(int8_t val);
    //! \brief Construct a JSONValue from a char (stored as number).
    JSONValue(char val);
    //! \brief Construct a JSONValue from a bool.
    JSONValue(bool val);
    //! \brief Construct a JSONValue from a uint16.
    JSONValue(uint16_t val);
    //! \brief Construct a JSONValue from an int16.
    JSONValue(int16_t val);
    //! \brief Construct a JSONValue from a uint32.
    JSONValue(uint32_t val);
    //! \brief Construct a JSONValue from an int32.
    JSONValue(int32_t val);
    //! \brief Construct a JSONValue from a uint64.
    JSONValue(uint64_t val);
    //! \brief Construct a JSONValue from an int64.
    JSONValue(int64_t val);
    //! \brief Construct a JSONValue from a float.
    JSONValue(float val);
    //! \brief Construct a JSONValue from a double.
    JSONValue(double val);
    //! \brief Construct a JSONValue from a string.
    JSONValue(string val);
    //! \brief Construct a JSONValue from a Vector (serialised as JSON array).
    JSONValue(Vector val);
    //! \brief Construct a JSONValue from a Quaternion (serialised as JSON array).
    JSONValue(Quaternion val);
    //! \brief Construct a JSONValue from a nested JSONObject.
    JSONValue(JSONObject val);
    //! \brief Construct a JSONValue from a JSONArray.
    JSONValue(JSONArray val);

    //! \brief Set the value to JSON null.
    void set_null();
    //! \brief Set the value to a string.
    void set(string val);
    //! \brief Set the value to a float.
    void set(float val);
    //! \brief Set the value to a double.
    void set(double val);
    //! \brief Set the value to a uint64.
    void set(uint64_t val);
    //! \brief Set the value to a uint32.
    void set(uint32_t val);
    //! \brief Set the value to a uint16.
    void set(uint16_t val);
    //! \brief Set the value to a uint8.
    void set(uint8_t val);
    //! \brief Set the value to an int64.
    void set(int64_t val);
    //! \brief Set the value to an int32.
    void set(int32_t val);
    //! \brief Set the value to an int16.
    void set(int16_t val);
    //! \brief Set the value to an int8.
    void set(int8_t val);
    //! \brief Set the value to a Vector.
    void set(Vector val);
    //! \brief Set the value to a Quaternion.
    void set(Quaternion val);
    //! \brief Set the value to a char.
    void set(char val);
    //! \brief Set the value to a nested JSONObject.
    void set(JSONObject val);
    //! \brief Set the value to a JSONArray.
    void set(JSONArray val);
    //! \brief Set the value to a bool.
    void set(bool val);

    //! \brief Return true if this value holds JSON null.
    bool isNull();
    //! \brief Return true if this value holds a string.
    bool isString();
    //! \brief Return true if this value holds a number.
    bool isNumber();
    //! \brief Return true if this value holds an object.
    bool isObject();
    //! \brief Return true if this value holds an array.
    bool isArray();
    //! \brief Return true if this value holds a boolean.
    bool isBool();

    //! \brief Serialise the value to its JSON string representation.
    //! \return JSON string (e.g. "\"hello\"", "42", "true", "null").
    string to_json_string();
private:
    string value_as_string;
    JsonValueType type;
};

#endif // JSONVALUE_H
