#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
#endif /* __PROGTEST__ */

uint64_t reverseBits(uint64_t character) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (character & 0x00000001);
        character >>= 1;
    }
    return result;
}

void printAsBinaryString(uint64_t value)
{
    for (int i = 64; i >= 0; i--)
    {
        uint64_t mask = 1 << i;
        if (value & mask)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
}

int pow(int base, int exp)
{
    int res = 1;
    for (int i = 0 ; i < exp ; i++)
        res *= base;
    return res;
}

class CBuffer
{
private:
    int m_index;
    uint64_t m_buffer;
    int m_shift; // kolik bytu je v bufferu
    int canWrite()
    {
        uint64_t mask = 0x0000000000000003;
        for (int i = 0 ; i < 64 ; i++)
        {
            if (mask == (mask & m_buffer))
                return i + 2;
            mask <<= 1;
        }
        return -1;
    }
    int countBits(uint64_t character)
    {
        int res = 0;
        while (character != 0 )
        {
            character >>= 1;
            res ++;
        }
        return res;
    }
public:
    CBuffer()
    :m_index(0),
    m_buffer(0x0000000000000000),
    m_shift(0)
    {}
    bool addToBufferUTF(uint64_t character, ostream & out)
    {
        //pridavani do bufferu by melo fungovat dobre
        m_buffer |= (character <<= m_index);
        m_index = countBits(m_buffer);
        //m_buffer <<= m_index;
        //printAsBinaryString(m_buffer);
        //cout << endl;
        while ( m_index >= 8 ){
            const char toWrite = m_buffer & 0x00000000000000ff;
            out.write(&toWrite,sizeof(char));
            if (out.fail())
                return false;
            m_buffer >>= 8;
            m_index -= 8;
        }
        return true;
        //cout << endl;
        //printAsBinaryString(m_buffer);
        //cout << endl;
    }
    bool addToBufferFIB(uint64_t byte, ostream & out)
    {

        m_buffer |= (byte << (m_shift ));
        m_shift += 8;
        //cout << "Buffer s novym charem: " ;
        //printAsBinaryString(m_buffer);
        //cout << endl;
        m_index = canWrite();
        //cout << canWrite() << endl;
        while (m_index != -1)
        {
            //cout << "Co budeme zapisovat : ";
            //printAsBinaryString(getChar());
            uint64_t write = fibBinaryToUint(getChar()); // hodnota cisla, fibonacciho posloupnosti
            //cout << endl << "V desitkove: " ;
            //printAsBinaryString(write);
            m_buffer >>= m_index; // buffer posuneme o cislo, ktere posilame do souboru
            m_shift -= m_index; // do bufferu budeme muset pridavat cislo posunute o velikost, kde koncilo predchozi
            //cout << endl;
            //todo vymyslet jak z velikosti writu udelat spravne velkej uint a poslat ho do souboru
            //funguje prevadeni z fib na uint, nacitani, posouvani bufferu si nejsem jistej pro vetsi
            int bytesToWriteCount = writeLength(write); // kolik bytu bude mit cislo, ktere budeme zapisovat
            if (bytesToWriteCount == -1)
                return false;
            if (bytesToWriteCount == 1)
            {
                const char toWrite = write;
                out.write(&toWrite, sizeof(toWrite));
            }
            else if (bytesToWriteCount == 2)
            {
                uint16_t res = getTwoBytes(write);
                const char second = res;
                res >>= 8;
                const char first = res;
                out.write(&first,sizeof(char));
                out.write(&second,sizeof(char));
            }
            else if (bytesToWriteCount == 3)
            {
                uint32_t res = getThreeBytes(write);
                const char third = res;
                res >>= 8;
                const char second = res;
                res >>= 8;
                const char first = res;
                out.write(&first,sizeof(char));
                out.write(&second,sizeof(char));
                out.write(&third,sizeof(char));
            }
            else if (bytesToWriteCount == 4)
            {
                uint32_t res = getFourBytes(write);
                const char fourth = res;
                res >>= 8;
                const char third = res;
                res >>= 8;
                const char second = res;
                res >>= 8;
                const char first = res;
                out.write(&first,sizeof(char));
                out.write(&second,sizeof(char));
                out.write(&third,sizeof(char));
                out.write(&fourth,sizeof(char));
            }
            //out.write(reinterpret_cast<const char *>(&write),sizeof(write));
            m_index = canWrite(); // zjistime, jestli je jeste nejake cislo v bufferu, ktere bysme mohli zapsat
        }
        m_index = 0; // index toho, kde konci cislo, ktere budeme zapisovat resetujeme
        return true;
        //m_index = countBits(m_buffer);
        //m_buffer <<= 8;
        //cout << fibBinaryToUint() << endl;
    }
    void addRest (ofstream & out) // pro utf8 to fib
    {
        if (m_buffer == 0)
            return;
        const char toWrite = m_buffer & 0x00000000000000ff;
        out.write(&toWrite,sizeof(char));
    }
    bool isEmpty()
    {
        return m_buffer == 0;
    }
    uint64_t fibBinaryToUint(uint64_t binary)
    {
        int numberOfBits = countBits(binary);
        int prev = 0;
        int curr = 1;
        int last = 1;
        uint64_t mask = 0x0000000000000001;
        uint64_t res = 0;
        for ( int i = 0 ; i < numberOfBits -1  ; i++ ){

            if (mask == (binary & mask))
                res += last;
            mask <<= 1;
            prev = curr;
            curr = last;
            last = prev + curr;
        }
        return res -1 ;

    }
    int writeLength(uint64_t write)
    {
        if (write <= 127)
            return 1;
        if ( write <= 2047)
            return 2;
        if ( write <= 65535)
            return 3;
        if ( write <= 1114111)
            return 4;
        else
            return -1;
    }
    uint64_t getChar()
    {
        uint64_t res = 0;
        uint64_t mask = 0x0000000000000001;
        for(int i = 0 ; i < m_index ; i++)
        {
            res |= (m_buffer & mask);
            mask <<= 1;
        }
        return res;
    }
    uint16_t getTwoBytes(uint64_t number)
    {
        uint16_t res = 49280; // = 11000000 10000000
        res |= (number & 0x000000000000003f); // poslednich 6 bitu
        number <<= 2;
        res |= (number & 0x0000000000001f00);
        return res;
    }
    uint32_t getThreeBytes(uint64_t number)
    {
        uint32_t res = 14712960; // = 11100000 10000000 10000000
        res |= (number & 0x000000000000003f);
        number <<= 2;
        res |= (number & 0x0000000000003f00);
        number <<=2;
        res |= (number & 0x00000000000f0000);
        return res;
    }
    uint32_t getFourBytes(uint64_t number)
    {
        uint32_t res = 0b11110000100000001000000010000000; // 11110000 10000000 10000000 10000000
        res |= (number & 0x000000000000003f);
        number <<=2;
        res |= (number & 0x0000000000003f00);
        number <<=2;
        res |= (number & 0x00000000003f0000);
        number <<=2;
        res |= (number & 0x0000000007000000);
        return res;
    }



};

class CByte
{
private:
    unsigned char m_char;
    int m_valueBits;
    int m_numberOfByte; // 0 je nejmensi bit

public:
    CByte(unsigned char characte, int numberOfbyte = 0)
        :m_char(characte),
         m_numberOfByte(numberOfbyte)

    {}
    //ziskame pocet bitu nesouci velikost
    //funguje
    int getValueBits() {return m_valueBits;}

    int numberOfBytes()
    {
        if ((m_char & 0b11111100) >= 248)
        {
            return -1;
        }
        if ((m_char & 0b11111000) == 0b11110000)
        {
            m_valueBits = 3;
            return 4;
        }
        if ((m_char & 0b11110000) == 0b11100000)
        {
            m_valueBits = 4;
            return 3;
        }
        if ((m_char & 0b11100000) == 0b11000000)
        {
            m_valueBits = 5;
            return 2;
        }
        if((m_char & 0b10000000) == 0x00) {
            m_valueBits = 7;
            return 1;
        }
        if ((m_char & 0b11000000) == 0b10000000) {
            m_valueBits = 6;
            return 0;
        }
        return 0;
    }
    int getByteValue()
    {
        if (m_valueBits == 7)
            return (m_char & 0b01111111) * getConstant();
        if (m_valueBits == 6)
            return (m_char & 0b00111111) * getConstant();
        if (m_valueBits == 5)
            return (m_char & 0b00011111) * getConstant();
        if (m_valueBits == 4)
            return (m_char & 0b00001111) * getConstant();
        if (m_valueBits == 3)
            return (m_char & 0b00000111) * getConstant();
        else return -1;
    }
    int getConstant()
    {
        //toto je otestovano, melo by fungovat
        if ( m_valueBits == 3)
            return 64 * 64 * 64;
        if ( m_valueBits== 4)
            return 64 * 64;
        if ( m_valueBits == 5)
            return 64;
        if (m_valueBits == 7)
            return 1;
        else
        {
            return  pow(64, ( m_numberOfByte ));
        }
    }
    uint64_t uintToFib(size_t value)
    {
        //vraci s lsb vpravo a ukoncovaci jednickou
        value ++;
        uint64_t prev = 0;
        uint64_t current = 1;
        uint64_t last = 1;
        uint64_t res = 0x0000000000000001;

        while (value >= last)
        {
            prev = current;
            current = last;
            //if ((prev + current ) > a)
            //    break;
            last = prev + current;
        }

        last = current;
        current = prev;
        prev = last - current;

        while ( true )
        {
            if ( value >= last)
            {
                res <<= 1;
                res |= 0x0000000000000001;
                //res += "1";
                value -= last;
            }
            else
            {
                res <<= 1;
            }
            if ( prev == 0)
                break;
            last = current;
            current = prev;
            prev = last - current;
        }
       // res = (res << 1) | 0x00000001;
        //res = ::reverseBits(res);
        res &= 0x00000000ffffffff;
        return res;
    }

};

bool               utf8ToFibonacci                         ( const char      * inFile,
                                                             const char      * outFile )
{
    ifstream in;
    ofstream out;
    in.open(inFile,ios::binary);
    out.open(outFile,ios::binary);
    if ( !in.is_open() || !out.is_open())
        return false;
    CBuffer buffer;

    while (true)
    {
        uint64_t res = 0;
        //if (in.eof())
        //    break;
        unsigned char input = in.get();
        if (in.eof())
            break;
        if (in.fail())
            return false;
        CByte byte(input);
        int size = byte.numberOfBytes();
        if (size == -1 || size == 0)
            return false;
        res +=byte.getByteValue();
        for (int i = 1; i < size ; i++ )
        {
            CByte byte2(in.get(),(size - 1 )  - i);
            if (in.fail() || in.eof())
                return false;
            byte2.numberOfBytes();
            if (byte2.getValueBits() != 6)
                return false;
            res += byte2.getByteValue();
        }
        //cout << res << endl;
        if (res > 1114111)
            return false;
        uint64_t fib = byte.uintToFib(res);
        //printAsBinaryString(fib);
        //cout << endl;
        if (! buffer.addToBufferUTF(fib,out))
            return false;
    }
    buffer.addRest(out);

    if(in.bad() || out.bad())
        return false;
    in.close();
    out.close();
    return true;
}
  
bool               fibonacciToUtf8                         ( const char      * inFile, 
                                                             const char      * outFile )
{
    ifstream in;
    ofstream out;
    in.open(inFile,ios::binary);
    out.open(outFile,ios::binary);
    if (!in || !out || in.fail() || out.fail())
        return false;
    CBuffer buffer;

    while (true)
    {
        //if (in.eof() || in.bad())
        //    break;
        uint8_t byte = in.get();
        if (in.eof())
            break;
        if (!(buffer.addToBufferFIB(byte,out)))
            return false;
    }
    if (!(buffer.isEmpty()))
        return false;
    if(in.bad() || out.bad())
        return false;
    in.close();
    out.close();
    return true;

}

#ifndef __PROGTEST__

bool               identicalFiles                          ( const char      * file1,
                                                             const char      * file2 )
{
    ifstream inf1;
    ifstream inf2;
    inf1.open(file1, ios::binary);
    inf2.open(file2, ios::binary);
    while (true)
    {
        unsigned char char1,char2;
        char1 = inf1.get();
        char2 = inf2.get();
        if (inf1.eof())
            break;
        if (char1 != char2)
            return false;
    }
    return true;
}

int main ( int argc, char * argv [] )
{
    /*
     * OSETRENO
     * UTF8
     *  - vice bytove znaky, maji vzdy dalsi znaky bajty zacinajici na 10
     *  - pokuj nejde otevrit soubor
     *  -
     *  FIB
     *  -
     */
    CBuffer buffer;
    //printAsBinaryString(buffer.fibBinaryToUint(0b110000101));
//
//    fibonacciToUtf8("example/src_10.fib", "bin.bin");
    assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_0.fib" ) );
    assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_1.fib" ) );
    assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_2.fib" ) );
    assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_3.fib" ) );
    assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
             && identicalFiles ( "output.fib", "example/dst_4.fib" ) );
    assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );
    assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );
    assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
             && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );
    assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
