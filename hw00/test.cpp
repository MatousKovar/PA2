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

const uint8_t mask4bytes = 0x07; // pro prvni cislo z 4 bytoveho charu
const uint8_t mask3bytes = 0x0F; // pro prvni cislo z 3 bytoveho charu
const uint8_t mask2bytes = 0x1F; // 2 bytovy
const uint8_t maskbytes =  0x3F; // zbyle cisla vice bytovych cisel
const uint8_t mask1bytes = 0x7F; // 1 byte

int pow(int base, int exp)
{
    int res = 1;
    for (int i = 0 ; i < exp ; i++)
        res *= base;
    return res;
}
//buffer bude ukladat bity do bitsetu, a bude se starat o to aby data poslal do out streamu
class CBuffer
{
private:
    std::bitset<64> buffer;
    int index;
    // zjistime jestli jsou dve jednicky po sobe v bitsetu
    int containsNumber()
    {
        //otestovano, pravdepodobne funkcni
        int i = 0;
        int j = 1;
        for ( ; j < index ; j ++, i++ )
        {
            if ((buffer[i] == 1) && (buffer[j] == 1))
                return j;
        }
        return -1;
    }
    int getNumberOfBytes(size_t number)
    {
        if (number <= 127)
            return 1;
        if (number <= 2047)
            return 2;
        if (number <= 65535)
            return 3;
        if (number <= 1114111)
            return 4;
        return -1;
    }

