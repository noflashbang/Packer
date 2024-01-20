
#ifndef SERIAL_H
#define SERIAL_H

//backward compatibility(remove in VS2008)
//#define sprintf_s(x,y,z,q) sprintf(x,z,q)
//#define _itoa_s(x,y,z,q) _itoa(x,y,q)

#include <string>
#include "AutoPtr.h"

class CSerializer //base serializer, pure virtual
{
public:
  virtual ~CSerializer(){};

  virtual void IO(short          &io)=0;
  virtual void IO(unsigned short &io)=0;
  virtual void IO(int            &io)=0;
  virtual void IO(unsigned int   &io)=0;
  virtual void IO(long           &io)=0;
  virtual void IO(unsigned long  &io)=0;
  virtual void IO(float          &io)=0;
  virtual void IO(double         &io)=0;
  virtual void IO(char           &io)=0;
  virtual void IO(unsigned char  &io)=0;
  virtual void IO(bool           &io)=0;
  virtual void IO(std::string    &io)=0;

  //arrays
  virtual void IOA(short*          io, int &len)=0;
  virtual void IOA(unsigned short* io, int &len)=0;
  virtual void IOA(int*            io, int &len)=0;
  virtual void IOA(unsigned int*   io, int &len)=0;
  virtual void IOA(long*           io, int &len)=0;
  virtual void IOA(unsigned long*  io, int &len)=0;
  virtual void IOA(float*          io, int &len)=0;
  virtual void IOA(double*         io, int &len)=0;
  virtual void IOA(char*           io, int &len)=0;
  virtual void IOA(bool*           io, int &len)=0;
};
//===============================================================
class CTextSerializerO : CSerializer //outputs text
{//Numbers will be quoted -> "12", strings will be terminated with a semicolon -> "This is a string;", Bools outputed as T or F.
public:
  CTextSerializerO(char* pBuffer, unsigned bufferLength);
  virtual ~CTextSerializerO(){};

  virtual void IO(short &io);
  virtual void IO(unsigned short &io);
  virtual void IO(int &io);
  virtual void IO(unsigned int &io);
  virtual void IO(long &io);
  virtual void IO(unsigned long &io);
  virtual void IO(float &io);
  virtual void IO(double &io);
  virtual void IO(char &io);
  virtual void IO(unsigned char &io);
  virtual void IO(bool &io);
  virtual void IO(std::string &io);

  virtual void IOA(short*          io, int &len);
  virtual void IOA(unsigned short* io, int &len);
  virtual void IOA(int*            io, int &len);
  virtual void IOA(unsigned int*   io, int &len);
  virtual void IOA(long*           io, int &len);
  virtual void IOA(unsigned long*  io, int &len);
  virtual void IOA(float*          io, int &len);
  virtual void IOA(double*         io, int &len);
  virtual void IOA(char*           io, int &len);
  virtual void IOA(bool*           io, int &len);

  unsigned GetBytesWritten();
  bool HasOverflowed();
protected:
  char* m_Buffer;
  unsigned m_BufferLength;
  unsigned m_BytesWritten;
  bool m_OverFlowed;

  CTextSerializerO(){};
};

class CTextSerializerI : CSerializer //reads text
{//Numbers must be quoted -> "12", strings must be terminated with a semicolon -> "This is a string;", bools can be (T or F) or (1 or 0)
public:
  CTextSerializerI(char* pBuffer, unsigned bufferLength);
  virtual ~CTextSerializerI(){};

  virtual void IO(short &io);
  virtual void IO(unsigned short &io);
  virtual void IO(int &io);
  virtual void IO(unsigned int &io);
  virtual void IO(long &io);
  virtual void IO(unsigned long &io);
  virtual void IO(float &io);
  virtual void IO(double &io);
  virtual void IO(char &io);
  virtual void IO(unsigned char &io);
  virtual void IO(bool &io);
  virtual void IO(std::string &io);

  virtual void IOA(short*          io, int &len);
  virtual void IOA(unsigned short* io, int &len);
  virtual void IOA(int*            io, int &len);
  virtual void IOA(unsigned int*   io, int &len);
  virtual void IOA(long*           io, int &len);
  virtual void IOA(unsigned long*  io, int &len);
  virtual void IOA(float*          io, int &len);
  virtual void IOA(double*         io, int &len);
  virtual void IOA(char*           io, int &len);
  virtual void IOA(bool*           io, int &len);

  unsigned GetBytesRead();
  bool HasOverflowed();
protected:
  char* m_Buffer;
  unsigned m_BufferLength;
  unsigned m_BytesRead;
  bool m_OverFlowed;

  CTextSerializerI(){};
};
//============================================================
class CTextFBSerializerO : public CTextSerializerO //outputs text - assumes use of full buffer
{//bools outputed as T or F.
public:
  CTextFBSerializerO(char* pBuffer, unsigned bufferLength):CTextSerializerO(pBuffer,bufferLength) {};
  virtual ~CTextFBSerializerO(){};

