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

#endif