    //ziska binarni zapis cisla z bufferu - stale ve fibonacci coding
    string getNumberBinary(int pos)
    {
        //otestovano, funguje
        string res;
        for (int i = 0 ; i <= pos  ; i++ )
        {
            if (buffer.test(i))
                res.insert(res.begin(),'1');
            else
                res.insert(res.begin(),'0');
        }

        return res;
    }
    string decToBinary(size_t number)
    {
        std::bitset<32> numb(number);
        string res = numb.to_string();

        return res;
    }
    //ziskame hodnotu cisla v 10 soustave
    size_t getIntValue(string & number)
    {
        //funguje
        size_t prev = 0;
        size_t current = 1;
        size_t val = 1;
        size_t res = 0;
        for (size_t i = number.length() - 1  ; i > 0 ; --i)
        {
            if (number[i] == '1')
                res += val;
            //cout << val << " ";
            prev = current;
            current = val;
            val = current + prev;
        }
        res --;
        return res;
    }
public:
    CBuffer(int index = 0)
    :
        index(index)
    {}
    //davame stringy do bitsetu, stringy jsou opacne - nejmensi bit vlevo
    //potreba davat pozor na to jesli se nam tam vejde dalsi cislo
    // po kazdym zapisu do bufferu zkusime, kolik bytu muzeme poslat do souboru
    bool addToBufferUTF(string & binary, ostream & out)
    {
        for( int i = 0; i < binary.length(); i++, index++)
        {
            if (binary[i] == '1')
                buffer.set(index);
            else
                buffer.reset(index );
        }
        //cout << buffer.to_string() << endl;
        //kontrolni vypis

        while(index >= 8)
        {
            const char toWrite = static_cast<unsigned char>(buffer.to_ulong() & 0xff);
            buffer >>= 8;
            out.write(&toWrite,sizeof(char));
            index -=8;
        }
        //cout << buffer.to_string() << endl;
        //cout << endl;
        return true;
    }
    void addRest(ostream & out)
    {
        //cout << buffer.to_string() << endl;
        //buffer >>= 8;
        const char toWrite = static_cast<unsigned char>(buffer.to_ulong() & 0xff);
        if (toWrite == 0)
            return;
        out.write(&toWrite,sizeof(char));
    }
    bool addToBufferFIB( char byte, ostream & out)
    {
        std::bitset<8> bit(byte);
        for (size_t i = 0; i < 8 ; i++,index++) {
            if (bit[i] == 1)
                buffer.set(index);
            else
                buffer.reset(index);
        }
        cout << buffer.to_string() << " " << index << endl;
        while (true) {
            int i = containsNumber();
            if (i == -1)
                break;

            string number = getNumberBinary(i); //nacte cislo ve fib z bufferu
            size_t decimal = getIntValue(number); // jaka je hodnota cisla
            string binary = decToBinary(decimal); // binarni cislo jako string
            int numberOfBytes = getNumberOfBytes(decimal); // na kolik bytu bude cislo
            if (numberOfBytes == -1)
                return false;
            if (numberOfBytes == 1)
            {
                std::bitset<8> res(decimal);
                cout << res.to_string() << endl;
                const char character = res.to_ulong();
                out.write(&character,sizeof(char));
            }
            if (numberOfBytes == 2)
            {
                std::bitset<16> tmp(decimal);
                cout << tmp.to_string() << endl;
                std::bitset<16> res;
                res.set(15).set(14).reset(13).set(7).reset(6);
                int j = 0;
                for (int i = 0 ; i < 6; i++, j++)
                {
                    if (tmp[j] == 1)
                        res.set(i);
                    else res.reset(i);
                }
                for (int i = 8 ; i < 13 ; i ++, j++ )
                {
                    if (tmp[j] == 1)
                        res.set(i);
                    else res.reset(i);
                }
                const char char1 =   res.to_ulong() & 0xff;
                const char char2 = ( res.to_ulong() >> 8 )& 0xff;
                out.write(&char2, sizeof(char));
                out.write(&char1, sizeof(char));

            }
            if (numberOfBytes == 3)
            {
                std::bitset<24> tmp(decimal);
                cout << tmp.to_string() << endl;
                std::bitset<24> res;
                res.set(23).set(22).set(21).reset(20).set(15).reset(14).set(7).reset(6);
                int j = 0;
                for (int i = 0 ; i < 6; i++, j++)
                {
                    if (tmp[j] == 1)
                        res.set(i);
                    else res.reset(i);
                }
                for (int i = 8 ; i < 14 ; i ++, j++ )
                {
                    if (tmp[j] == 1)
                        res.set(i);
                    else res.reset(i);
                }
                for (int i = 16 ; i < 20 ; i ++, j++ )
                {
                    if (tmp[j] == 1)
                        res.set(i);
                    else res.reset(i);
                }
                const char char1 =   res.to_ulong() & 0xff;
                const char char2 = ( res.to_ulong() >> 8 )& 0xff;
                const char char3 = ( res.to_ulong() >> 16 ) & 0xff;
                out.write(&char3, sizeof(char));
                out.write(&char2, sizeof(char));
                out.write(&char1, sizeof(char));

            }
            if (numberOfBytes == 4)
            {
                std::bitset<32> tmp(decimal);
                cout << tmp.to_string() << endl;
                std::bitset<32> res;
                res.set(31).set(30).set(29).set(28).reset(27).set(23).reset(22).set(15).reset(14).set(7).reset(6);
                int j = 0;
                for (int i = 0 ; i < 6; i++, j++)
                {
                    if (tmp[j] == 1)
                        res.set(i);
                    else res.reset(i);
                }
                for (int i = 8 ; i < 14 ; i ++, j++ )
                {
                    if (tmp[j] == 1)
                        res.set(i);
                    else res.reset(i);
                }
                for (int i = 16 ; i < 22 ; i ++, j++ )
                {
                    if (tmp[j] == 1)
                        res.set(i);
                    else res.reset(i);
                }
                for (int i = 24 ; i < 27 ; i ++, j++ )
                {
                    if (tmp[j] == 1)
                        res.set(i);
                    else res.reset(i);
                }
                const char char1 =   res.to_ulong() & 0xff;
                const char char2 = ( res.to_ulong() >> 8 )& 0xff;
                const char char3 = ( res.to_ulong() >> 16 ) & 0xff;
                const char char4 = ( res.to_ulong() >> 24 ) & 0xff;
                out.write(&char4, sizeof(char));
                out.write(&char3, sizeof(char));
                out.write(&char2, sizeof(char));
                out.write(&char1, sizeof(char));

            }
            buffer >>= i + 1;
            index -= i + 1;
        }
        cout << buffer.to_string() << " " << index << endl;
        return true;
    }
    bool cleanBuffer()
    {
        return buffer.to_ulong() == 0;
    }

};
class CByte {
    unsigned char m_binary;
    std::bitset<8> m_bit;
    size_t m_valuableBits; // pocet bytu nesoucich hodnotu
    int m_NumberOfByte; //poradi bytu v charakteru
private:
    //otestovano, funguje dobre
    void GetBits() {
        for (size_t i = 0; i < 8; i++) {
            m_bit[i] = ((m_binary >> i) & 1U);
        }
    }
    int getConstant()
    {
        //toto je otestovano, melo by fungovat
        if ( m_valuableBits == 3)
            return 64 * 64 * 64;
        if ( m_valuableBits == 4)
            return 64 * 64;
        if ( m_valuableBits == 5)
            return 64;
        if (m_valuableBits == 7)
            return 1;
        else
        {
            return  pow(64, ( m_NumberOfByte ));
        }
    }

public:
    CByte(unsigned char byte, int numberOfByte = 1)
            : m_binary(byte),
             m_NumberOfByte(numberOfByte)
            {}


