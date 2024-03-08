#ifndef RetoUI_FluentUI_SingletonHandle_h
#define RetoUI_FluentUI_SingletonHandle_h

template <typename Tp>
class SingletonHandle {
public:
    static Tp* getInstance();
};

template <typename Tp>
Tp* SingletonHandle<Tp>::getInstance() {
    static Tp instance;
    return &instance;
}

#define SINGLETON_HANDLE(X) \
    friend class SingletonHandle<X>; \
    public: \
    static X* getInstance() { \
        return SingletonHandle<X>::getInstance(); \
    }

#endif // RetoUI_FluentUI_SingletonHandle_h