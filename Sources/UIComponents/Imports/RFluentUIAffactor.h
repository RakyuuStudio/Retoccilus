#ifndef RetoUI_FluentUI_Affactor_h
#define RetoUI_FluentUI_Affactor_h

#define Q_PROPERTY_AUTOMATICALLY(Tp, M)                                                                                       \
Q_PROPERTY(Tp M Member _##M Notify M##Changed)                                                                     \
    public:                                                                                                                \
    Q_SIGNAL void M##Changed();                                                                                          \
    void M(Tp in_##M)                  {                                                                                 \
        _##M = in_##M;                                                                                                     \
        Q_EMIT M##Changed();                                                                                               \
}                                                                                                                    \
    Tp M()                              {                                                                               \
        return _##M;                                                                                                       \
}                                                                                                                    \
    private:                                                                                                               \
    Tp _##M;                                                                                                         \

#define Q_PROPERTY_READONLY_AUTOMATICALLY(Tp, M)                                                                                       \
Q_PROPERTY(Tp M Member _##M Notify M##Changed)                                                                     \
    public:                                                                                                                \
    Q_SIGNAL void M##Changed();                                                                                          \
    void M(Tp in_##M)                  {                                                                                 \
        _##M = in_##M;                                                                                                     \
        Q_EMIT M##Changed();                                                                                               \
}                                                                                                                    \
    Tp M()                              {                                                                               \
        return _##M;                                                                                                       \
}                                                                                                                    \
    private:                                                                                                               \
    Tp _##M;                                                                                                         \


#endif