    size_t GetSizeUTF() {
        //otestovano, funguje
        GetBits();
        if (m_bit[7] == 1 && m_bit[6] == 1 && m_bit[5] == 1 && m_bit[4] == 1 && m_bit[3] == 0) {
            m_valuableBits = 3;
            return 4;
        }
        if (m_bit[7] == 1 && m_bit[6] == 1 && m_bit[5] == 1 && m_bit[4] == 0) {
            m_valuableBits = 4;
            return 3;
        }
        if (m_bit[7] == 1 && m_bit[6] == 1 && m_bit[5] == 0) {
            m_valuableBits = 5;
            return 2;
        }
        if (m_bit[7] == 0) {
            m_valuableBits = 7;
            return 1;
        }
        if (m_bit[7] == 1 && m_bit[6] == 1 && m_bit[5] == 1 && m_bit[4] == 1 && m_bit[3] == 0)
        {
            return -1;
        }
        else {
            m_valuableBits = 6;
            return 7;
        }
    }

    void printBits() {
            cout << getConstant() << " " <<m_valuableBits <<endl;
    }

    //vratime hodnotu bytu bez predpony
    size_t charValueUTF() {

        //otestovano, funguje
        unsigned char result = m_bit.to_ulong();
        if (m_valuableBits == 3)
            result &= mask4bytes;
        if (m_valuableBits == 4)
            result &= mask3bytes;
        if (m_valuableBits == 5)
            result &= mask2bytes;
        if (m_valuableBits == 7)
            result &= mask1bytes;
        if (m_valuableBits == 6)
            result &= maskbytes;

        size_t res = result;
        size_t constant = getConstant();
        res *= constant;

        return res;
    }
};
//todo tady jsem skoncil, bude potreba uz jenom vymyslet jak to prevest na byte, prevod ze soubou na inty by mel byt okej

class CFibonacciEncoder
{
private:
public:
    // tady se pokusime nasimulovat jakou binarni hodnotu bude mit cislo v desitkove
    // musime ziskat nejvyssi clen fibonacciho posloupnosti, ktery je mensi nez cislo, ktere chceme dostat
    // zaroven abychom byli schopni zakodovat jednicku, tak musime vstupni cislo zvetsit o 1
    // potom budeme scitat hodnoty fibonacciho posloupnosti pospatku, dokud nedosahneme cisla, ktere chceme zakodovat
    // pokud se prvek fib. posloupnosti vejde do souctu tak do stringu pridame 1, pokud ne tak 0
    // na konec cisla pridame 1 abychom vedeli ze konci