  using CTextSerializerO::IO; //unhides IO(char&) and IO(bool&)
  virtual void IO(short &io);
  virtual void IO(unsigned short &io);
  virtual void IO(int &io);
  virtual void IO(unsigned int &io);
  virtual void IO(long &io);
  virtual void IO(unsigned long &io);
  virtual void IO(float &io);
  virtual void IO(double &io);
  virtual void IO(std::string &io);

  virtual void IOA(short*          io, int &len);
  virtual void IOA(unsigned short* io, int &len);
  virtual void IOA(int*            io, int &len);
  virtual void IOA(unsigned int*   io, int &len);
  virtual void IOA(long*           io, int &len);
  virtual void IOA(unsigned long*  io, int &len);
  virtual void IOA(float*          io, int &len);
  virtual void IOA(double*         io, int &len);
  virtual void IOA(char*           io, int &len);
  virtual void IOA(bool*           io, int &len);

protected:

  CTextFBSerializerO(){};
};

class CTextFBSerializerI : public CTextSerializerI //reads text - assumes use of full buffer, bools can be (T or F) or (1 or 0)
{//assumes use of full buffer. Entire buffer will be loaded for numbers and strings, chars and bools will only use 1 char,
public:
  CTextFBSerializerI(char* pBuffer, unsigned bufferLength):CTextSerializerI(pBuffer,bufferLength) {};
  virtual ~CTextFBSerializerI(){};

  using CTextSerializerI::IO; //unhides IO(char&) and IO(bool&)
  virtual void IO(short &io);
  virtual void IO(unsigned short &io);
  virtual void IO(int &io);
  virtual void IO(unsigned int &io);
  virtual void IO(long &io);
  virtual void IO(unsigned long &io);
  virtual void IO(float &io);
  virtual void IO(double &io);
  virtual void IO(std::string &io);

  virtual void IOA(short*          io, int &len);
  virtual void IOA(unsigned short* io, int &len);
  virtual void IOA(int*            io, int &len);
  virtual void IOA(unsigned int*   io, int &len);
  virtual void IOA(long*           io, int &len);
  virtual void IOA(unsigned long*  io, int &len);
  virtual void IOA(float*          io, int &len);
  virtual void IOA(double*         io, int &len);
  virtual void IOA(char*           io, int &len);
  virtual void IOA(bool*           io, int &len);

protected:

  CTextFBSerializerI(){};
};
//===============================================================
class CBinarySerializerO : CSerializer //outputs binary
{
public:
  CBinarySerializerO(char* pBuffer, unsigned bufferLength);
  virtual ~CBinarySerializerO(){};

  virtual void IO(short &io);
  virtual void IO(unsigned short &io);
  virtual void IO(int &io);
  virtual void IO(unsigned int &io);
  virtual void IO(long &io);
  virtual void IO(unsigned long &io);
  virtual void IO(float &io);
  virtual void IO(double &io);
  virtual void IO(char &io);
  virtual void IO(unsigned char &io);
  virtual void IO(bool &io);
  virtual void IO(std::string &io);

  virtual void IOA(short*          io, int &len);
  virtual void IOA(unsigned short* io, int &len);
  virtual void IOA(int*            io, int &len);
  virtual void IOA(unsigned int*   io, int &len);
  virtual void IOA(long*           io, int &len);
  virtual void IOA(unsigned long*  io, int &len);
  virtual void IOA(float*          io, int &len);
  virtual void IOA(double*         io, int &len);
  virtual void IOA(char*           io, int &len);
  virtual void IOA(bool*           io, int &len);

  unsigned GetBytesWritten();
  bool HasOverflowed();
protected:
  char* m_Buffer;
  unsigned m_BufferLength;
  unsigned m_BytesWritten;
  bool m_OverFlowed;

  CBinarySerializerO(){};
};

class CBinarySerializerI : CSerializer //reads binary
{
public:
  CBinarySerializerI(char* pBuffer, unsigned bufferLength);
  virtual ~CBinarySerializerI(){};

  virtual void IO(short &io);
  virtual void IO(unsigned short &io);
  virtual void IO(int &io);
  virtual void IO(unsigned int &io);
  virtual void IO(long &io);
  virtual void IO(unsigned long &io);
  virtual void IO(float &io);
  virtual void IO(double &io);
  virtual void IO(char &io);
  virtual void IO(unsigned char &io);
  virtual void IO(bool &io);
  virtual void IO(std::string &io);

  virtual void IOA(short*          io, int &len);
  virtual void IOA(unsigned short* io, int &len);
  virtual void IOA(int*            io, int &len);
  virtual void IOA(unsigned int*   io, int &len);
  virtual void IOA(long*           io, int &len);
  virtual void IOA(unsigned long*  io, int &len);
  virtual void IOA(float*          io, int &len);
  virtual void IOA(double*         io, int &len);
  virtual void IOA(char*           io, int &len);
  virtual void IOA(bool*           io, int &len);

  unsigned GetBytesRead();
  bool HasOverflowed();
protected:
  char* m_Buffer;
  unsigned m_BufferLength;
  unsigned m_BytesRead;
  bool m_OverFlowed;

  CBinarySerializerI(){};
};
//============================================================

#endif