#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <db/WukPsql.hh>
#include <WukBuffer.hh>

namespace wuk::im {
    enum class ACCOUNT_STATUS {
        NORMAL,  // 正常账号
        BANNED,  // 封禁账号
        MUTED,   // 禁言账号
        DELETED, // 注销账号（已删除）
    };

    class LIBWUK_API Account {
    private:

    public:
        Account() = default;
    };
}

#endif
