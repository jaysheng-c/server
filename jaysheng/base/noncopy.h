#ifndef __JAYSHENG_NONCOPY_H__
#define __JAYSHENG_NONCOPY_H__

namespace jaysheng {

// 禁止拷贝，赋值的类
class Noncopy {
public:
    Noncopy() = default;
    ~Noncopy() = default;

    Noncopy(const Noncopy &) = delete;
    Noncopy &operator==(const Noncopy &) = delete;
};

} // namespace jaysheng

#endif // !__JAYSHENG_NONCOPY_H__