//
// Created by Grant Williams on 8/12/16.
//

#ifndef GCOMP_BYTEIO_H
#define GCOMP_BYTEIO_H

#include <type_traits>
#include <iosfwd>

// Family of Template classes used for byte oriented output.
// Specialized classes need to implement putByte()

template<typename T, typename Enable = void>
class output_bytes
{
public:
    // Makes sure you dont instantiate an output_bytes<T> object
    // for a type that doesnt have the specialization.

    output_bytes(...)
    {
        static_assert(!std::is_void<Enable>::value, "Instantiating output_bytes<> without specialization\n");
    };
public:
    void putByte(char){}
}; // output_bytes

template<typename T>
class output_bytes<T, typename std::enable_if<std::is_base_of<std::ostream, T>::value>::type>
{
public:
    output_bytes(T& stream) : m_stream(stream){}

    void putByte(char c)
    {
        m_stream.put(c);
    }

private:
    T& m_stream;
}; // output_bytes

// Specialization of output_bytes for FILE *

template<>
class output_bytes<FILE*>
{
public:
    output_bytes(FILE* pFile) : m_pFile(pFile){}

    void putByte(char c)
    {
        putc(c, m_pFile);
    }

private:
    FILE* m_pFile;
}; // output_bytes

// Family of Template classes used for byte oriented input.
// Specialized class needed to implement getByte()

template<typename T, typename Enable = void>
class input_bytes
{
public:
    // Makes sure you dont instantiate an input_bytes<T> object
    // for a type that doesnt have the specialization.

    input_bytes(...)
    {
        static_assert(!std::is_void<Enable>::value, "Instantiating input_bytes<> without a specialization\n");
    };

public:
    int getByte();

}; // input_bytes

// Specialization of input_bytes for class istream

template<typename T>
class input_bytes<T, typename std::enable_if<std::is_base_of<std::istream, T>::value>::type>
{
public:
    input_bytes(T& stream) : m_stream(stream){}

    int getByte()
    {
        return m_stream.get();
    }

private:
    T& m_stream;
}; // input_bytes

// Specialization of input_bytes for FILE *

template<>
class input_bytes<FILE *>
{
public:
    input_bytes(FILE *pFile) : m_pFile(pFile){}

    int getByte()
    {
        return getc(m_pFile);
    }

private:
    FILE* m_pFile;
};




#endif //GCOMP_BYTEIO_H