    //vraci string 1 a 0 s ukoncovacim bitem, otestovano, funguje
    string uintToFib(size_t value)
    {
        value ++;
        size_t prev = 0;
        size_t current = 1;
        size_t last = 1;
        string res = "1";

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
                res += "1";
                value -= last;
            }
            else
            {
                res += "0";
            }
            if ( prev == 0)
                break;
            last = current;
            current = prev;
            prev = last - current;
        }
        reverse(res.begin(),res.end());
        return res;
    }
};
//--------------------------------------------------------------------------------------------------------------------------------
bool               utf8ToFibonacci                         ( const char      * inFile,
                                                             const char      * outFile )
{
    //v byte mame bitset, ze ktereho dostaneme vzdy pouze potrebne byty
    //nacteme prvni byte, zjistime kolik jeste je potreba nacist abychom meli cely znak
    //vzdy byte prevedeme na korespondujici uint a vynasobime ho konstantou ktera se odviji pole toho kolikaty je to byte
    //256^size-1, budeme asi muset vytvorit kopii size, kterou po dekodovani kazdeho uintu ubereme o 1
    CBuffer buffer;
    CFibonacciEncoder encoder;
    ifstream in;
    ofstream out;

    out.open(outFile,ios::binary);
    in.open(inFile,ios::binary);
    while ( true )
    {
      size_t res = 0; // promenna ve ktere budeme ukladat postupne hodnotu cisla
      CByte byte(in.get());

      if ( in.eof()  || in.fail() ) {
          break;
      }

      //zjistime na kolik bytu je znak, a prvni byte rovnou zapiseme
      size_t size = byte.GetSizeUTF();
      if (size == -1)
          return false;

      res += (byte.charValueUTF());

      for (size_t i = 1; i < size; i++) {
          //nacteme dalsi byty
          CByte byte2(in.get(), (size-1) - i);
          if (in.eof() || in.fail()) {
              return false;
            }
          if (byte2.GetSizeUTF() == -1)
              return false;
          //byte2.printBits();
          res += byte2.charValueUTF();
      }

      cout << res << endl;
      string binaryString = encoder.uintToFib(res);
      cout << binaryString << endl;
      //cout << res << " " << binaryString << endl;
      buffer.addToBufferUTF(binaryString,out);
    }
    buffer.addRest(out);
    in.close();
    out.close();
    return true;
  }

  
bool               fibonacciToUtf8                         ( const char      * inFile,
                                                             const char      * outFile )
{
    //todo funkce ktera prevede fibonacciho string s nevetsim bitem vlevo na int
    //int prevedeme na binary - podle velikosti zjistime kolik budeme potrebovat bytu, pokud bude potreba tak doplnime nulami aby to vychazelo
    //adekvatne rozmistime bity do bytu s potrebnymy maskami
    // nezapomenout ze zakodovane cislo je jakoby o 1 vyssi kvuli delicimu bitu

    // DULEZITE: do bufferu musime pridavat doleva ale odebirat zprava, neni potreba otacet,je potreba odebrat prebyvajici rozdelovaci bit
    //jinak budou vychazet spatne hodnoty
    // 00010001 v bufferu - pridame dalsi  --> 01000110 00010001, ted muzeme odebrat zprava vezmeme 110000100001 - jemu sebereme prvni jednicku
    //vypocteme dec hodnotu od ni odecteme jedna, muzeme vesele kodovat do binarni
    // nesmime zapomenout posunout buffer o 12 bitu doprava

  ifstream in;
  ofstream out ;
  in.open(inFile,ios::binary);
  out.open(outFile,ios::binary);
  CBuffer buffer;


  //buffer.addToBufferFIB(0x18,out);
  while (true)
  {
      char byte = in.get();
      if (in.eof() || in.fail())
          break;
      buffer.addToBufferFIB(byte,out);
  }
  in.close();
  out.close();
  return buffer.cleanBuffer();


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
  cout << endl << endl;
    //fibonacciToUtf8("example/src_8.fib", "bin.bin");
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
