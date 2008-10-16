#ifndef FR_MACRO
#define FR_MACRO

// This allow fast defining of property in class
#define FrPropMacro(Type, Name) \
    private: Type m_##Name; \
    public: \
        Type Get##Name () { return m_##Name; } \
        void Set##Name(Type Name) { m_##Name=Name; }

// This allow fast defining of property in class
#define FrSetPropMacro(Type, Name) \
    private: Type m_##Name; \
    public: \
        void Set##Name(Type Name) { m_##Name=Name; }

// This allow fast defining of property in class
#define FrGetPropMacro(Type, Name) \
    private: Type m_##Name; \
    public: \
        Type Get##Name () { return m_##Name; }

// This allow fast defining of property in class
#define FrGetRefPropMacro(Type, Name) \
    private: Type m_##Name; \
    public: \
        Type& Get##Name () { return m_##Name; }

// This copies values from source array to destiny array
#define COPY_ARR3(dst,src) dst[0]=src[0]; dst[1]=src[1]; dst[2]=src[2]
//This inits values of array
template<class T>
void INIT_ARR3(T dst[], T a, T b, T c){
    dst[0]=a; dst[1]=b; dst[2]=c;
}

#endif
