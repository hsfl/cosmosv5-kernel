/*!
 * \file jsonclass.h
 * \brief Structural JSON document class for the kernel layer.
 * \ingroup cosmos_kernel_json
 * \defgroup cosmos_jsonclass JSON Class
 *
 * Provides a complete JSON document class (parse, build, serialise) built on
 * jsonvalue.h and jsonobject.h. Use when kernel-layer code needs to read or
 * emit JSON without depending on the heavier agent-layer jsonlib.
 */

#ifndef JSONCLASS_H
#define JSONCLASS_H

#include "support/configCosmos.h"
//#include "support/cosmos-errno.h"

namespace Cosmos
{
    namespace Support
    {
        class Json
        {
        public:
            Json(string json="");

            //! JSON Basic Types
            //! enum defining the basic types of value to be found in a JSON member
            enum class Type : uint16_t
                {
                Undefined,
                Null,
                False,
                True,
                Number,
                String,
                Array,
                Object
                };


            //! JSON Value
            //! Structure containg the Value portion of a JSON Member
            struct Value
            {
                //        string name;
                Type type = Type::Undefined;
                string svalue;
                double nvalue;
                bool bvalue;
                map<string, Value> object;
                vector<Value> array;
                string::iterator begin;
                string::iterator end;
            };

            typedef pair<string, Value> Member;
            typedef map<string, Value> Object;
            typedef vector<Value> Array;

            //! JSON Member elements
            //! Structure containing the 2 basic parts of a JSON member, plus some supporting information.
            //    struct Member
            //    {
            //        string name;
            //        Value value;
            //    };

            //! JSON Object
            //! Complete JSON Object, containing both the string of characters and a heirarchical vector of values.
            //    struct Object
            //    {
            //        vector <Member> members;
            //    };

            //! \brief Return the Type of the top-level member named \p name.
            //! \param name Member key.
            //! \return Type enum value.
            Type contents_type(string name);

            //! \brief Return the string value of member \p name.
            string contents_string(string name);

            //! \brief Return the numeric value of member \p name.
            double contents_number(string name);

            //! \brief Return the boolean value of member \p name.
            bool contents_bool(string name);

            //! \brief Return the nested Object value of member \p name.
            Object contents_object(string name);

            //! \brief Return the Array value of member \p name.
            Array contents_array(string name);

            //! \brief Parse a JSON string and populate ObjectContents / ArrayContents.
            //! \param json Well-formed JSON string.
            //! \return 0 on success, negative error code on parse failure.
            int32_t extract_contents(string json);

            //! \brief Recursively parse all key-value members from [begin, end).
            //! \param begin Iterator pointing at the first character after '{'.
            //! \param end   Iterator pointing at '}'.
            //! \param members Output object map.
            //! \return Number of members parsed, or negative error code.
            int32_t extract_members(string::iterator &begin, string::iterator &end, Object &members);

            //! \brief Recursively parse all values in a JSON array from [begin, end).
            //! \param begin Iterator pointing at the first character after '['.
            //! \param end   Iterator pointing at ']'.
            //! \param values Output array vector.
            //! \return Number of values parsed, or negative error code.
            int32_t extract_values(string::iterator &begin, string::iterator &end, Array &values);

            //! \brief Extract a JSON string token from [bit, eit).
            //! \param bit     Iterator positioned at the opening '"'; advanced past closing '"'.
            //! \param eit     End-of-input iterator.
            //! \param ostring Receives the unescaped string content.
            //! \return 0 on success, negative error code on failure.
            int32_t extract_string(string::iterator &bit, string::iterator &eit, string &ostring);

            //! \brief Extract a JSON member name token from [bit, eit).
            //! \param bit  Iterator positioned at the opening '"'; advanced past ':'.
            //! \param eit  End-of-input iterator.
            //! \param name Receives the member key string.
            //! \return 0 on success, negative error code on failure.
            int32_t extract_name(string::iterator &bit, string::iterator &eit, string &name);

            //! \brief Extract a single JSON value (object, array, string, number, bool, null).
            //! \param bit   Iterator positioned at the first character of the value.
            //! \param eit   End-of-input iterator.
            //! \param value Receives the parsed Value.
            //! \return 0 on success, negative error code on failure.
            int32_t extract_value(string::iterator &bit, string::iterator &eit, Value &value);

            //! \brief Extract a complete JSON member (name + value) pair.
            //! \param bit   Iterator positioned at the opening '"' of the name.
            //! \param eit   End-of-input iterator.
            //! \param value Receives the parsed Member.
            //! \return 0 on success, negative error code on failure.
            int32_t extract_member(string::iterator &bit, string::iterator &eit, Member &value);

            //! \brief Advance \p bit past a specific expected character, returning an error if missing.
            //! \param bit       Iterator to advance.
            //! \param eit       End-of-input iterator.
            //! \param character Expected character.
            //! \return 0 on success, negative error code if character not found.
            int32_t skip_character(string::iterator &bit, string::iterator &eit, const char character);

            //! \brief Advance \p bit to the next occurrence of any character in \p characters.
            //! \param bit        Iterator to advance.
            //! \param eit        End-of-input iterator.
            //! \param characters Set of characters to stop at.
            //! \return 0 on success, negative error code if none found.
            int32_t skip_to_character(string::iterator &bit, string::iterator &eit, string characters);

            //! \brief Advance \p bit past any leading whitespace characters.
            //! \param bit Iterator to advance.
            //! \param eit End-of-input iterator.
            //! \return 0 on success.
            int32_t skip_white(string::iterator &bit, string::iterator &eit);

            string Content;
            Type JType;
            Object ObjectContents;
            Array ArrayContents;
            //    vector <Member> Members;

        private:

            string::iterator begin;
            string::iterator end;
        };


    }
}
#endif // JSONCLASS_H
