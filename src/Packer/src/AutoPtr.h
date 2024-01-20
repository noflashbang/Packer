#ifndef AUTO_POINTER
#define AUTO_POINTER

template <typename T>
class AutoPointer
{
public:
  void Build()
  {
    if(m_Ptr != NULL)
      Release();
    
    if(!m_Array)
    {
      m_Ref = new long;
      (*m_Ref) = 1;
      m_Ptr = new T();
      m_SizeOf = sizeof(T);
    }
    else
    {
      m_Ref = new long;
      (*m_Ref) = 1;
      m_Ptr = new T[m_Size];
      m_SizeOf = sizeof(T) * m_Size;
    }
  };
  AutoPointer()
  {
    m_Ref = NULL;
    m_Ptr = NULL;
    m_Size = 1;
    m_Array = false;
  };
  AutoPointer(int size)
  {
    m_Ref = NULL;
    m_Ptr = NULL;
    m_Size = size;
    m_Array = true;
  };
  AutoPointer(AutoPointer<T> &ref)
  {
    if(ref.m_Ptr == NULL)
        ref.Build();

    m_Ref = ref.m_Ref;
    (*m_Ref)++;
    m_Ptr = ref.m_Ptr;
    m_Size = ref.m_Size;
    m_Array =  ref.m_Array;
    m_SizeOf = ref.m_SizeOf;
  };
  AutoPointer<T>& operator= (AutoPointer<T> &ref)
  {
    if(this != &ref)
    {
      if(ref.m_Ptr == NULL)
        ref.Build();

      if(m_Ptr != NULL)
        Release();

      m_Ref = ref.m_Ref;
      (*m_Ref)++;
      m_Ptr = ref.m_Ptr;
      m_Size = ref.m_Size;
      m_Array =  ref.m_Array;
      m_SizeOf = ref.m_SizeOf;
    }
    return *this;
  }
  operator T*()
  {
    if(m_Ptr == NULL)
      Build();

    return m_Ptr;
  }
  T* operator->()
  {
    if(m_Ptr == NULL)
      Build();

    return m_Ptr;
  };
  T& operator*()
  {
   if(m_Ptr == NULL)
      Build();
   
    return (*m_Ptr);
  };
  //array operators for all integer types -----
  T& operator[](short offset)
  {
    if(m_Ptr == NULL)
      Build();

    if(m_Array)
    {
      return m_Ptr[offset];
    }
    else
    {
      return (*m_Ptr);
    }
  };
  T& operator[](unsigned short offset)
  {
    if(m_Ptr == NULL)
      Build();

    if(m_Array)
    {
      return m_Ptr[offset];
    }
    else
    {
      return (*m_Ptr);
    }
  };
  T& operator[](int offset)
  {
    if(m_Ptr == NULL)
      Build();

    if(m_Array)
    {
      return m_Ptr[offset];
    }
    else
    {
      return (*m_Ptr);
    }
  };
  T& operator[](unsigned int offset)
  {
    if(m_Ptr == NULL)
      Build();

    if(m_Array)
    {
      return m_Ptr[offset];
    }
    else
    {
      return (*m_Ptr);
    }
  };
  T& operator[](long offset)
  {
    if(m_Ptr == NULL)
      Build();

    if(m_Array)
    {
      return m_Ptr[offset];
    }
    else
    {
      return (*m_Ptr);
    }
  };
  T& operator[](unsigned long offset)
  {
    if(m_Ptr == NULL)
      Build();

    if(m_Array)
    {
      return m_Ptr[offset];
    }
    else
    {
      return (*m_Ptr);
    }
  };
  //-----
  void Release()
  {
    if(m_Ref != NULL)
    {
      if((*m_Ref) == 1)
      {
        if(m_Array)
          delete [] m_Ptr;
       else
          delete m_Ptr;

        delete m_Ref;
      }
      else
      {
        (*m_Ref)--;
      }
    }
    m_Ptr = NULL;
    m_Ref = NULL;
  }
  ~AutoPointer()
  {
    Release();
  };
  int SizeOf()
  {
    if(m_Ptr == NULL)
      Build();

    return m_SizeOf;
  };
  void MemSet(T value)
  {
    if(m_Ptr == NULL)
      Build();

    if(m_Array)
    {
     for(int aa = 0; aa < m_Size; aa++)
       m_Ptr[aa] = value;
    }
    else
    {
      (*m_Ptr) = value;
    }
  }
private:
  T* m_Ptr;
  long m_Size;
  long m_SizeOf;
  bool m_Array;
  long* m_Ref;
};

#endif