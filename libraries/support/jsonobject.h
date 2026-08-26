/*!
 * \file jsonobject.h
 * \brief JSON object type (ordered key-value map).
 * \ingroup cosmos_kernel_json
 * \defgroup cosmos_jsonobject JSON Object
 *
 * Defines the JsonObject type used to represent a JSON object (a mapping from
 * string keys to JsonValue). Provides insert, lookup, and iteration. Part of
 * the lightweight kernel-layer JSON infrastructure; used by jsonvalue.h and
 * jsonclass.h.
 */

#ifndef JSONOBJECT_H
#define JSONOBJECT_H
#include "support/jsonvalue.h"

class JSONObject
{
    struct jsonTuple
    {
        string key;
        JSONValue value;
        jsonTuple(string k, JSONValue v)
        {
            key = k;
            value = v;
        }
    };
public:

    //! \brief Construct an empty JSON object.
    JSONObject();
    //! \brief Construct a JSON object with a single initial key-value pair.
    //! \param key   Member key.
    //! \param value Member value.
    JSONObject(string key, JSONValue value);

    //! \brief Add a generic JSONValue member.
    void addElement(string key, JSONValue value);
    //! \brief Add a nested JSONObject member.
    void addElement(string key, JSONObject value);
    //! \brief Add a JSONArray member.
    void addElement(string key, JSONArray value);
    //! \brief Add a string member.
    void addElement(string key, string value);
    //! \brief Add a double member.
    void addElement(string key, double value);
    //! \brief Add an int64 member.
    void addElement(string key, int64_t value);
    //! \brief Add an int32 member.
    void addElement(string key, int32_t value);
    //! \brief Add an int16 member.
    void addElement(string key, int16_t value);
    //! \brief Add an int8 member.
    void addElement(string key, int8_t value);
    //! \brief Add a uint64 member.
    void addElement(string key, uint64_t value);
    //! \brief Add a uint32 member.
    void addElement(string key, uint32_t value);
    //! \brief Add a uint16 member.
    void addElement(string key, uint16_t value);
    //! \brief Add a uint8 member.
    void addElement(string key, uint8_t value);
    //! \brief Add a boolean member.
    void addElement(string key, bool value);

    //! \brief Serialise the object to a complete JSON string (with outer braces).
    //! \return JSON string representation.
    string to_json_string();

    //! \brief Serialise only the inner members without outer braces.
    string to_json_object();

    //! \brief Append the JSON string representation to \p jstring.
    //! \param jstring String to append to.
    void get_json_string(string &jstring);

    //! \brief Remove all members from the object.
    void clear();

private:
    vector<jsonTuple> contents;
};

#endif // JSONOBJECT_H
